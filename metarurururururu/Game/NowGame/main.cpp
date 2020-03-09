#include "stdafx.h"
#include "system/system.h"
#include "Font.h"
#include "Title.h"
#include "Clear.h"
#include "sound/SoundEngine.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	CSoundEngine m_soundEngine;
	m_soundEngine.Init();

	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 50.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(15000.0f);
	
	StopWatch stopWatch;

	//�^�C�g���𐶐�����B
	NewGO<Title>(0,"Title");

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
		//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�B
		gameObjectManager().Update();
		//�T�E���h�G���W���X�V�B
		m_soundEngine.Update();

		//�`��I���B
		g_graphicsEngine->EndRender();
		stopWatch.Stop();

		GameTime().PushFrameDeltaTime(stopWatch.GetElapsed());
	}
}