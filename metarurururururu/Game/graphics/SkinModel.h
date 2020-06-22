#pragma once

#include "Skeleton.h"
#include "SkinModelEffect.h"

/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	スキンモデルクラス。
*/
class SkinModel
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw( CMatrix viewMatrix, CMatrix projMatrix );
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	void SetSkeleton(Skeleton& skeleton)
	{
		m_skeletonRef = &skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	/*!
		*@brief	モデルマテリアルの検索。
		*@param[in]	findEffect		マテリアルを見つけた時に呼ばれるコールバック関数
		*/
	void FindMaterial(std::function<void(ModelEffect*)> findMaterial) const
	{
		FindMesh([&](auto& mesh) {
			ModelEffect* effect = reinterpret_cast<ModelEffect*>(mesh->effect.get());
			findMaterial(effect);
			});
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};
	/// <summary>
	/// シャドウレシーバーのフラグを設定する。
	/// </summary>
	/// <param name="flag">trueを渡すとシャドウレシーバーになる</param>
	/// <remarks>
	/// シャドウレシーバーとは影を落とされるオブジェクトのことです。
	/// シャドウキャスターによって生成された、シャドウマップを利用して
	/// 自身に影を落とします。
	/// オブジェクトがシャドウレシーバーかつシャドウキャスターになっている場合は
	/// セルフシャドウ(自分の影が自分に落ちる)を行うことができます。
	/// </remarks>
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
	/// <summary>
	///	αに入ってるかどうか。
	/// </summary>
	enum WhetherAlpha {
		Alpha,
		None,
	};
	/// <summary>
	/// スペキュラマップを設定。
	/// </summary>
	/// <param name="srv"></param>
	void SetSpecularMap(ID3D11ShaderResourceView* srv,WhetherAlpha whetherAlpha)
	{
		m_specularMapSRV = srv;
		m_isAlpha = whetherAlpha;
	}
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);
	//ライトの初期化。
	void InitLight();
	//アルベドテクスチャの初期化。
	void InitAlbedoTexture();
private:
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//ライトビュー行列。
		CMatrix mLightProj;		//ライトプロジェクション行列。
		int isShadowReciever;	//シャドウレシーバーのフラグ。
		int isHasSpecuraMap;	//スペキュラマップを保持しているかどうかのフラグ。
		int isAlpha;			//スペキュラマップがαに入っているかどうか。
	};
	
	//ディレクションライト。
	struct SDirectionLight {
		CVector4 direction;		//ライトの方向。
		CVector4 color;			//ライトの色。
	};

	//ライト。
	struct Light {
		SDirectionLight dirLig;
		CVector4 Ambient;
		CVector3 eyePos;
		float specPow;
	};
	EnFbxUpAxis					m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
	ID3D11Buffer*				m_cb = nullptr;					//!<定数バッファ。
	Skeleton*					m_skeletonRef = nullptr;		//!<外部のスケルトン。指定されていなければnullptr
	Skeleton					m_skeleton;						//!<スケルトン。
	CMatrix						m_worldMatrix;					//!<ワールド行列。
	DirectX::Model*				m_modelDx;						//!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState*			m_samplerState = nullptr;		//!<サンプラステート。
	ID3D11Buffer*				m_lightCb = nullptr;			//ライト用の定数バッファ。
	Light						m_Light;						//ライト。
	ID3D11ShaderResourceView*	m_albedoTextureSRV = nullptr;	//アルベドテクスチャのSRV。
	ID3D11ShaderResourceView*   m_shadowMapSRV = nullptr;		//シャドウマップのSRV.
	ID3D11ShaderResourceView*	m_specularMapSRV = nullptr;		//スペキュラマップのSRV
	bool						m_isShadowReciever = false;		//シャドウレシーバーのフラグ。
	bool						isHasSpecuraMap = false;		//スペキュラマップのフラグ。
	ID3D11ShaderResourceView*	m_skyCube = nullptr;			//スカイキューブマップのSRV
	WhetherAlpha				m_isAlpha = WhetherAlpha::None;
};

