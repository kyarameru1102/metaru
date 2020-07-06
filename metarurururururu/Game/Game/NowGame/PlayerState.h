#pragma once
/// <summary>
/// プレイヤーのステートパターン
/// </summary>
class IPlayerState	{
public:
	virtual void Enter() = 0;				//切り替わった。
	virtual void Leave() = 0;				//終わった。
	virtual void Update() = 0;				//継続中。
	virtual bool IsInvincible() = 0;		//無敵かどうか。
	virtual bool IsPossibleMove() = 0;		//動けるかどうか。
	virtual bool IsPossibleGunShoot() = 0;	//銃が撃てるかどうか。
	virtual bool IsPossibleHoldGun() = 0;	//銃を構えれるかどうか。
	virtual bool IsRotateByMove() = 0;		//キャラクターが移動により回転するか。
};