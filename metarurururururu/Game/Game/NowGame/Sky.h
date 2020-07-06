#pragma once
//��B
class Sky : public IGameObject
{
public:
	Sky();
	~Sky();
	bool Start() override;
	void Update() override;
	void SetScale(float scale)
	{
		m_scale = { scale, scale, scale };
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
private:
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B
	ID3D11ShaderResourceView* m_skyCube = nullptr;		//�X�J�C�L���[�u�}�b�v��SRV
	Shader m_psSkyShader;								//��p�̃V�F�[�_�[�B
};

