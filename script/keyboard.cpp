//=============================================================================
//
// �L�[�{�[�h���� [keyboard.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "keyboard.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECTINPUT8 g_pKeyboard = NULL;			// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

BYTE g_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏�񃏁[�N
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		// �g���K�[
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		// �����[�X
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		// ���s�[�g

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pKeyboard,NULL)))
	{
		return E_FAIL;
	}
	
	// ���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(g_pKeyboard->CreateDevice(GUID_SysKeyboard,&g_pDevKeyboard,NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (g_pKeyboard != NULL)
	{
		g_pKeyboard->Release();
		g_pKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �g���K�[����
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			// �����[�X����
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey])&g_aKeyState[nCntKey];

			// ���s�[�g����
			//

			g_aKeyState[nCntKey] = aKeyState[nCntKey];// �L�[�v���X���ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================================================================
// �L�[�{�[�h���������Ƃ��̏����Q�v���X
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey]) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�𗣂����Ƃ��̏����Q�����[�X
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return(g_aKeyStateRelease[nKey]) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�𒷉��������Ƃ��̏����Q���s�[�g
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return(g_aKeyStateRepeat[nKey]) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�𒷉��������Ƃ��̏����Q�����O�v���X
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}

