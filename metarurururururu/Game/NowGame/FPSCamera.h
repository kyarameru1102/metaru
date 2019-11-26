#pragma once
/// <summary>
/// ��l�̎��_�̃J�����B
/// </summary>
class Player;
class FPSCamera : public IGameObject
{
public:
	FPSCamera();
	~FPSCamera();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void Render() override;
	/// <summary>
	/// �J�����̌�����ݒ肷��B
	/// </summary>
	/// <param name="direction">
	/// �����B
	/// </param>
	void Setdirection(CVector3 direction)
	{
		m_direction = direction;
	}
	/// <summary>
	/// �J�����̌����Ă��������Ԃ��B
	/// </summary>
	/// <returns>
	/// �����B
	/// </returns>
	CVector3 Getdirection() const
	{
		return m_direction;
	}
private:
	CVector3	m_position = CVector3::Zero();
	Player*		m_player = nullptr;
	CVector3	m_direction;					//�����B
};

