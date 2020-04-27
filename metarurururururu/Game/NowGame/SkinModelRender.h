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
	//重みでブレンディングする方
	void Init(const wchar_t* filePath,
		bool blend,	
		AnimationClip* animationClips = nullptr, int
		numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ
	);

	//アニメーションの再生。
	void PlayAnimation(int animNo, Body body, float interpolateTime = 0.0f)
	{
		if (body == Body::enUpperBody) {
			m_animation.Play(animNo, interpolateTime);
			m_animation.SetBody(body);
		}
		else if(body == Body::enLowerBody) {
			m_animation2.Play(animNo, interpolateTime);
			m_animation2.SetBody(body);
		}
		else if (body == Body::enWholeBody) {
			m_animation.Play(animNo, interpolateTime);
			m_animation.SetBody(body);
		}
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
	//アニメーションの初期化。
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips,Skeleton& skeletonUpper,Skeleton& skeletonLower);
	/// <summary>
	/// モデルをレンダリングするかどうかの関数。
	/// </summary>
	/// <param name="on">true レンダリングする。
	/// false レンダリングしない。</param>
	void SetRenderOn(bool on)
	{
		m_RenderOn = on;
	}
	/// <summary>
	/// シャドウレシーバーに登録するかどうかの関数
	/// </summary>
	/// <param name="reciever">
	/// true	登録する。
	/// false	登録解除。
	/// </param>
	void SetShadowReciever(bool reciever)
	{
		m_shadowRecieverOn = reciever;
	}
	void SetShadowCaster(bool caster)
	{
		m_shadowCasterOn = caster;
	}
	Animation& GetAnimation()  {
		return m_animation2;
	}
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
	Animation					m_animation;							//上半身アニメーション。
	Animation					m_animation2;							//下半身アニメーション。
	bool						m_isUpdateAnimation = true;				//アニメーションを連続再生するか？
	bool						m_RenderOn = true;						//描画されるかどうか。
	bool                        m_renderOK = false;						//Updateの後にレンダーが呼ばれるようにするフラグ。
	bool						m_shadowRecieverOn = true;				//影を表示される側になるかどうか。
	bool						m_shadowCasterOn = true;				//影を落とすかどうか。
	bool						m_UpperBody = true;						//上半身のアニメーションを流すならtrue。
	Skeleton					m_skeletonUpper;				//上半身のアニメーション用のスケルトン。
	Skeleton					m_skeletonLower;				//下半身のアニメーション用のスケルトン。
	Skeleton					m_blendSkeleton;				//上半身と下半身のアニメーションを合成したスケルトン。
	bool						m_blend = false;
};

