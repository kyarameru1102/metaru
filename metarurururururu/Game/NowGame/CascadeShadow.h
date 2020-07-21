#pragma once
#include "RenderTarget.h"

class CascadeShadow
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	CascadeShadow();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~CascadeShadow();
	static inline CascadeShadow& GetCascadeShadowMap()
	{
		static CascadeShadow shadowmap;
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
		return m_shadowMapRT[0].GetRenderTargetSRV();
	}
	CVector3 GetLightPos()
	{
		return m_lightCameraPosition;
	}
	void Update();
public:
	static const int	m_numCascade = 3;			//�����_�����O�^�[�Q�b�g�̐��B
private:
	CVector3 m_lightCameraPosition = CVector3::Zero();	//���C�g�J�����̎��_�B
	CVector3 m_lightCameraTarget = CVector3::Zero();	//���C�g�J�����̒����_�B
	CVector3 m_lightDirection = CVector3::One();		//���C�g�̕����B
	CMatrix m_lightViewMatrix = CMatrix::Identity();	//���C�g�r���[�s��B
	CMatrix  m_lightProjMatrix;
	RenderTarget m_shadowMapRT[m_numCascade];			//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g�B
	std::vector<SkinModel*> m_shadowCasters;			//�V���h�E�L���X�^�[�̔z��B
};

