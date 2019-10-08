#pragma once
//�f�B���N�V�������C�g�쐬�N���X�B
struct SDirectionLight{
	CVector4 direction;		//���C�g�̕����B
	CVector4 Color;			//���C�g�̐F�B
};
//�萔�o�b�t�@�B
struct SVSConstantBuffer {
	CMatrix mWorld;
	CMatrix mView;
	CMatrix mProj;
};
class DirectionLight : public IGameObject
{
	
public:
	DirectionLight();
	~DirectionLight();
	bool Start() override;
	void Update() override;
	void Render() override;
	//�������֐��B
	void Init();
	//���C�g�̌�����ݒ肷��֐��B
	void SetDirection(const CVector3& direction)
	{
		m_dirLight.direction = direction;
	}
	//���C�g�̐F��ݒ肷��֐��B
	void SetColor(const CVector4& color)
	{
		m_dirLight.Color = color;
	}
private:
	ID3D11Buffer*		m_cb = nullptr;						//�萔�o�b�t�@�B
	ID3D11Buffer*		m_lightCb = nullptr;				//���C�g�p�̒萔�o�b�t�@�B
	SDirectionLight		m_dirLight;							//�f�B���N�V�������C�g�B
};

