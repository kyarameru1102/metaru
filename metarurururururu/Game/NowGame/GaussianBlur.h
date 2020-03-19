#pragma once
#include "RenderTarget.h"
#include "graphics/Shader.h"

/// <summary>
/// �K�E�V�A���u���[��������N���X�B
/// </summary>
class GaussianBlur
{
public:
	void Init(ID3D11ShaderResourceView* srcTextureSRV, float blurIntensity);
	void Update();
	void Execute(Shader& vs, ID3D11Buffer* vertexBuffer);
	/// <summary>
	/// �t���X�N���[���`�悷��֐��B
	/// </summary>
	/// <param name="vsShader">Shader	���_�V�F�[�_�[�B</param>
	/// <param name="psShader">Shader	�s�N�Z���V�F�[�_�[�B</param>
	void DrowFullScreen(Shader& vsShader, Shader& psShader, Shader& vs, ID3D11Buffer* vertexBuffer);
	ID3D11ShaderResourceView* GetResultTextureSRV()
	{
		return m_downSamplingRT[1].GetRenderTargetSRV();
	}
private:
	static const int NUM_WEIGHTS = 8;	//�J�E�X�t�B���^�̏d�݂̐��B
	/// <summary>
	/// �u���[�p�̃p�����[�^�o�b�t�@�B
	/// </summary>
	struct SBlurParam {
		float weights[NUM_WEIGHTS];
	};
	ID3D11ShaderResourceView*		m_srcTextureSRV = nullptr;			//�\�[�X�e�N�X�`����SRV
	float							m_blurIntensity = 25.0f;			//�u���[�̋����B
	unsigned int					m_srcTextureWidth = 0;				//�\�[�X�e�N�X�`���̕��B
	unsigned int					m_srcTextureHeight = 0;				//�\�[�X�e�N�X�`���̍����B
	RenderTarget					m_downSamplingRT[2];				//�u���[�������邽�߂̃_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g�B
	Shader							m_vsXBlur;							//X�u���[�p�̒��_�V�F�[�_�[�B
	Shader							m_vsYBlur;							//Y�u���[�p�̒��_�V�F�[�_�[�B
	Shader							m_psBlur;							//�u���[�p�̃s�N�Z���V�F�[�_�[�B
	bool							m_isInited = false;					//�������ς݃t���O�B
	SBlurParam						m_blurParam;						//�u���[�p�̃p�����[�^�B
	ID3D11Buffer*					m_constantBuffer = nullptr;			//GPUBuffe��̒萔�o�b�t�@�B
};

