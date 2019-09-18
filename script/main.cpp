//=============================================================================
//
// ���C������ [main.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "camera.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "keyboard.h"
#include "light.h"
#include "player.h"
#include "ranking.h"
#include "result.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CLASS_NAME		"WindowClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"RideParty"	// �E�C���h�E�̃L���v�V������

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

#ifdef _DEBUG
void DrawFPS(void);
void DrawInfo(void);
#endif

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^(�`��ɕK�v)

MODE				g_mode = MODE_TITLE;	// ���݂̃��[�h
MODE				g_modeold = MODE_MAX;	// �O�̃��[�h

#ifdef _DEBUG
LPD3DXFONT			g_pFont = NULL;			// �t�H���g�ւ̃|�C���^
int					g_nCountFPS = 0;		// FPS�J�E���^
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect =	{0,	0, SCREEN_WIDTH, SCREEN_HEIGHT};

	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwExecLastTime;		// �Ō�ɏ�����������	

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�C���h�E���쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)TRUE/FALSE
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ 0 ��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W�������烋�[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���

			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;		// ��������������ۑ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;			// �J�E���g�����Z
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �N���C�A���g�̈�𒼂��ɍX�V����

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
									D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
									hWnd,								// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,// �f�o�C�X�쐬����̑g�ݍ��킹
									&d3dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
									&g_pD3DDevice)))					// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}
	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����
	
	//// �t�H���g��ݒ�
	//D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
	//	OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &g_pFont);
#ifdef _DEBUG
	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &g_pFont);
#endif

	// �L�[�{�[�h�̏���������
	InitKeyboard(hInstance, hWnd);

	// �W���C�p�b�h����������
	InitJoypad(hInstance, hWnd);

	// �T�E���h�̏���������
	InitSound(hWnd);

	// ���C�g�̏���������
	InitLight();

	// �t�F�[�h�̏�����
	InitFade(g_mode, hWnd);

	// �^�C�g����ʂ̏�����
	InitTitle();

	InitResult();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �^�C�g����ʏI������
	UninitTitle();

	// �`���[�g���A����ʏI������
	UninitTutorial();

	// �Q�[����ʏI������
	UninitGame();

	// ���U���g��ʏI������
	UninitResult();

	// �����L���O��ʏI��
	UninitRanking();

	// �J�����I��
	UninitCamera();

	// �L�[�{�[�h�I��
	UninitKeyboard();

	// �W���C�p�b�h�I��
	UninitJoypad();

	// �t�F�[�h�I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

#ifdef _DEBUG
	if (g_pFont != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif
	
	// Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	// �W���C�p�b�h�X�V����
	UpdateJoypad();

	// ���C�g�̍X�V����
	UpdateLight();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_RESULT:
		UpdateResult();
		break;
	}
	// �t�F�[�h�X�V����
	UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;
		}
		// �t�F�[�h�̕`��
		DrawFade();

#ifdef _DEBUG
		// ���\��
		DrawInfo();

		// FPS�\��
		DrawFPS();
#endif

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode, HWND hWnd)
{
	g_modeold = g_mode;

	// �Q�[����Ԃ̎擾
	GAMESTATE *GameState = GetGameState();

	// �����~�߂�
	if (g_mode != MODE_TITLE&&*GetModeOld() != MODE_GAME)
	{
		StopSound();
	}

	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}
	g_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		PlaySound(SOUND_LABEL_BGM000);// BGM�̐ݒ�
		break;

	case MODE_TUTORIAL:
		InitTutorial(hWnd);
		StopSound();
		PlaySound(SOUND_LABEL_BGM000);
		break;

	case MODE_GAME:
		InitGame(hWnd);

		if (*GetModeOld() != MODE_GAME)
		{
			StopSound();
			PlaySound(SOUND_LABEL_BGM001);
		}

		break;

	case MODE_RESULT:
		InitResult();
		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE *GetMode(void)
{
	return &g_mode;
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE *GetModeOld(void)
{
	return &g_modeold;
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// FPS�\������
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
}

//=============================================================================
// ���\������
//=============================================================================
void DrawInfo(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];
	CAMERA *pCamera = GetCamera(0);				// �J����1���̎擾
	D3DXVECTOR3 *pPlayerPos = GetPlayerPos(0);	// �v���C���[1�̏ꏊ���̎擾
	D3DXVECTOR3 *pPlayerRot = GetPlayerRot(0);	// �v���C���[1�̌������̎擾
	int nLength;

	sprintf(&aStr[0], "\n�J�����̎��_    :(%3.2f , %3.2f , %3.2f)\n�J�����̒����_  :(%3.2f , %3.2f , %3.2f)\n",
		pCamera->posV.x, pCamera->posV.y, pCamera->posV.z, pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

	nLength = (int)strlen(&aStr[0]);

	sprintf(&aStr[nLength], "�J�����̉�]���:(%3.2f , %3.2f , %3.2f)\n\n���f���̈ʒu    :(%3.2f , %3.2f , %3.2f)\n",
		pCamera->rot.x, pCamera->rot.y, pCamera->rot.z, pPlayerPos->x, pPlayerPos->y, pPlayerPos->z);

	nLength = (int)strlen(&aStr[0]);

	sprintf(&aStr[nLength], "���f���̉�]���:(%3.2f , %3.2f , %3.2f)\n\n",
		pPlayerRot->x, pPlayerRot->y, pPlayerRot->z);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
}
#endif
