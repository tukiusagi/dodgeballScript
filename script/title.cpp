//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "title.h"
#include "camera.h"
#include "fade.h"
#include "joypad.h"
#include "keyboard.h"
#include "meshField.h"
#include "meshWall.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0	"data/TEXTURE/RideParty.png"			// �ǂݍ��ރ^�C�g���e�N�X�`��
#define TEXTUREFILE1	"data/TEXTURE/text_press_button.png"	// �ǂݍ��ރe�L�X�g�e�N�X�`��
#define TEXTUREFILE2	"data/TEXTURE/start1.png"				// �ǂݍ��ރe�L�X�g�e�N�X�`��
#define TEXTUREFILE3	"data/TEXTURE/�^�[���ݒ�.png"			// �ǂݍ��ރe�L�X�g�e�N�X�`��
#define TEXTUREFILE4	"data/TEXTURE/NumSet.png"				// �ǂݍ��ރe�L�X�g�e�N�X�`��
#define TEXTUREFILE5	"data/TEXTURE/��.png"					// �ǂݍ��ރe�L�X�g�e�N�X�`��

#define TITLE_POS_X		(640)									// �^�C�g����X���W
#define TITLE_POS_Y		(150)									// �^�C�g����Y���W
#define TITLE_WIDTH		(1000)									// �^�C�g���̕�
#define TITLE_HEIGHT	(150)									// �^�C�g���̍���
#define TEXT_POS_X		(400)									// �e�L�X�g��X���W
#define TEXT_POS_Y		(500)									// �e�L�X�g��Y���W
#define TEXT_WIDTH		(480)									// �e�L�X�g�̕�
#define TEXT_HEIGHT		(80)									// �e�L�X�g�̍���

#define PAUSE_POS_X		(640)									// �Z���N�g���j���[�S�̂�X���W

#define CONTINUE_POS_Y	(410)									// �n�߂��Y���W
#define RETEY_POS_Y		(510)									// �^�[���ݒ��Y���W
#define QUIT_POS_Y		(600)									// �I����Y���W

#define CONTINUE_WIDTH	(300)									// �n�߂�̕�
#define CONTINUE_HEIGHT	(100)									// �n�߂�̍���
#define RETEY_WIDTH		(300)									// �^�[���ݒ�̕�
#define RETEY_HEIGHT	(100)									// �^�[���ݒ�̍���
#define QUIT_WIDTH		(300)									// �߂�̕�
#define QUIT_HEIGHT		(100)									// �߂�̍���
#define ARROW_WIDTH		(70)									// ���̕�
#define ARROW_HEIGHT	(50)									// ���̍���

#define TURNSET_POS_Y	(370)									//�^�[���ݒ�e�N�X�`����Y���W
#define TURN5_POS_Y		(470)									//5�^�[���e�N�X�`����Y���W
#define TURN10_POS_Y	(570)									//10�^�[���e�N�X�`����Y���W
#define TURN15_POS_Y	(670)									//15�^�[���e�N�X�`����Y���W

#define NUMSET_POS_Y	(370)									//�l���ݒ�e�N�X�`����Y���W
#define N2_POS_Y		(470)									//2�e�N�X�`����Y���W
#define N3_POS_Y		(570)									//3�e�N�X�`����Y���W
#define N4_POS_Y		(670)									//4�e�N�X�`����Y���W

#define TURN_WIDTH		(230)									//�^�[���e�N�X�`���̕�
#define TURN_HEIGHT		(70)									//�^�[���e�N�X�`���̍���

#define MAX_TEXTURE		(12)										// �g�p����e�N�X�`����
#define NUM_TEXT		(12)										// �e�L�X�g��
#define MAX_VERTEX		(4)										// �e�N�X�`���̒��_��

//=============================================================================
// �^�C�g���̏��
//=============================================================================
typedef enum
{
	TITLESTATE_STANDBY = 0,		// �ҋ@���
	TITLESTATE_SELECT,			// ���[�h�I�����
	TITLESTATE_FADE,			// ��ʑJ�ڏ��
	TITLESTATE_MAX				// ��Ԃ̍ő吔
} TITLESTATE;

//=============================================================================
// �\���̒�`�Q[�}�N����`����ɂȂ��ƃ}�W�b�N�i���o�[���킩��Ȃ�]
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR col;
	bool bUse;
} TITLETEXT;

typedef struct
{
	D3DXVECTOR3 pos;
	bool bUse;
} ARROW;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTitle[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

TITLESTATE				g_TitleState;						// �^�C�g���̏��

int						g_nCntTitleText;					// �e�L�X�g�J�E���g�p
TITLETEXT				g_TitleText[NUM_TEXT];				// �^�C�g���e�L�X�g���
ARROW					g_TitleArrow;						// ���̏��

JOYPADUSE				g_TitlePadUse;						// �W���C�p�b�h�̎g�p��Ԃ̏��

int						g_nSetTurn;							// �ݒ�^�[����

//=============================================================================
// �^�C�g����ʂ̏�����
//=============================================================================
void InitTitle()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X�̎擾

	// ������
	g_nCntTitleText = 60;
	g_TitleState = TITLESTATE_STANDBY;
	g_TitleArrow.pos = D3DXVECTOR3(450, CONTINUE_POS_Y, 0.0f);
	g_TitleArrow.bUse = false;
	g_nSetTurn = 1;

	for (int nCntTitle = 0; nCntTitle < NUM_TEXT; nCntTitle++)
	{//�e�N�X�`���̏�����
		if (nCntTitle == 0 || nCntTitle == 1)
		{
			g_TitleText[nCntTitle].bUse = true;
		}
		else
		{
			g_TitleText[nCntTitle].bUse = false;
		}

		g_TitleText[nCntTitle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TitleText[nCntTitle].col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`���̓ǂݍ���
	// �^�C�g��
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_pTextureTitle[0]);

	// �e�L�X�g
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE1, &g_pTextureTitle[1]);		// �v���X�{�^��
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE2, &g_pTextureTitle[2]);		// �n�߂�
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE3, &g_pTextureTitle[3]);		// �ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE4, &g_pTextureTitle[4]);		// �߂�
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE5, &g_pTextureTitle[5]);		// ���

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �^�C�g��
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - (TITLE_WIDTH / 2), TITLE_POS_Y - (TITLE_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + (TITLE_WIDTH / 2), TITLE_POS_Y - (TITLE_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - (TITLE_WIDTH / 2), TITLE_POS_Y + (TITLE_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + (TITLE_WIDTH / 2), TITLE_POS_Y + (TITLE_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �e�L�X�g
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(TEXT_POS_X, TEXT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TEXT_POS_X + TEXT_WIDTH, TEXT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TEXT_POS_X, TEXT_POS_Y + TEXT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TEXT_POS_X + TEXT_WIDTH, TEXT_POS_Y + TEXT_HEIGHT, 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �n�߂�
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (CONTINUE_WIDTH / 2), CONTINUE_POS_Y - (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (CONTINUE_WIDTH / 2), CONTINUE_POS_Y - (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (CONTINUE_WIDTH / 2), CONTINUE_POS_Y + (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (CONTINUE_WIDTH / 2), CONTINUE_POS_Y + (CONTINUE_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �^�[���ݒ�
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �߂�
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// ���
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffTitle->Unlock();

	// �J�����̏���������
	InitCamera();

	// �J�����̒����_�ݒ�
	SetCameraPosR(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 0);

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	InitMeshWall();

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏I������
	InitPlayer();

	// ���f���̏���������
	InitModel();

	SetModel(D3DXVECTOR3(200.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f), MODELTYPE_BILL0);
	SetModel(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MODELTYPE_BILL1);
	SetModel(D3DXVECTOR3(200.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f), MODELTYPE_BILL2);
	SetModel(D3DXVECTOR3(200.0f, 0.0f, -150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODELTYPE_BILL3);
	SetModel(D3DXVECTOR3(-220.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f), MODELTYPE_BILL4);

	SetModel(D3DXVECTOR3(450.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f), MODELTYPE_CONVENI);
	SetModel(D3DXVECTOR3(-380.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODELTYPE_CONVENI);

	SetModel(D3DXVECTOR3(-380.0f, 0.0f, -280.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MODELTYPE_WAC);
}

//=============================================================================
// �^�C�g����ʂ̏I������
//=============================================================================
void UninitTitle()
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	// �e�̏I������
	UninitShadow();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���f���̏I������
	UninitModel();
}

//=============================================================================
// �^�C�g����ʂ̍X�V����
//=============================================================================
void UpdateTitle()
{
	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^
	int nCntTitle;
	int nStickH, nStickV;

	// �A�i���O�X�e�B�b�N���̎擾
	GetJoypadStickLeft(0, &nStickH, &nStickV);

	g_nCntTitleText--;

	// �J�����̒����_�̉�]
	RotMoveCameraR(0.005f, 0);

	// ���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	// ���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	// ���f���̍X�V����
	UpdateModel();

	switch (g_TitleState)
	{
//=============================================================================
	case TITLESTATE_STANDBY:
//=============================================================================

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		// ���_�J���[�̍X�V(���Ԃœ_��)
		if (g_nCntTitleText <= 0)
		{
			g_nCntTitleText = 60;
		}
		else if (g_nCntTitleText <= 20)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (g_nCntTitleText <= 60)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N
		g_pVtxBuffTitle->Unlock();

		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
			GetJoypadTrigger(0, JOYPADKEY_START) || GetJoypadTrigger(0, JOYPADKEY_A))
		{
			// ���ʉ��̐ݒ�
			PlaySound(SOUND_LABEL_SE_DECISION);

			g_TitleState = TITLESTATE_SELECT;

		}
		break;
//=============================================================================
	case TITLESTATE_SELECT:
//=============================================================================

		g_TitleArrow.bUse = true;
		g_TitleText[1].bUse = false;

		for (nCntTitle = 2; nCntTitle < NUM_TEXT; nCntTitle++)
		{
			g_TitleText[nCntTitle].bUse = true;

			if (nCntTitle == 6 || nCntTitle == 7 || nCntTitle == 8|| nCntTitle == 9 || nCntTitle == 10 || nCntTitle == 11)
			{
				g_TitleText[nCntTitle].bUse = false;
			}
		}

		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoypadTrigger(0, JOYPADKEY_UP) || nStickV > 0)
		{
			if (g_TitlePadUse.UpThumb == false)
			{
				PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�

				if (g_TitleArrow.pos.y == CONTINUE_POS_Y)
				{

				}
				else if (g_TitleArrow.pos.y == RETEY_POS_Y)
				{
					PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�
					g_TitleArrow.pos.y = CONTINUE_POS_Y;
				}
				else if (g_TitleArrow.pos.y == QUIT_POS_Y)
				{
					PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�
					g_TitleArrow.pos.y = RETEY_POS_Y;
				}
				g_TitlePadUse.UpThumb = true;
			}
		}
		else if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoypadTrigger(0, JOYPADKEY_DOWN) || nStickV < 0)
		{
			if (g_TitlePadUse.DownThumb == false)
			{
				PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�

				if (g_TitleArrow.pos.y == CONTINUE_POS_Y)
				{
					PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�
					g_TitleArrow.pos.y = RETEY_POS_Y;
				}
				else if (g_TitleArrow.pos.y == RETEY_POS_Y)
				{
					PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�
					g_TitleArrow.pos.y = QUIT_POS_Y;
				}
				else if (g_TitleArrow.pos.y == QUIT_POS_Y)
				{

				}
				g_TitlePadUse.DownThumb = true;
			}
		}
		else
		{
			g_TitlePadUse.UpThumb = false;
			g_TitlePadUse.DownThumb = false;
		}

		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(0, JOYPADKEY_A))
		{//���[�h�̑I����
			PlaySound(SOUND_LABEL_SE_POUSE_1);// ���ʉ��̐ݒ�

			if (g_TitleArrow.pos.y == CONTINUE_POS_Y)
			{//�n�߂�
				g_TitleState = TITLESTATE_FADE;

				// �t�F�[�h�̐ݒ�
				SetFade(MODE_TUTORIAL);
			}
			else if (g_TitleArrow.pos.y == RETEY_POS_Y)
			{//�I�v�V����
				//g_TitleState = TITLESTATE_TURN;
			}
			else if (g_TitleArrow.pos.y == QUIT_POS_Y)
			{//�߂�
				//g_TitleState = TITLESTATE_PLAYERNUM;
			}
		}

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * 3;//�^�[���ݒ�܂Ői�߂�

		pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);

		pVtx += 4;// �l���ݒ�܂Ői�߂�

		pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);

		pVtx += 4;// ���܂Ői�߂�

		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);

		// ���_�f�[�^���A�����b�N
		g_pVtxBuffTitle->Unlock();

		break;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * 5;// ���܂Ői�߂�

		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TitleArrow.pos.x - (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TitleArrow.pos.x + (ARROW_WIDTH / 2), g_TitleArrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);

		// ���_�f�[�^���A�����b�N
		g_pVtxBuffTitle->Unlock();

		break;

//=============================================================================
	case TITLESTATE_FADE:
//=============================================================================

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		// ���_�J���[�̍X�V
		if (g_nCntTitleText <= 0)
		{
			g_nCntTitleText = 4;
		}
		else if (g_nCntTitleText <= 2)
		{
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (g_nCntTitleText <= 4)
		{
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N
		g_pVtxBuffTitle->Unlock();

		break;
	}
}

//=============================================================================
// �^�C�g����ʂ̕`�揈��
//=============================================================================
void DrawTitle()
{
	// �J�����̐ݒ�
	SetCamera(0);

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���E�H�[���̕`�揈��
	DrawMeshWall();

	// ���f���̕`�揈��
	DrawModel();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X���擾����

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[�~���O�Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �^�C�g���ƃe�L�X�g�̕`��
	for (int nCntTitle = 0; nCntTitle < NUM_TEXT; nCntTitle++)
	{
		if (g_TitleText[nCntTitle].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

			// �|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle *4, 2);
		}
	}

	if (g_TitleArrow.bUse == true)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[5]);

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 5 * 4, 2);
	}
}
