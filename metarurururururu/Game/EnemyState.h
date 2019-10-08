#pragma once
/// <summary>
/// エネミーのステートパターン。
/// </summary>
class IEnemyState	{
public:
	virtual void Enter() = 0;				//切り替わった。
	virtual void Leave() = 0;				//終わった。
	virtual void Update() = 0;				//継続中。
};
