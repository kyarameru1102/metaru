#include "stdafx.h"
#include "system/system.h"
#include "Font.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 50.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	NewGO<Game>(0, "Game");
	StopWatch stopWatch;

	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		stopWatch.Start();
		//描画開始。
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//物理エンジンの更新。
		g_physics.Update();
		
		gameObjectManager().Update();

		//描画終了。
		g_graphicsEngine->EndRender();
		stopWatch.Stop();

		GameTime().PushFrameDeltaTime(stopWatch.GetElapsed());
	}
}