#pragma once
#include "RenderTarget.h"
#include "graphics/Shader.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	void Update();
	void Drow(RenderTarget renderTarget);
private:
	static const int NUM_WEIGHTS = 8;	//�J�E�X�t�B���^�̏d�݂̐��B
	/// <summary>
	/// �u���[�p�̃p�����[�^�o�b�t�@�B
	/// </summary>
	struct SBlurParam {
		CVector4 offset;
		float weights[NUM_WEIGHTS];
	};
	ID3D11Buffer*			m_blurParamCB = nullptr;			//�u���[�p�����[�^�p�̒萔�o�b�t�@�B
	SBlurParam				m_blurParam;						//�u���[�p�̃p�����[�^�B
	float					m_blurDispersion = 50.0f;			//�u���[�̕��U��B�傫������قǋ������U����B
	RenderTarget			m_luminanceRT;						//�P�x�𒊏o���郌���_�����O�^�[�Q�b�g
	RenderTarget			m_downSamplingRT[2];				//�u���[�������邽�߂̃_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g�B
	ID3D11SamplerState*		m_samplerState = nullptr;			//�T���v���X�e�[�g�B
	ID3D11BlendState*		m_disableBlendState = nullptr;		//�A���t�@�u�����f�B���O�𖳌��ɂ���u�����f�B���O�X�e�[�g�B
	ID3D11BlendState*		m_finalBlendState = nullptr;		//�ŏI�����p�̃u�����f�B���O�X�e�[�g�B
	Shader					m_vs;								//�������Ȃ����_�V�F�[�_�[�B
	Shader					m_psLuminance;						//�P�x���o�p�̃s�N�Z���V�F�[�_�[�B
	Shader					m_vsXBlur;							//X�u���[�p�̒��_�V�F�[�_�[�B
	Shader					m_vsYBlur;							//Y�u���[�p�̒��_�V�F�[�_�[�B
	Shader					m_psBlur;							//�u���[�p�̃s�N�Z���V�F�[�_�[�B
	Shader					m_psFinal;							//�ŏI�����p�̃s�N�Z���V�F�[�_�[�B
	//�t���X�N���[���`��p�̃����o�ϐ��B
	ID3D11Buffer*			m_vertexBuffer = nullptr;			//���_�o�b�t�@�B
	
};

