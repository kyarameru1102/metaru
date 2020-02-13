#pragma once
#include "ShadowMap.h"

//3D���f����\������@�\��񋟂���N���X�B
class SkinModelRender : public IGameObject
{
	//IGameObject�Ȃ̂�NewGO�ŃC���X�^���X���쐬����K�v������B
public:
	SkinModelRender();
	void OnDestroy() override;
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <param name="filePath">
	/// ���[�h������cmo�t�@�C���̃t�@�C���p�X�B
	/// </param>
	/// <param name="animationClips">
	/// �A�j���[�V�����N���b�v�̔z��̐擪�A�h���X�B
	/// </param>
	/// <param name="numAnimationClips">
	/// �A�j���[�V�����N���b�v�̐��B
	/// </param>
	/// <param name="fbxUpAxis">
	/// FBX�̏�����B
	/// </param>
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,int
		numAnimationClips = 0, 
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ
	);
	//�A�j���[�V�����̍Đ��B
	void PlayAnimation(int animNo, bool UpperBody, float interpolateTime = 0.0f)
	{
		if (UpperBody) {
			m_animation.Play(animNo, interpolateTime);
			m_animation.SetUpperBody(UpperBody);
		}
		else {
			m_animation2.Play(animNo, interpolateTime);
			m_animation2.SetUpperBody(UpperBody);
		}
	}

	//�A�j���[�V�����̍X�V�t���O��ݒ肷��֐��B
	//false�Ȃ�Đ�����߂�B
	void SetUpdateAnimationFlag(bool flag)
	{
		m_isUpdateAnimation = flag;
	}
	//3D���f����\������|�W�V������ݒ肷��֐��B
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//�����Ă��������ݒ肷��֐��B
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	//�傫����ݒ肷��֐��B
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	//�X�L�����f�����擾�B
	SkinModel& GetSkinModel()
	{
		return m_skinModel;
	}
	//�A�j���[�V�����̏������B
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);
	/// <summary>
	/// ���f���������_�����O���邩�ǂ����̊֐��B
	/// </summary>
	/// <param name="on">true �����_�����O����B
	/// false �����_�����O���Ȃ��B</param>
	void SetRenderOn(bool on)
	{
		m_RenderOn = on;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�ɓo�^���邩�ǂ����̊֐�
	/// </summary>
	/// <param name="reciever">
	/// true	�o�^����B
	/// false	�o�^�����B
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
	AnimationClip*				m_animationClips = nullptr;				//�A�j���[�V�����N���b�v�B
	int							m_numAnimationClips = 0;				//�A�j���[�V�����N���b�v�̐��B
	CVector3 					m_position = CVector3::Zero();			//���W�B
	CQuaternion	 				m_rotation = CQuaternion::Identity();	//��]�B
	CVector3					m_scale = CVector3::One();				//�g�嗦�B
	EnFbxUpAxis					m_enFbxUpAxis = enFbxUpAxisZ;			//FBX�̏�����B
	SkinModel					m_skinModel;							//�X�L�����f���B
	Animation					m_animation;							//�A�j���[�V�����B
	Animation					m_animation2;							//�A�j���[�V�����B
	bool						m_isUpdateAnimation = true;				//�A�j���[�V������A���Đ����邩�H
	bool						m_RenderOn = true;						//�`�悳��邩�ǂ����B
	bool                        m_renderOK = false;						//Update�̌�Ƀ����_�[���Ă΂��悤�ɂ���t���O�B
	bool						m_shadowRecieverOn = true;				//�e��\������鑤�ɂȂ邩�ǂ����B
	bool						m_shadowCasterOn = true;				//�e�𗎂Ƃ����ǂ����B
	bool						m_UpperBody = true;						//�㔼�g�̃A�j���[�V�����𗬂��Ȃ�true�B
};

