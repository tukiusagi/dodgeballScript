//=============================================================================
//
// �W���C�p�b�h���� [joypad.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_JOYPAD_MAX		(4)		// �p�b�h�ő吔

//=============================================================================
// �L�[�̎��
//=============================================================================
typedef enum
{
	JOYPADKEY_X = 0,			// [0] �w�{�^��
	JOYPADKEY_Y,				// [1] �x�{�^��
	JOYPADKEY_A,				// [2] �`�{�^��
	JOYPADKEY_B,				// [3] �a�{�^��
	JOYPADKEY_LEFT_SHOULDER,	// [4] ����O�{�^��
	JOYPADKEY_RIGHT_SHOULDER,	// [5] �E��O�{�^��
	JOYPADKEY_LEFT_TRIGGER,		// [6] ���g���K�[
	JOYPADKEY_RIGHT_TRIGGER,	// [7] �E�g���K�[
	JOYPADKEY_LEFT_THUMB,		// [8] ���X�e�B�b�N
	JOYPADKEY_RIGHT_THUMB,		// [9] �E�X�e�B�b�N
	JOYPADKEY_BACK,				// [10] �o�b�N�{�^��
	JOYPADKEY_START,			// [11] �X�^�[�g�{�^��
	JOYPADKEY_LEFT,				// [12] �����L�[[��]
	JOYPADKEY_RIGHT,			// [13] �����L�[[�E]
	JOYPADKEY_UP,				// [14] �����L�[[��]
	JOYPADKEY_DOWN,				// [15] �����L�[[��]
	JOYPADKEY_MAX				// [16] �L�[�̎�ސ�
} JOYPADKEY;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	bool UpThumb;	// ���X�e�B�b�N��
	bool DownThumb;	// ���X�e�B�b�N��
} JOYPADUSE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
void UninitJoypad(void);
void UpdateJoypad(void);

bool GetJoypadPress(int nIDPad, JOYPADKEY key);
bool GetJoypadTrigger(int nIDPad, JOYPADKEY key);
bool GetJoypadRelease(int nIDPad, JOYPADKEY key);
bool GetJoypadRepeat(int nIDPad, JOYPADKEY key);

int GetJoypadTriggerLeft(int nIDPad);
int GetJoypadTriggerRight(int nIDPad);

void GetJoypadStickLeft(int nIDPad, int *pValueH, int *pValueV);
void GetJoypadStickRight(int nIDPad, int *pValueH, int *pValueV);

static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);
static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
void SetKeyStateJoypad(int nIDPad);

void SetNumPad(int nNum);	// �v���C���[���̐ݒ�
int GetNumPad(void);		// �v���C���[���̎擾

#endif