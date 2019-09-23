//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
#include "camera.h"
#include "effect.h"
#include "fade.h"
#include "joypad.h"
#include "keyboard.h"
#include "meshField.h"
#include "meshWall.h"
#include "model.h"
#include "pause.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "title.h"
#include "time.h"
#include "ball.h"
#include "field.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEXTURE		(6)								// �e�N�X�`����
#define SELECT_POS_X	(165)							// �|�[�Y�̘g��X���W
#define ARROW_POS_X		(25)							// ����X���W
#define DICE_POS_Y		(120)							// �T�C�R����Y���W
#define ITEM_POS_Y		(180)							// �A�C�e����Y���W
#define MAP_POS_Y		(240)							// �}�b�v��Y���W
#define CONTINUE_POS_Y	(300)							// �R���e�j���[��Y���W
#define RETEY_POS_Y		(400)							// ���g���C��Y���W
#define QUIT_POS_Y		(500)							// �I����Y���W
#define SELECT_WIDTH	(330)							// �Z���N�g�̕�
#define SELECT_HEIGHT	(50)							// �Z���N�g�̍���
#define ARROW_WIDTH		(46)							// ���̕�
#define ARROW_HEIGHT	(46)							// ���̍���
#define ITEM1_POS_X		(500)							// �A�C�e����X���W
#define ITEM1_POS_Y		(300)							// �A�C�e����Y���W
#define ITEM1_WIDTH		(100)							// �A�C�e���̕�
#define ITEM1_HEIGHT	(100)							// �A�C�e���̍���
#define ITEM2_POS_X		(800)							// �A�C�e����X���W
#define ITEM2_POS_Y		(300)							// �A�C�e����Y���W
#define ITEM2_WIDTH		(100)							// �A�C�e���̕�
#define ITEM2_HEIGHT	(100)							// �A�C�e���̍���
#define MAX_SELECT		(4)								// �I���e�N�X�`����
#define SELECT_MAX		(3)								// �I�����̐�
#define MAX_PALYER		(4)	//�v���C���[��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void SetChangePlayer(void);

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct// �Q�[��
{
	JOYPADUSE				g_PadUse;							// �W���C�p�b�h�̎g�p��Ԃ̏��
	GAMESTATE				state = GAMESTATE_NONE;				// �Q�[�����
	int						nCounterState = 0;					// �Q�[����ԃJ�E���^�[
	int						nNowPlayer;							// ���݂̃v���C���[�ԍ�
} GAME;

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
GAME					g_game;								// �Q�[���̏��

//=============================================================================
// �Q�[����ʂ̏�����
//=============================================================================
void InitGame(HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X�̎擾

	// ������
	g_game.nCounterState = 0;

	if (g_game.state == GAMESTATE_NONE)
	{// �ŏ��̂ݏ���������
		g_game.nNowPlayer = 0;		// 1P����J�n����
	}
	else
	{
		SetChangePlayer();		// ����v���C���[�̕ύX
	}

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏���������
	InitPlayer();

	SetPlayerPos(D3DXVECTOR3(-200.0f, 0.0f, 200.0f), 0);
	SetPlayerPos(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 1);
	SetPlayerPos(D3DXVECTOR3(-200.0f, 0.0f, -200.0f), 2);
	SetPlayerPos(D3DXVECTOR3(200.0f, 0.0f, -200.0f), 3);

	// �J�����̏���������
	InitCamera();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	InitMeshWall();

	// �|�[�Y��ʏ���������
	InitPause();

	// ���f���̏���������
	InitModel();

	//�{�[���̏�����
	InitBall();

	// �G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N��������
	InitParticle();

	// �t�B�[���h�̏�����
	InitField();

	//�J�E���g�_�E���̏�����
	InitTime();

	g_game.state = GAMESTATE_NORMAL;// �X�^�[�g��Ԃɐݒ�
}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitGame()
{
	// �|�[�Y��ʏI������
	UninitPause();

	//�{�[���̏I��
	UninitBall();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	//�J�E���g�_�E���̏I��
	UninitTime();

	// ���f���̏I������
	UninitModel();

	// �e�̏I������
	UninitShadow();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���I��
	UninitParticle();

	// �t�B�[���h�̏I��
	UninitField();
}

//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame()
{
	ARROW *pArrow = GetArrow();									// �|�[�Y�̖����擾
	D3DXVECTOR3 pPlayerPos = *GetPlayerPos(g_game.nNowPlayer);	// �v���C���[�̈ʒu
	int nStickH, nStickV;										// �A�i���O�X�e�B�b�N���̓��͏��

	// �A�i���O�X�e�B�b�N���̎擾
	GetJoypadStickLeft(g_game.nNowPlayer, &nStickH, &nStickV);

	if (g_game.state != GAMESTATE_PAUSE)
	{// �|�[�Y��Ԃ���Ȃ��Ƃ�

		// �J�����̍X�V����
		UpdateCamera();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		//�{�[���̍X�V
		UpdateBall();

		// ���b�V���t�B�[���h�̍X�V����
		UpdateMeshField();

		// ���b�V���E�H�[���̍X�V����
		UpdateMeshWall();

		// ���f���̍X�V����
		UpdateModel();

		// �e�̍X�V����
		UpdateShadow();

		//�J�E���g�_�E���̍X�V
		UpdateTime();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		//�p�[�e�B�N���X�V
		UpdateParticle();

		// �t�B�[���h�̍X�V
		UpdateField();
	}

	int nNumPlayer = 0;	//�����Ă���v���C���[��

	switch (g_game.state)
	{
	case GAMESTATE_NORMAL:// �ʏ���

		g_game.nCounterState++;

		for (int nCnt = 0; nCnt < MAX_PALYER; nCnt++)
		{
			if (GetPlayerUse(nCnt))
			{
				nNumPlayer++;	//�v���C���[���J�E���g
			}
		}
		if (nNumPlayer == 1)
		{	//�Ō�̈�l�ɂȂ�����I��
			SetGameState(GAMESTATE_END);
		}

		if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(g_game.nNowPlayer, JOYPADKEY_START))
		{// �|�[�Y���������ꍇ
			g_game.state = GAMESTATE_PAUSE;
		}

		if (g_game.nCounterState >= 60)
		{
			//if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(g_game.nNowPlayer, JOYPADKEY_A))
			//{
			//	PlaySound(SOUND_LABEL_SE_POUSE_1);// ���ʉ��̐ݒ�
			//	g_game.state = GAMESTATE_END;
			//	g_game.nCounterState = 0;
			//}
		}

		break;

	case GAMESTATE_END:

		if (g_game.nCounterState == 0)
		{
			StopSound();
			PlaySound(SOUND_LABEL_GAMECLEAR);
		}

		g_game.nCounterState++;

		if (GetFade() != FADE_OUT)
		{
			if (g_game.nCounterState >= 300)
			{
				// ��ʃ��[�h�̐ݒ�
				SetFade(MODE_RESULT);
				g_game.state = GAMESTATE_NONE;
			}
		}
		break;

	case GAMESTATE_PAUSE:

		// �|�[�Y�̍X�V����
		UpdatePause();

		if (GetFade() != FADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(g_game.nNowPlayer, JOYPADKEY_START) || GetJoypadTrigger(g_game.nNowPlayer, JOYPADKEY_B))
			{
				PlaySound(SOUND_LABEL_SE_POUSE_1);// ���ʉ��̐ݒ�

				g_game.state = GAMESTATE_NORMAL;
			}

			if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(g_game.nNowPlayer, JOYPADKEY_A))
			{
				PlaySound(SOUND_LABEL_SE_POUSE_1);// ���ʉ��̐ݒ�

				if (pArrow->pos.y == CONTINUE_POS_Y)
				{
					g_game.state = GAMESTATE_NORMAL;
				}
				else if (pArrow->pos.y == RETEY_POS_Y)
				{
					SetFade(MODE_GAME);// ��ʃ��[�h�̐ݒ�
					g_game.state = GAMESTATE_NONE;
				}
				else if (pArrow->pos.y == QUIT_POS_Y)
				{
					SetFade(MODE_TITLE);
					g_game.state = GAMESTATE_NONE;
				}
			}
			break;
		}
	}
}

//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame()
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// �J�����̐ݒ�
	SetCamera(g_game.nNowPlayer);
	
	// ���b�V���t�B�[���h�̕`�揈��
	//DrawMeshField();
	
	// �t�B�[���h�̕`��
	DrawField();

	// �e�̕`�揈��
	DrawShadow();
	
	// �v���C���[�̕`�揈��
	DrawPlayer();

	//�{�[���̕`��
	DrawBall();

	// ���f���̕`�揈��
	DrawModel();
	
	// ���b�V���E�H�[���̕`�揈��
	//DrawMeshWall();

	//�p�[�e�B�N���`��
	DrawParticle();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�J�E���g�_�E���̕`��
	DrawTime();

	// �|�[�Y��ʕ`��
	if (g_game.state == GAMESTATE_PAUSE)
	{// �|�[�Y��Ԃ̂Ƃ�
		DrawPause();
	}
}

//=============================================================================
// ���쒆�̏�Ԃ̎擾
//=============================================================================
int *GetPlayer(void)
{
	return &g_game.nNowPlayer;
}

//=============================================================================
// ����v���C���[�̕ύX
//=============================================================================
void SetChangePlayer(void)
{
	g_game.nNowPlayer++;// ����v���C���[�ύX

	if (g_game.nNowPlayer >= GetNumPad())
	{// ����v���C���[�ȏ�ɂȂ����ꍇ
		g_game.nNowPlayer = 0;	// 1P�ɕύX
	}
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_game.state = state;
	g_game.nCounterState = 0;
}

//=============================================================================
// �Q�[����Ԃ̎擾
//=============================================================================
GAMESTATE *GetGameState(void)
{
	return &g_game.state;
}