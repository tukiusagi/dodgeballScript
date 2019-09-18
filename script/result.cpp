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
LPDIRECT3DTEXTURE9			g_pTextureR[MAX_RTEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffR = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

int							g_nCntR;						// ��ʑJ�ڃJ�E���g
int							g_nCntS[MAX_NUMPLAYER];			// �X�R�A�J�E���^�[
int							aData[MAX_NUMPLAYER] = {};		// ���U���g�X�R�A
RESULT						g_Result[MAX_RTEXTURE];			// ���U���g�̏��

//=============================================================================
// ���U���g��ʂ̏�����
//=============================================================================
void InitResult()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X�̎擾

	// ������
	g_nCntR = 1800;
	int nData;

	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		g_nCntS[nCnt] = 0;
	}

	for (int nCnt = 0; nCnt < MAX_RTEXTURE; nCnt++)
	{
		g_Result[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Result[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Result[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Result[nCnt].fWidht = 0.0f;
		g_Result[nCnt].fHeight = 0.0f;
		g_Result[nCnt].nState = 0;
		g_Result[nCnt].nType = 0;
		g_Result[nCnt].bUse = false;
	}

	// ���U���g�X�R�A�̃\�[�g
	for (int nCnt = 0; nCnt < (MAX_NUMPLAYER - 1); nCnt++)
	{
		for (int nCntM = nCnt; nCntM < MAX_NUMPLAYER; nCntM++)
		{
			if (aData[nCnt] < aData[nCntM])
			{
				nData = aData[nCnt];
				aData[nCnt] = aData[nCntM];
				aData[nCntM] = nData;
			}
		}
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE1, &g_pTextureR[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE2, &g_pTextureR[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE3, &g_pTextureR[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE4, &g_pTextureR[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE5, &g_pTextureR[4]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RTEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffR,
		NULL);

	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffR->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RTEXTURE; nCnt++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �������W
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffR->Unlock();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// ���b�V���E�H�[���̏���������
	InitMeshWall();

	// ���ʔ��\
	SetResult(D3DXVECTOR3(420.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 390.0f, R_POS_Y, 4);
	// ���
	SetResult(D3DXVECTOR3(R_POS_X + 0.0f, R_POS_Y, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), R_WIDTH, 600.0f / (4 + 1)*2, 0);		
	// ���
	SetResult(D3DXVECTOR3(R_POS_X + 180.0f, R_POS_Y + R_HEIGHT / (4 + 1) * 2, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), R_WIDTH, R_HEIGHT / (4 + 1), 1);
	// �O��
	SetResult(D3DXVECTOR3(R_POS_X + 360.0f, R_POS_Y + R_HEIGHT / (4 + 1) * 2 + R_HEIGHT / (4 + 1) * 1, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), R_WIDTH, R_HEIGHT / (4 + 1), 2);
	// �l��
	SetResult(D3DXVECTOR3(R_POS_X + 540.0f, R_POS_Y + R_HEIGHT / (4 + 1) * 2 + (R_HEIGHT / (4 + 1)) * 2, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), R_WIDTH, R_HEIGHT / (4 + 1), 3);
}

//=============================================================================
// ���U���g��ʂ̏I������
//=============================================================================
void UninitResult()
{
	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���E�H�[���̏I������
	UninitMeshWall();

	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_RTEXTURE; nCntTex++)
	{
		if (g_pTextureR[nCntTex] != NULL)
		{
			g_pTextureR[nCntTex]->Release();
			g_pTextureR[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffR != NULL)
	{
		g_pVtxBuffR->Release();
		g_pVtxBuffR = NULL;
	}
}

//=============================================================================
// ���U���g��ʂ̍X�V����
//=============================================================================
void UpdateResult()
{
	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	// ���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffR->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RTEXTURE; nCnt++)
	{
		if (g_Result[nCnt].bUse)
		{
			g_Result[nCnt].pos.x += g_Result[nCnt].move.x;

			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Result[nCnt].pos.x, g_Result[nCnt].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result[nCnt].pos.x + g_Result[nCnt].fWidht, g_Result[nCnt].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result[nCnt].pos.x, g_Result[nCnt].pos.y + g_Result[nCnt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result[nCnt].pos.x + g_Result[nCnt].fWidht, g_Result[nCnt].pos.y + g_Result[nCnt].fHeight, 0.0f);

			// �ړ�����
			if (g_Result[nCnt].pos.x <= 0.0f)
			{
				g_Result[nCnt].move.x = 0.0f;

				switch (g_Result[nCnt].nType)
				{
				case RTYPE_FIRST:	// 0

					g_nCntS[RTYPE_FIRST]++;

					break;

				case RTYPE_SECOND:	// 1

					g_nCntS[RTYPE_SECOND]++;

					//pFunds += RTYPE_SECOND;

					break;

				case RTYPE_THIRD:	// 2

					g_nCntS[RTYPE_THIRD]++;

					//pFunds += RTYPE_THIRD;
					break;

				case RTYPE_FOUR:	// 3

					g_nCntS[RTYPE_FOUR]++;

					//pFunds += RTYPE_FOUR;
					break;
				}
			}
		}

		pVtx += 4;
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffR->Unlock();

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
	LPDIRECT3DDEVICE9 pDevice;

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���E�H�[���̕`�揈��
	DrawMeshWall();

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[�~���O�Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffR, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFR = 0; nCntFR < MAX_RTEXTURE; nCntFR++)
	{
		if (g_Result[nCntFR].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureR[g_Result[nCntFR].nType]);

			// �|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFR * 4, 2);
		}
	}
}

//=============================================================================
// ���U���g�̔z�u���
//=============================================================================
void SetResult(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nType)
{
	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffR->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RTEXTURE; nCnt++)
	{
		if (!g_Result[nCnt].bUse)
		{
			g_Result[nCnt].pos = pos;
			g_Result[nCnt].move = move;
			g_Result[nCnt].col = col;
			g_Result[nCnt].fWidht = fWidth;
			g_Result[nCnt].fHeight = fHeight;
			g_Result[nCnt].nType = nType;
			g_Result[nCnt].bUse = true;

			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Result[nCnt].pos.x, g_Result[nCnt].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result[nCnt].pos.x + g_Result[nCnt].fWidht, g_Result[nCnt].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result[nCnt].pos.x, g_Result[nCnt].pos.y + g_Result[nCnt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result[nCnt].pos.x + g_Result[nCnt].fWidht, g_Result[nCnt].pos.y + g_Result[nCnt].fHeight, 0.0f);

			// �F�̐ݒ�
			pVtx[0].col = g_Result[nCnt].col;
			pVtx[1].col = g_Result[nCnt].col;
			pVtx[2].col = g_Result[nCnt].col;
			pVtx[3].col = g_Result[nCnt].col;

			break;
		}

		pVtx += 4;
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffR->Unlock();
}
