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
	/// �R���X�g���N�^�B
	/// </summary>
	Title();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Title();
	/// <summary>
	/// �J�n�֐��B
	/// </summary>
	/// <returns></returns>
	bool Start()override;
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	void Update()override;
	void Render()override;
private:
	enum EnAnimationClip {
		enAnimationClip_idle,								//�ҋ@�B
		enAnimationClip_Num,								//�A�j���[�V�����N���b�v�̐��B
	};
	AnimationClip				m_animClips[enAnimationClip_Num];					//�A�j���[�V�����N���b�v�B
	ID3D11ShaderResourceView*	m_specMapSRV = nullptr;								//�X�y�L�����}�b�v��SRV
	SkinModelRender*			m_skinModelRender = nullptr;						//�X�L�����f�������_�[�B
	Sprite						m_sprite;											//�X�v���C�g�B
	bool						m_on = false;										//A�{�^���������ꂽ���̃t���O�B
	int							m_count = 0;										//�J�E���^�[�B
	CSoundSource				m_se;												//SE�B
	TitleCamera*				m_titleCamera = nullptr;							//�J�����B
	Ground*						m_ground = nullptr;									//�n�ʁB
	Sky*						m_sky = nullptr;									//��B
	float						m_skyScale = 20000.0f;								//��̑傫���B
	CVector3					m_position = CVector3(-1535.0f, 0.0f, 2360.0f);		//���m�̍��W�B
	CQuaternion					m_rotation = CQuaternion::Identity();				//���m�̉�]�B
};

