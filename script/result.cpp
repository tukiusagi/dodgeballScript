//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : DAIKI TSUCHIDA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result.h"
#include "camera.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "keyboard.h"
#include "meshField.h"
#include "meshWall.h"
#include "sound.h"
#include "player.h"
#include "title.h"
#include"logo.h"

//=============================================================================
// ���U���g�̎�ނƏ��
//=============================================================================
typedef enum
{
	RTYPE_FIRST = 0,	// ���
	RTYPE_SECOND,		// ���
	RTYPE_THIRD,		// �O��
	RTYPE_FOUR,			// �l��
	RTYPE_RESULT,		// ���ʔ��\
	RTYPE_MAX
} RTYPE;

typedef enum
{
	RSTATE_NONE = 0,	// �������Ă��Ȃ�
	RSTATE_NORMAL,		// �ʏ���
	RSTATE_INSCORE,		// �X�R�A�\��
	RSTATE_NEXT = 0,	// ���̉�ʂ�
	RSTATE_MAX
} RSTATE;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR	col;
	float		fWidht;
	float		fHeight;
	int			nState;
	int			nType;
	bool		bUse;
} RESULT;

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE1		"data/TEXTURE/Gold.png"				// �ǂݍ��ރe�N�X�`��1
#define TEXTUREFILE2		"data/TEXTURE/Silver.png"			// �ǂݍ��ރe�N�X�`��2
#define TEXTUREFILE3		"data/TEXTURE/Copper.png"			// �ǂݍ��ރe�N�X�`��3
#define TEXTUREFILE4		"data/TEXTURE/black.png"			// �ǂݍ��ރe�N�X�`��4
#define TEXTUREFILE5		"data/TEXTURE/���ʔ��\.png"			// �ǂݍ��ރe�N�X�`��5
#define MAX_RTEXTURE		(5)									// �g�p����e�N�X�`����
#define R_POS_X				(1280.0f)							// �e�N�X�`����X���W
#define R_POS_Y				(120.0f)							// �e�N�X�`����Y���W
#define R_WIDTH				(1280.0f)							// �e�N�X�`���̕�
#define R_HEIGHT			(600.0f)							// �e�N�X�`���̍���
#define MAX_NUMPLAYER		(4)									// �Q���l��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
int	g_nCntR;						// ��ʑJ�ڃJ�E���g

//=============================================================================
// ���U���g��ʂ̏�����
//=============================================================================
void InitResult()
{
	// ������
	g_nCntR = 1800;

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	InitMeshWall();

	//�������S�̏�����
	InitLogo();

	int nID = 0;		//�����c�����v���C���[�̔ԍ�
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		if (GetPlayerUse(nCnt))
		{	//���Ԃ������c�����H
			nID = nCnt;
		}
	}
	SetLogo((LOGOTYPE)nID);

}

//=============================================================================
// ���U���g��ʂ̏I������
//=============================================================================
void UninitResult()
{
	// �v���C���[�̏I������
	UninitPlayer();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	//���S�̏I��
	UninitLogo();
}

//=============================================================================
// ���U���g��ʂ̍X�V����
//=============================================================================
void UpdateResult()
{
	// ���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	// ���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	//���S�̍X�V
	UpdateLogo();

	g_nCntR--;		// �J�E���^�[��i�߂�

	if (GetFade() != FADE_OUT)
	{
		if (g_nCntR <= 0)
		{
			//���[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
			GetJoypadTrigger(0, JOYPADKEY_START) || GetJoypadTrigger(0, JOYPADKEY_A))
		{
			// ���[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}

}

//=============================================================================
// ���U���g��ʂ̕`�揈��
//=============================================================================
void DrawResult()
{
	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���E�H�[���̕`�揈��
	DrawMeshWall();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	//���S�̕`��
	DrawLogo();
}