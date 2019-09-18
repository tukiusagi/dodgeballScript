//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "ranking.h"
#include "camera.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "keyboard.h"
#include "meshField.h"
#include "meshWall.h"
#include "model.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0			"data/TEXTURE/text_ranking.png"			// �ǂݍ��ރ^�C�g���e�N�X�`��
#define TEXTUREFILE1			"data/TEXTURE/text_ranking_rank.png"	// �ǂݍ��ޏ��ʃe�N�X�`��
#define TEXTUREFILE2			"data/TEXTURE/number000.png"			// �ǂݍ��ރX�R�A�e�N�X�`��
#define MAX_TEXTURE				(3)										// �g�p����e�N�X�`����
#define NUM_RANK				(5)										// ���ʂ̐�
#define MAX_SCORE				(5)										// �g�p����X�R�A�̐�
#define NUM_SCORE				(8)										// �X�R�A�̌���
#define MAX_SET_TEXTURE			(2+NUM_RANK+MAX_SCORE*NUM_SCORE)		// �ݒu����e�N�X�`���̍��v��
#define LOGO_POS_X				(460)									// ���S��X���W
#define LOGO_POS_Y				(50)									// ���S��Y���W
#define LOGO_WIDTH				(360)									// ���S�̕�
#define LOGO_HEIGHT				(60)									// ���S�̍���
#define RANK_POS_X				(260)									// ���ʂ�X���W
#define RANK_POS_Y				(150)									// ���ʂ�Y���W
#define RANK_WIDTH				(128)									// ���ʂ̕�
#define RANK_HEIGHT				(96)									// ���ʂ̍���
#define RANK_INTERVAL			(100)									// ���ʂ̊Ԋu
#define SCORE_POS_X				(360)									// �X�R�A��X���W
#define SCORE_POS_Y				(165)									// �X�R�A��Y���W
#define SCORE_WIDTH				(35)									// �X�R�A�̕�
#define SCORE_HEIGHT			(70)									// �X�R�A�̍���
#define SCORE_INTERVAL_WIDTH	(38)									// �X�R�A���̊Ԋu
#define SCORE_INTERVAL_HEIGHT	(100)									// �X�R�A�c�̊Ԋu
#define RANKING_TIME			(600)									// �����L���O��ʕ\������
#define MAX_VERTEX				(4)										// �e�N�X�`���̒��_��
#define MAX_DATA				(6)										// �\�[�g�p

//=============================================================================
// �\���̒�`�Q[�}�N����`����ɂȂ��ƃ}�W�b�N�i���o�[���킩��Ȃ�]
//=============================================================================
typedef struct// �X�R�A
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
} SCORE;

typedef struct // ����
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
} RANK;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;									// ���_�o�b�t�@�ւ̃|�C���^

int						g_nCntRanking;												// ��ʑJ�ڃJ�E���g
int						g_aData[MAX_DATA] = { 13500,13250,13000,12750,12500,0 };	// �����L���O�X�R�A
int						g_aFlashingData[MAX_DATA];									// �_�ŗp
SCORE					g_aRankingScore[MAX_SCORE][NUM_SCORE];						// �X�R�A�̏��
RANK					g_aRank[NUM_RANK];											// ���ʂ̏��
int						g_nCntRankingText;

//=============================================================================
// �����L���O��ʂ̏�����
//=============================================================================
void InitRanking()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCnt;
	int nCntRanking;
	GAMESTATE *GameState;
	//int *pScore;
	//int *pTime;
	int nData;

	// ������
	g_nCntRanking = RANKING_TIME;
	g_nCntRankingText = 30;

	for (nCntRanking = 0; nCntRanking < MAX_DATA; nCntRanking++)
	{
		g_aFlashingData[nCntRanking] = 0;
	}

	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)// �X�R�A
	{
		for (nCntRanking = 0; nCntRanking < NUM_SCORE; nCntRanking++)
		{
			g_aRankingScore[nCnt][nCntRanking].pos = D3DXVECTOR3(SCORE_POS_X + RANK_WIDTH,
														SCORE_POS_Y + nCnt*SCORE_INTERVAL_HEIGHT*1.0f, 0.0f);
			g_aRankingScore[nCnt][nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	for (nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)// ����
	{
		g_aRank[nCntRanking].pos=D3DXVECTOR3(RANK_POS_X,
									       RANK_POS_Y + nCnt*RANK_INTERVAL*1.0f, 0.0f);
		g_aRank[nCntRanking].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �Q�[����Ԃ̎擾
	GameState = GetGameState();
	
	g_aFlashingData[MAX_DATA - 1] = 1;

	// �����L���O�̃\�[�g
	for (nCnt = 0; nCnt < (MAX_DATA - 1); nCnt++)
	{
		for (nCntRanking = nCnt + 1; nCntRanking < MAX_DATA; nCntRanking++)
		{
			if (g_aData[nCnt] < g_aData[nCntRanking])
			{
				// �X�R�A
				nData = g_aData[nCnt];
				g_aData[nCnt] = g_aData[nCntRanking];
				g_aData[nCntRanking] = nData;

				// �_��
				nData = g_aFlashingData[nCnt];
				g_aFlashingData[nCnt] = g_aFlashingData[nCntRanking];
				g_aFlashingData[nCntRanking] = nData;
			}
		}
	}

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	// �����L���O
	D3DXCreateTextureFromFile(pDevice,
		TEXTUREFILE0,
		&g_pTextureRanking[0]);

	// ����
	D3DXCreateTextureFromFile(pDevice,
		TEXTUREFILE1,
		&g_pTextureRanking[1]);

	// �X�R�A
	D3DXCreateTextureFromFile(pDevice,
		TEXTUREFILE2,
		&g_pTextureRanking[2]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_SET_TEXTURE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ���S
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(LOGO_POS_X + LOGO_WIDTH, LOGO_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y + LOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(LOGO_POS_X + LOGO_WIDTH, LOGO_POS_Y + LOGO_HEIGHT, 0.0f);

	// �������W
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// ����
	for (nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y + RANK_INTERVAL*nCntRanking*1.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RANK_POS_X + RANK_WIDTH, RANK_POS_Y + RANK_INTERVAL*nCntRanking*1.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y + RANK_HEIGHT + RANK_INTERVAL*nCntRanking*1.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RANK_POS_X + RANK_WIDTH, RANK_POS_Y + RANK_HEIGHT + RANK_INTERVAL*nCntRanking*1.0f, 0.0f);

		// �������W
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + nCntRanking*0.2f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + nCntRanking*0.2f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + nCntRanking*0.2f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + nCntRanking*0.2f);

		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// �X�R�A
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		for (nCntRanking = 0; nCntRanking < NUM_SCORE; nCntRanking++)
		{
			// �X�R�A�̍��W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankingScore[nCnt][nCntRanking].pos.x + SCORE_INTERVAL_WIDTH*nCntRanking,
				g_aRankingScore[nCnt][nCntRanking].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankingScore[nCnt][nCntRanking].pos.x + SCORE_INTERVAL_WIDTH*nCntRanking + SCORE_WIDTH,
				g_aRankingScore[nCnt][nCntRanking].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankingScore[nCnt][nCntRanking].pos.x + SCORE_INTERVAL_WIDTH*nCntRanking,
				g_aRankingScore[nCnt][nCntRanking].pos.y + SCORE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankingScore[nCnt][nCntRanking].pos.x + SCORE_INTERVAL_WIDTH*nCntRanking + SCORE_WIDTH,
				g_aRankingScore[nCnt][nCntRanking].pos.y + SCORE_HEIGHT, 0.0f);

			// �������W
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			int nNumber = g_aData[nCnt] % (int)powf(10.0f, (NUM_SCORE - nCntRanking)*1.0f) /
				(int)powf(10.0f, (NUM_SCORE - 1 - nCntRanking)*1.0f);

			// �e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 1.0f);

			pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
		}
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffRanking->Unlock();

	// �J�����̏���������
	InitCamera();

	// �J�����̒����_�̐ݒ�
	SetCameraPosR(D3DXVECTOR3(0.0f, 50.0f, 100.0f), 0);

	// �J�����̎��_�̐ݒ�
	SetCameraPosV(D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0);

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	InitMeshWall();

	// ���f���̏���������
	InitModel();

	//SetModel(D3DXVECTOR3(-70.0f, 0.0f, -140.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MODELTYPE_BILL0);
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
// �����L���O��ʂ̏I������
//=============================================================================
void UninitRanking()
{
	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	// ���f���̏I������
	UninitModel();

	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_pTextureRanking[nCntTex] != NULL)
		{
			g_pTextureRanking[nCntTex]->Release();
			g_pTextureRanking[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//=============================================================================
// �����L���O��ʂ̍X�V����
//=============================================================================
void UpdateRanking()
{
	VERTEX_2D *pVtx;

	// �J�����̒����_�̉�]
	RotMoveCameraV(0.005f, 0);

	// ���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	// ���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	// ���f���̍X�V����
	UpdateModel();

	g_nCntRankingText--;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * 6;

	// �_��
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		for (int nCntRanking = 0; nCntRanking < NUM_SCORE; nCntRanking++)
		{
			if (g_aFlashingData[nCnt] == 1)
			{
				// ���_�J���[
				if (g_nCntRankingText <= 0)
				{
					g_nCntRankingText = 30;
				}
				else if (g_nCntRankingText <= 15)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				else if (g_nCntRankingText <= 30)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			pVtx += 4;
		}
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffRanking->Unlock();

	if (GetFade() != FADE_OUT)
	{
		g_nCntRanking--;
		if (g_nCntRanking <= 0)
		{
			//���[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(0, JOYPADKEY_START) || GetJoypadTrigger(0, JOYPADKEY_A))
		{
			//���[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// �����L���O��ʂ̕`�揈��
//=============================================================================
void DrawRanking()
{
	// �J�����̐ݒ�
	SetCamera(0);

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���E�H�[���̕`�揈��
	DrawMeshWall();

	// ���f���̕`�揈��
	DrawModel();

	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntRanking;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[�~���O�Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���S�`��
	pDevice->SetTexture(0, g_pTextureRanking[0]);			// �e�N�X�`���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		// �|���S���`��

	// ���ʕ`��
	for (nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[1]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (1 + nCntRanking) * 4, 2);
	}

	// �X�R�A�`��
	for (nCntRanking = 0; nCntRanking < NUM_SCORE*MAX_SCORE; nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[2]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (6 + nCntRanking) * 4, 2);
	}
}
