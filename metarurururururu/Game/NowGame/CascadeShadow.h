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
	CMatrix GetLightViewMatrix(int i) const
	{
		return m_lightViewMatrix[i];
	}
	/// <summary>
	/// ���C�g�v���W�F�N�V�����s����擾�B
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix(int i) const
	{
		return m_lightProjMatrix[i];
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
	ID3D11ShaderResourceView* GetShadowMapSRV(int i)
	{
		return m_shadowMapRT[i].GetRenderTargetSRV();
	}
	
	void Update();
public:
	static const int	m_numCascade = 3;					//�����_�����O�^�[�Q�b�g�̐��B
private:
	/// <summary>
	/// 8���_�p�̍\���́B
	/// </summary>
	struct EightVertices {
		CVector3 vertex[8];
	};
	void MatrixCalculation(float Near,float Far,int i);		//�s��֌W�̌v�Z�B
	void Render(int i);
private:
	EightVertices m_eightVertices[m_numCascade];
	CVector3 m_lightCameraPosition[m_numCascade];			//���C�g�J�����̎��_�B
	CVector3 m_lightCameraTarget[m_numCascade];				//���C�g�J�����̒����_�B
	CVector3 m_lightDirection = CVector3::One();			//���C�g�̕����B
	CMatrix m_lightViewMatrix[m_numCascade];				//���C�g�r���[�s��B
	CMatrix  m_lightProjMatrix[m_numCascade];
	RenderTarget m_shadowMapRT[m_numCascade];				//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g�B
	std::vector<SkinModel*> m_shadowCasters;				//�V���h�E�L���X�^�[�̔z��B
	float m_nearDistance[m_numCascade];						//�J�����̋ߕ��ʂ܂ł̋����B
	float m_farDistance[m_numCascade];						//�J�����̉����ʂ܂ł̋����B
};

