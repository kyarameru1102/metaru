#pragma once
#include "RenderTarget.h"

class ShadowMap
{
public:
	//�R���X�g���N�^�B
	ShadowMap();
	//�f�X�g���N�^�B
	~ShadowMap();
	static inline ShadowMap& GetShadowMap()
	{
		static ShadowMap shadowmap;
		return shadowmap;
	};
	/// <summary>
	/// ���C�g�r���[�s����擾�B
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightViewMatrix() const
	{
		return m_lightViewMatrix;
	}
	/// <summary>
	/// ���C�g�v���W�F�N�V�����s����擾�B
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}
	/// <summary>
	/// �X�V(���C�g�J�����̒����_���w�肷��o�[�W�����B
	/// </summary>
	/// UpdateFromLightTarget��UpdateFromLightDir�̂ǂ��炩�𖈃t���[���ĂԁB
	/// <param name="lightPos">���C�g�J�����̎��_�̍��W�B</param>
	/// <param name="lightTarget">���C�g�J�����̒����_�̍��W�B</param>
	void UpdateFromLightTarget(CVector3 lightPos, CVector3 lightTarget);
	/// <summary>
	/// �X�V(���C�g�J�����̌������w�肷��o�[�W�����B
	/// </summary>
	/// ���t���[���ĂԁB
	/// <param name="lightPos">���C�g�J�����̎��_�̍��W</param>
	/// <param name="lightDir">���C�g�̕���</param>
	void UpdateFromLightDir(CVector3 lightPos, CVector3 lightDir);
	/// <summary>
	/// �V���h�E�}�b�v�ɕ`�悳���V���h�E�L���X�^�[��
	/// RegistShadowCaster�֐����g�p���ēo�^���ꂽ�L���X�^�[�݂̂ł��B
	/// ���̊֐����Ăяo���ƁA�V���h�E�L���X�^�[�̃��X�g�̓N���A����܂��B
	/// </summary>
	void RenderToShadowMap();
	/// <summary>
	/// �V���h�E�L���X�^�[��o�^�B
	/// </summary>
	/// ��ɉe�𐶐��������ꍇ�͖��t���[���ĂԁB
	/// <param name="shadowCaster">�V���h�E�L���X�^�[�B</param>
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	/// <summary>
	/// �V���h�E�}�b�v��SRV���擾�B
	/// </summary>
	/// <returns>�V���h�E�}�b�v��SRV</returns>
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//���C�g�J�����̎��_�B
	CVector3 m_lightCameraTarget = CVector3::Zero();	//���C�g�J�����̒����_�B
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//���C�g�r���[�s��B
	CMatrix m_lightProjMatrix = CMatrix::Identity();	//���C�g�v���W�F�N�V�����s��B
	RenderTarget m_shadowMapRT;						//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g�B
	std::vector< SkinModel*> m_shadowCasters;	//�V���h�E�L���X�^�[�̔z��B
};

