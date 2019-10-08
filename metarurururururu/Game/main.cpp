#include "stdafx.h"
#include "system/system.h"
#include "Font.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 50.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	NewGO<Game>(0, "Game");
	StopWatch stopWatch;

	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		stopWatch.Start();
		//�`��J�n�B
		g_graphicsEngine->BegineRender();
		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//�����G���W���̍X�V�B
		g_physics.Update();
		
		gameObjectManager().Update();

		//�`��I���B
		g_graphicsEngine->EndRender();
		stopWatch.Stop();

		GameTime().PushFrameDeltaTime(stopWatch.GetElapsed());
	}
}