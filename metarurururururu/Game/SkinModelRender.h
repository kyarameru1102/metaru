#pragma once
#include "ShadowMap.h"

//3Dモデルを表示する機能を提供するクラス。
class SkinModelRender : public IGameObject
{
	//IGameObjectなのでNewGOでインスタンスを作成する必要がある。
public:
	SkinModelRender();
	void OnDestroy() override;
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <param name="filePath">
	/// ロードしたいcmoファイルのファイルパス。
	/// </param>
	/// <param name="animationClips">
	/// アニメーションクリップの配列の先頭アドレス。
	/// </param>
	/// <param name="numAnimationClips">
	/// アニメーションクリップの数。
	/// </param>
	/// <param name="fbxUpAxis">
	/// FBXの上方向。
	/// </param>
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,int
		numAnimationClips = 0, 
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ
	);
	//アニメーションの再生。
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}
	//アニメーションの更新フラグを設定する関数。
	//falseなら再生をやめる。
	void SetUpdateAnimationFlag(bool flag)
	{
		m_isUpdateAnimation = flag;
	}
	//3Dモデルを表示するポジションを設定する関数。
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//向いている方向を設定する関数。
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	//大きさを設定する関数。
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	//スキンモデルを取得。
	SkinModel& GetSkinModel()
	{
		return m_skinModel;
	}
	//アニメーションの初期化。
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);
	void SetRenderOn(bool on)
	{
		m_RenderOn = on;
	}
	/*auto* GetShadowMap()
	{
		return &m_shadowMap;
	}*/
private:
	bool Start() override;
	void Update() override;
	void Render() override;
private:
	AnimationClip*				m_animationClips = nullptr;				//アニメーションクリップ。
	int							m_numAnimationClips = 0;				//アニメーションクリップの数。
	CVector3 					m_position = CVector3::Zero();			//座標。
	CQuaternion	 				m_rotation = CQuaternion::Identity();	//回転。
	CVector3					m_scale = CVector3::One();				//拡大率。
	EnFbxUpAxis					m_enFbxUpAxis = enFbxUpAxisZ;			//FBXの上方向。
	SkinModel					m_skinModel;							//スキンモデル。
	Animation					m_animation;							//アニメーション。
	bool						m_isUpdateAnimation = true;				//アニメーションを連続再生するか？
	//ShadowMap					m_shadowMap;							//シャドウマップ。
	bool						m_RenderOn = true;						//描画されるかどうか。
};

