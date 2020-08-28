#pragma once
#include "Sprite.h"
#include "sound/SoundSource.h"


class Ground;
class Sky;
class TitleCamera;
class Title : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Title();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Title();
	/// <summary>
	/// 開始関数。
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update()override;
	void Render()override;
private:
	enum EnAnimationClip {
		enAnimationClip_idle,								//待機。
		enAnimationClip_Num,								//アニメーションクリップの数。
	};
	AnimationClip				m_animClips[enAnimationClip_Num];					//アニメーションクリップ。
	ID3D11ShaderResourceView*	m_specMapSRV = nullptr;								//スペキュラマップのSRV
	SkinModelRender*			m_skinModelRender = nullptr;						//スキンモデルレンダー。
	Sprite						m_sprite;											//スプライト。
	bool						m_on = false;										//Aボタンが押された時のフラグ。
	int							m_count = 0;										//カウンター。
	CSoundSource				m_se;												//SE。
	TitleCamera*				m_titleCamera = nullptr;							//カメラ。
	Ground*						m_ground = nullptr;									//地面。
	Sky*						m_sky = nullptr;									//空。
	float						m_skyScale = 20000.0f;								//空の大きさ。
	CVector3					m_position = CVector3(-1535.0f, 0.0f, 2360.0f);		//兵士の座標。
	CQuaternion					m_rotation = CQuaternion::Identity();				//兵士の回転。
};

