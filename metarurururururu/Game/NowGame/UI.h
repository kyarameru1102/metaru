#pragma once
class UI : public IGameObject
{
public:
	UI();
	~UI();
	bool Start() override;
	void Update() override;
	void Render()override;
private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
};

