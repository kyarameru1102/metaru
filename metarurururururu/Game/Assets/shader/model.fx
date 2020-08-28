/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	            //アルベド。
Texture2D<float4> g_shadowMap0 : register(t2);              //シャドウマップ1枚目。
Texture2D<float4> g_shadowMap1 : register(t5);              //シャドウマップ2枚目。
Texture2D<float4> g_shadowMap2 : register(t6);              //シャドウマップ3枚目。
Texture2D<float4> specularMap : register(t3);		        //スペキュラマップ。
TextureCube<float4> skyCubeMapReflection : register(t4);	//スカイキューブマップ。

//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//ライトビュー行列。
	float4x4 mLightView[3];	//ライトビュー行列。
	float4x4 mLightProj[3];	//ライトプロジェクション行列。
	int isShadowReciever;	//シャドウレシーバーフラグ。
	int isHasSpecularMap;	//スペキュラマップあるかどうか。
	int	isAlpha;			//スペキュラマップがαに入っているかどうか。
};

//ライト用の定数バッファ。
struct DirectionL {
	float3 dligDirection;
	float4 dligColor;
};
cbuffer LightCb : register(b1) {
	DirectionL drg;
	float4 ambient;
	float3 eye;	
	float specPow;
};

//シャドウマップ用の定数バッファ
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
}

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
    float4 WorldPos     : TEXCOORD1;    
    float4 posInLVP[3]  : TEXCOORD2;        //ライトビュープロジェクション空間での座標。
};

/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}


//float Beckmann(float m, float t)
//{
//    float M = m * m;
//    float T = t * t;
//    return exp((T - 1) / (M * T)) / (M * T * T);
//}

//float spcFresnel(float f0, float u)
//{
//    // from Schlick
//    return f0 + (1 - f0) * pow(1 - u, 5);
//}

//float BRDF(float3 L, float3 V, float3 N)
//{
//    float microfacet = 1.0f;
//    float f0 = 0.5;
//    bool include_F = 0;
//    bool include_G = 0;
//    // compute the half float3
//    float3 H = normalize(L + V);

//    float NdotH = dot(N, H);
//    float VdotH = dot(V, H);
//    float NdotL = dot(N, L);
//    float NdotV = dot(N, V);
 	
//    float D = Beckmann(microfacet, NdotH);
//    float F = spcFresnel(f0, VdotH);
	
//    float t = 2.0 * NdotH / VdotH;
//    float G = max(0.0f, min(1.0, min(t * NdotV, t * NdotL)));
//    float m = 3.14159265 * NdotV * NdotL;
//	/*
//    NdotH = NdotH + NdotH;
//    float G = (NdotV < NdotL) ? 
//        ((NdotV*NdotH < VdotH) ?
//         NdotH / VdotH :
//         oneOverNdotV)
//        :
//        ((NdotL*NdotH < VdotH) ?
//         NdotH*NdotL / (VdotH*NdotV) :
//         oneOverNdotV);
//	*/
//    //if (include_G) G = oneOverNdotV;
//    return max(F * D * G / m, 0.0);
//}

//float SchlickFresnel(float u, float f0, float f90)
//{
//    return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
//}

//float3 NormalizedDisneyDiffuse(float3 N, float3 L, float3 V, float roughness)
//{
//  //光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める。
//    float3 H = normalize(L + V);

//    float energyBias = lerp(0.0f, 0.5f, roughness);
//    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
//  //光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める。
//    float dotLH = saturate(dot(L, H));
//  //法線と光源に向かうベクトルがどれだけ似ているかを内積で求める。
//    float dotNL = saturate(dot(N, L));
//  //法線と視線に向かうベクトルがどれだけ似ているかを内積で求める。
//    float dotNV = saturate(dot(N, V));

//    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
  
//    float FL = SchlickFresnel(1.0f, Fd90, dotNL);
//    float FV = SchlickFresnel(1.0f, Fd90, dotNV);
//    return (FL * FV) / PI;
//}


/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	float4 worldPos = pos;
	psInput.WorldPos = pos;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
        for (int i = 0; i < 3; i++)
        {
            psInput.posInLVP[i] = mul(mLightView[i], worldPos);
            psInput.posInLVP[i] = mul(mLightProj[i], psInput.posInLVP[i]);
        }
    }

	//UV座標はそのままピクセルシェーダーに渡す。
	psInput.TexCoord = In.TexCoord;
	//法線はそのままピクセルシェーダーに渡す。
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	psInput.WorldPos = pos;
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
        for (int i = 0; i < 3; i++)
        {
            psInput.posInLVP[i] = mul(mLightView[i], psInput.WorldPos);
            psInput.posInLVP[i] = mul(mLightProj[i], psInput.posInLVP[i]);
        }
    }

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	/*return albedoTexture.Sample(Sampler, In.TexCoord);*/
	//ディレクションライトの拡散反射光を計算する。
	float3 lig = max(0.0f, dot(In.Normal, drg.dligDirection * -1.0f)) * drg.dligColor;
	lig.xyz += ambient.xyz;//float3(0.5f, 0.5f,0.5f);
	//アルベドカラーを引っ張ってくる。
	float4 albedo = albedoTexture.Sample(
		Sampler,
		In.TexCoord
	);
	//ディレクションライトの鏡面反射を計算する。

	//ライトを当てる面から視点に伸びるベクトルを計算する。
	float3 toEyeDir = normalize(eye - In.WorldPos.xyz);
	//反射を計算する。
	float3 R = -toEyeDir + 2.0f * dot(In.Normal, toEyeDir) * In.Normal;
	R = normalize(R);
	float3 direction = normalize(drg.dligDirection);
	//反射ベクトルとディレクションライトの方向との内積をとってスペキュラの強さを計算する。
	float3 spec = max(0.0f, dot(-direction, R));
	float specPower = 0.0f;
	float4 specMap;
	//スペキュラマップがある。
	if (isHasSpecularMap == 1) {
		if (isAlpha == 1) {
			specPower = albedo.a;
			specPower *= 20.0f;
		}
		else {
			specMap = specularMap.Sample(
				Sampler,
				In.TexCoord
			);
			specPower = specMap.x * 5.0;
		}
		
	}
	//pow関数使っていい感じにする。
	spec = pow(spec, specPow);
	//スペキュラ反射の計算結果をligに加算する。
	lig += spec * drg.dligColor * specPower;
	if (isShadowReciever == 1) {	//シャドウレシーバー。
        for (int i = 0; i < 3; i++)
        {
		//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
            float2 shadowMapUV = In.posInLVP[i].xy / In.posInLVP[i].w;
            shadowMapUV *= float2(0.5f, -0.5f);
            shadowMapUV += 0.5f;
		//シャドウマップの範囲内かどうかを判定する。
            if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			)
            {
			///LVP空間での深度値を計算。
                float zInLVP = In.posInLVP[i].z / In.posInLVP[i].w;
			//シャドウマップに書き込まれている深度値を取得。
                float zInShadowMap;
                if (i == 0)
                {
                    zInShadowMap = g_shadowMap0.Sample(Sampler, shadowMapUV);
                }
                else if (i == 1)
                {
                    zInShadowMap = g_shadowMap1.Sample(Sampler, shadowMapUV);
                }
                else if (i == 2)
                {
                    zInShadowMap = g_shadowMap2.Sample(Sampler, shadowMapUV);
                }
                if (zInLVP > zInShadowMap + 0.0001f)
                {
				//影が落ちているので、光を弱くする
                    lig *= 0.5f;
                  
                }
                break;
            }
        }
    }
	float4 final = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 skyColor = skyCubeMapReflection.Sample(Sampler, In.Normal);

	if (isHasSpecularMap == 1) {
		if (isAlpha == 1) {
			albedo.xyz = lerp(albedo.xyz, skyColor.xyz, albedo.a);
		}
		else {
			albedo.xyz = lerp(albedo.xyz, skyColor.xyz, specMap.x / 5.0f);
		}
	}
	final.xyz = albedo.xyz * lig;
	
	//セピア調にする。
	//{
    //    float4 NewColor;
    //    NewColor.r = (final.r * 0.593) + (final.g * 0.969) + (final.b * 0.389);
    //    NewColor.g = (final.r * 0.449) + (final.g * 0.786) + (final.b * 0.268);
    //    NewColor.b = (final.r * 0.272) + (final.g * 0.534) + (final.b * 0.131);
    //+   final.r = NewColor * 1.2;
    //    final.g = NewColor * 1.1;
    //    final.b = NewColor * 1.1;
    //}
	return float4(final.xyz, 1.0f);
}
/// <summary>
/// シャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}

TextureCube<float4> skyCubeMap : register(t0);	//スカイキューブマップ。

/// <summary>
///	空用シェーダー。
/// </summary>
float4 PSMain_SkyCube(PSInput In) : SV_Target0
{
	float4 color = skyCubeMap.Sample(Sampler, In.Normal);
	return color;
}