#pragma once
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update()override;
	void Render()override;
private:
	ID3D11Device*							m_device = nullptr;
	ID3D11DeviceContext*					m_deviceContext = nullptr;
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch = nullptr;
	ID3D11ShaderResourceView*				m_shaderResourceView = nullptr;
};

