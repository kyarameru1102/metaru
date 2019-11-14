#pragma once
//ディレクションライト作成クラス。
struct SDirectionLight{
	CVector4 direction;		//ライトの方向。
	CVector4 Color;			//ライトの色。
};
//定数バッファ。
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
	//初期化関数。
	void Init();
	//ライトの向きを設定する関数。
	void SetDirection(const CVector3& direction)
	{
		m_dirLight.direction = direction;
	}
	//ライトの色を設定する関数。
	void SetColor(const CVector4& color)
	{
		m_dirLight.Color = color;
	}
private:
	ID3D11Buffer*		m_cb = nullptr;						//定数バッファ。
	ID3D11Buffer*		m_lightCb = nullptr;				//ライト用の定数バッファ。
	SDirectionLight		m_dirLight;							//ディレクションライト。
};

