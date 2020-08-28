/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);	            //�A���x�h�B
Texture2D<float4> g_shadowMap0 : register(t2);              //�V���h�E�}�b�v1���ځB
Texture2D<float4> g_shadowMap1 : register(t5);              //�V���h�E�}�b�v2���ځB
Texture2D<float4> g_shadowMap2 : register(t6);              //�V���h�E�}�b�v3���ځB
Texture2D<float4> specularMap : register(t3);		        //�X�y�L�����}�b�v�B
TextureCube<float4> skyCubeMapReflection : register(t4);	//�X�J�C�L���[�u�}�b�v�B

//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//���C�g�r���[�s��B
	float4x4 mLightView[3];	//���C�g�r���[�s��B
	float4x4 mLightProj[3];	//���C�g�v���W�F�N�V�����s��B
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
	int isHasSpecularMap;	//�X�y�L�����}�b�v���邩�ǂ����B
	int	isAlpha;			//�X�y�L�����}�b�v�����ɓ����Ă��邩�ǂ����B
};

//���C�g�p�̒萔�o�b�t�@�B
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

//�V���h�E�}�b�v�p�̒萔�o�b�t�@
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//���C�g�r���[�v���W�F�N�V�����s��B
}

/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
    float4 WorldPos     : TEXCOORD1;    
    float4 posInLVP[3]  : TEXCOORD2;        //���C�g�r���[�v���W�F�N�V������Ԃł̍��W�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};
/*!
 *@brief	�X�L���s����v�Z�B
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
//  //�����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�B
//    float3 H = normalize(L + V);

//    float energyBias = lerp(0.0f, 0.5f, roughness);
//    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
//  //�����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�B
//    float dotLH = saturate(dot(L, H));
//  //�@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�B
//    float dotNL = saturate(dot(N, L));
//  //�@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�B
//    float dotNV = saturate(dot(N, V));

//    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
  
//    float FL = SchlickFresnel(1.0f, Fd90, dotNL);
//    float FV = SchlickFresnel(1.0f, Fd90, dotNV);
//    return (FL * FV) / PI;
//}


/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
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
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
        for (int i = 0; i < 3; i++)
        {
            psInput.posInLVP[i] = mul(mLightView[i], worldPos);
            psInput.posInLVP[i] = mul(mLightProj[i], psInput.posInLVP[i]);
        }
    }

	//UV���W�͂��̂܂܃s�N�Z���V�F�[�_�[�ɓn���B
	psInput.TexCoord = In.TexCoord;
	//�@���͂��̂܂܃s�N�Z���V�F�[�_�[�ɓn���B
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
	}
	psInput.WorldPos = pos;
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
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
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	/*return albedoTexture.Sample(Sampler, In.TexCoord);*/
	//�f�B���N�V�������C�g�̊g�U���ˌ����v�Z����B
	float3 lig = max(0.0f, dot(In.Normal, drg.dligDirection * -1.0f)) * drg.dligColor;
	lig.xyz += ambient.xyz;//float3(0.5f, 0.5f,0.5f);
	//�A���x�h�J���[�����������Ă���B
	float4 albedo = albedoTexture.Sample(
		Sampler,
		In.TexCoord
	);
	//�f�B���N�V�������C�g�̋��ʔ��˂��v�Z����B

	//���C�g�𓖂Ă�ʂ��王�_�ɐL�т�x�N�g�����v�Z����B
	float3 toEyeDir = normalize(eye - In.WorldPos.xyz);
	//���˂��v�Z����B
	float3 R = -toEyeDir + 2.0f * dot(In.Normal, toEyeDir) * In.Normal;
	R = normalize(R);
	float3 direction = normalize(drg.dligDirection);
	//���˃x�N�g���ƃf�B���N�V�������C�g�̕����Ƃ̓��ς��Ƃ��ăX�y�L�����̋������v�Z����B
	float3 spec = max(0.0f, dot(-direction, R));
	float specPower = 0.0f;
	float4 specMap;
	//�X�y�L�����}�b�v������B
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
	//pow�֐��g���Ă��������ɂ���B
	spec = pow(spec, specPow);
	//�X�y�L�������˂̌v�Z���ʂ�lig�ɉ��Z����B
	lig += spec * drg.dligColor * specPower;
	if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
        for (int i = 0; i < 3; i++)
        {
		//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
            float2 shadowMapUV = In.posInLVP[i].xy / In.posInLVP[i].w;
            shadowMapUV *= float2(0.5f, -0.5f);
            shadowMapUV += 0.5f;
		//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
            if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			)
            {
			///LVP��Ԃł̐[�x�l���v�Z�B
                float zInLVP = In.posInLVP[i].z / In.posInLVP[i].w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
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
				//�e�������Ă���̂ŁA�����キ����
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
	
	//�Z�s�A���ɂ���B
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
/// �V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
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
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}

TextureCube<float4> skyCubeMap : register(t0);	//�X�J�C�L���[�u�}�b�v�B

/// <summary>
///	��p�V�F�[�_�[�B
/// </summary>
float4 PSMain_SkyCube(PSInput In) : SV_Target0
{
	float4 color = skyCubeMap.Sample(Sampler, In.Normal);
	return color;
}