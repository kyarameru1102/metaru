#include "stdafx.h"
#include "system/system.h"
#include "Font.h"
#include "Title.h"
#include "Clear.h"
#include "sound/SoundEngine.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	CSoundEngine m_soundEngine;
	m_soundEngine.Init();

	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 50.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(15000.0f);
	
	StopWatch stopWatch;

	//タイトルを生成する。
	NewGO<Title>(0,"Title");

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
		//ゲームオブジェクトマネージャーの更新。
		gameObjectManager().Update();
		//サウンドエンジン更新。
		m_soundEngine.Update();

		//描画終了。
		g_graphicsEngine->EndRender();
		stopWatch.Stop();

		GameTime().PushFrameDeltaTime(stopWatch.GetElapsed());
	}
}