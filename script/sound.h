//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �T�E���h�t�@�C��
//=============================================================================
typedef enum
{
	// BGM
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_BGM002,				// BGM2
	SOUND_LABEL_BGM003,				// BGM3
	SOUND_LABEL_BGM004,				// BGM4

	// SE
	SOUND_LABEL_SE_DECISION,		// ���艹
	SOUND_LABEL_SE_POWER_UP,		// �p���[�A�b�v��
	SOUND_LABEL_SE_SAIROLL,			// �T�C�R���]����
	SOUND_LABEL_SE_CAR,				// �ԃG���W��
	SOUND_LABEL_SE_BANANA,			// �o�i�i���
	SOUND_LABEL_SE_DSAI,			// �_�u���T�C�R��
	SOUND_LABEL_SE_MINIQ,			// ������
	SOUND_LABEL_SE_MINIQB,			// �������~�X
	SOUND_LABEL_SE_MINIC,			// �`�L���X�^�[�g

	SOUND_LABEL_GAMECLEAR,			// CLEAR��

	SOUND_LABEL_SE_POUSE_0,			// �|�[�Y�I����
	SOUND_LABEL_SE_POUSE_1,			// �|�[�Y���艹

	SOUND_LABEL_MAX,				// ���̍ő吔
} SOUND_LABEL;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
