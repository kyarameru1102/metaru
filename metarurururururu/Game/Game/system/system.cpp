#include "stdafx.h"
#include "system.h"

HWND				g_hWnd = NULL;				//�E�B���h�E�n���h���B
GraphicsEngine*		g_graphicsEngine = NULL;	//�O���t�B�b�N�X�G���W���B

///////////////////////////////////////////////////////////////////
// DirectX�̏I�������B
///////////////////////////////////////////////////////////////////
void ReleaseDirectX()
{
	
}
///////////////////////////////////////////////////////////////////
//���b�Z�[�W�v���V�[�W���B
//hWnd�����b�Z�[�W�𑗂��Ă����E�B���h�E�̃n���h���B
//msg�����b�Z�[�W�̎�ށB
//wParam��lParam�͈����B���͋C�ɂ��Ȃ��Ă悢�B
///////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
	switch (msg)
	{
	case WM_DESTROY:
		ReleaseDirectX();
		PostQuitMessage(0);
		break;	
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////
// �E�B���h�E�̏������B
///////////////////////////////////////////////////////////////////
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//�E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		//�\���̂̃T�C�Y�B
		CS_CLASSDC,				//�E�B���h�E�̃X�^�C���B
								//�����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͕s�v�Ȃ̂�CS_CLASSDC�ł悢�B
		MsgProc,				//���b�Z�[�W�v���V�[�W��(��q)
		0,						//0�ł����B
		0,						//0�ł����B
		GetModuleHandle(NULL),	//���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
								//�����C�ɂ��Ȃ��Ă悢�B
		NULL,					//�A�C�R���̃n���h���B�A�C�R����ς������ꍇ������ύX����B�Ƃ肠��������ł����B
		NULL,					//�}�E�X�J�[�\���̃n���h���BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//�E�B���h�E�̔w�i�F�BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//���j���[���BNULL�ł����B
		appName,				//�E�B���h�E�N���X�ɕt���閼�O�B
		NULL					//NULL�ł����B
	};
	//�E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);

	// �E�B���h�E�̍쐬�B
	g_hWnd = CreateWindow(
		appName,		//�g�p����E�B���h�E�N���X�̖��O�B
								//��قǍ쐬�����E�B���h�E�N���X�Ɠ������O�ɂ���B
		appName,		//�E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢�B
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł����A
		0,						//�E�B���h�E�̏���X���W�B
		0,						//�E�B���h�E�̏���Y���W�B
		(UINT)FRAME_BUFFER_W,	//�E�B���h�E�̕��B
		(UINT)FRAME_BUFFER_H,	//�E�B���h�E�̍����B
		NULL,					//�e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł����B
		NULL,					//���j���[�B����NULL�ł����B
		hInstance,				//�A�v���P�[�V�����̃C���X�^���X�B
		NULL
	);

	ShowWindow(g_hWnd, nCmdShow);

}


//�Q�[���̏������B
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//�E�B���h�E���������B
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	//DirectX�̏������B
	g_graphicsEngine = new GraphicsEngine();
	g_graphicsEngine->Init(g_hWnd);

	//�Q�[���p�b�h�̏������B
	//�ő�S�̃R���g���[���[��ڑ��ł���悤�ɂ��܂��傤�B
	g_pad[0].Init(0);
	g_pad[1].Init(1);
	g_pad[2].Init(2);
	g_pad[3].Init(3);

	g_physics.Init();

	//2D�J�����̏������B
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetNear(0.1f);
	g_camera2D.SetFar(1000.0f);
	g_camera2D.Update();

	gameObjectManager().Init(255);
}
//�E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���Ԃ��Ă�����A�Q�[���I���B
bool DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		//�E�B���h�E����̃��b�Z�[�W���󂯎��B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//�E�B���h�E���b�Z�[�W����ɂȂ����B
			break;
		}
	}
	return msg.message != WM_QUIT;
}