#pragma once
#include "graphics/Shader.h"

/// <summary>
/// �X�v���C�g�N���X�B
/// </summary>
class Sprite
{
public:

	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Sprite();

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Sprite();

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="texFilePath">�e�N�X�`���̃t�@�C���p�X�B</param>
	/// <param name="w">�摜�̕��B</param>
	/// <param name="h">�摜�̍����B</param>
	void Init(const wchar_t* texFilePath, float w, float h);

	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="trans">���s�ړ��B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="scale">�g�嗦�B</param>
	/// <param name="pivot">
	/// �s�{�b�g�B
	/// 0.5, 0.5�ŉ摜�̒��S����_�B
	///	0.0, 0.0�ŉ摜�̍����B
	///	1.0, 1.0�ŉ摜�̉E��B
	/// </param>
	void Update(const CVector3& trans,
				const CQuaternion& rot,
				const CVector3& scale,
				CVector2 pivot = { 0.5f,0.5f });

	/// <summary>
	/// �`��B
	/// </summary>
	void Draw();
	/// <summary>
	/// �A���t�@�l��ݒ肷��֐��B
	/// </summary>
	/// <param name="delta">delta�ɓn�����l�ɉ����ăA���t�@�l��ύX����B</param>
	void DeltaAlpha(float delta)
	{
		m_alpha += delta;
		//���l�̋��E�`�F�b�N�B
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
		else if (m_alpha < 0.0f) {
			m_alpha = 0.0f;
		}
	}
	/// <summary>
	/// �A���t�@�l��ݒ肷��֐��B
	/// </summary>
	/// <param name="alpha">alpha�ɓn�����l�����̂܂ܓK�p����B</param>
	void SetAlpha(float alpha)
	{
		m_alpha = 1.0f;
		m_alpha += alpha;
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
		else if (m_alpha < 0.0f) {
			m_alpha = 0.0f;
		}
	}
	/// <summary>
	/// ���݂̃A���t�@�l��Ԃ��֐��B
	/// </summary>
	/// <returns>m_alpha�@�A���t�@�l�B</returns>
	float GetAlpha() const
	{
		return m_alpha;
	}
private:

	/// <summary>
	/// �萔�o�b�t�@�̏������B
	/// </summary>
	void InitConstantBuffer();

	/// <summary>
	/// ���_�o�b�t�@�̏������B
	/// </summary>
	/// <param name="w">�摜�̕�</param>
	/// <param name="h">�摜�̍���</param>
	void InitVertexBuffer(float w, float h);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̏������B
	/// </summary>
	void InitIndexBuffer();

	/// <summary>
	/// �T���v���X�e�[�g�̏������B
	/// </summary>
	void InitSamplerState();
	void CreateDepthStencilState();
	void SetRenderState();
	void GetRenderState();
private:
	struct ConstantBuffer {
		CMatrix WVP;		//���[���h�r���[�v���W�F�N�V�����s��B
		float alpha;		//���l�B
	};
	ID3D11Buffer*				m_vertexBuffer = NULL;					//���_�o�b�t�@�B
	ID3D11Buffer*				m_indexBuffer = NULL;					//�C���f�b�N�X�o�b�t�@�B
	Shader						m_vs;									//���_�V�F�[�_�[�B
	Shader						m_ps;									//�s�N�Z���V�F�[�_�[�B
	ID3D11ShaderResourceView*	m_texture = NULL;						//�e�N�X�`���B
	ID3D11SamplerState*			m_samplerState = NULL;					//�T���v���X�e�[�g�B
	CVector3					m_position = CVector3::Zero();			//���W�B
	CQuaternion					m_rotation = CQuaternion::Identity();	//��]
	CVector3					m_scale = CVector3::One();				//�g�嗦�B
	CMatrix						m_world = CMatrix::Identity();			//���[���h�s��B
	CVector2					m_size = CVector2::Zero();				//�摜�̃T�C�Y�B
	ID3D11Buffer*				m_cb = nullptr;							//�萔�o�b�t�@�B
	float						m_alpha = 1.0f;							//�X�v���C�g�̃��l�B
	ID3D11DeviceContext*		m_DC = nullptr;							//�f�o�C�X�R���e�L�X�g�B
	ID3D11DepthStencilState*	m_depthState = nullptr;					//���݂̃f�v�X�X�e���V���X�e�[�g�B
	ID3D11RasterizerState*		m_rasterrizerState = nullptr;			//���݂̃��X�^���C�U�X�e�[�g�B
	ID3D11BlendState*			m_blendState = nullptr;					//���݂̃u�����h�X�e�[�g�B
};

