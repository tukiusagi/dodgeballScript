//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "timer.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ސ����e�N�X�`��
#define MAX_TEXTER			(1)								// �g�p����e�N�X�`����
#define MAX_TIME			(2)								// �^�C�}�[�̌�
#define TIME_WIDTH			(30)							// �^�C�}�[�̕�
#define TIME_HEIGHT			(30)							// �^�C�}�[�̍���
#define TIME_INTERVAL		(30)							// �^�C�}�[�̊Ԋu
#define TIMER_MAX			(10)							// ��������
#define TIMER1_MAX			(30)							// ��������

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �\���̒�`�Q[�}�N����`����ɂȂ��ƃ}�W�b�N�i���o�[���킩��Ȃ�]
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
} TIME;

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTimer[MAX_TEXTER] = {};				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;							// ���_�o�b�t�@�ւ̃|�C���^

int						g_nTimer;										// ����
int						g_nCntTimer;									// ���ԃJ�E���g�p
TIME					g_aTimer[MAX_TIME];								// ���Ԃ̏��

//=============================================================================
// ����������
//=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ������
	g_nTimer = TIMER_MAX;
	
	g_nCntTimer = 0;
	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)// �^�C�}�[
	{
		g_aTimer[nCntScore].pos = D3DXVECTOR3(600.0f, 5.0f, 0.0f);
		g_aTimer[nCntScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE, &g_pTextureTimer[0]);

	// ���_���̍쐬
	MakeVertexTimer(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTER; nCntTex++)
	{
		if (g_pTextureTimer[nCntTex] != NULL)
		{
			g_pTextureTimer[nCntTex]->Release();
			g_pTextureTimer[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	VERTEX_2D *pVtx;

	g_nCntTimer++;
	if (0 == g_nCntTimer % 60)
	{
		g_nTimer--;
	}

	if (g_nTimer >= 0)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
		{
			int nNumber = g_nTimer % (int)powf(10.0f, (MAX_TIME - nCntScore)*1.0f) / (int)powf(10.0f, (MAX_TIME - 1 - nCntScore)*1.0f);

			// �e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 1.0f);

			pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
		}
		// ���_�f�[�^���A�����b�N
		g_pVtxBuffTimer->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �X�R�A&�^�C�}�[�̕`��
	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTimer[0]);

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)// �^�C�}�[
	{
		// �^�C�}�[�̍��W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTimer[nCntScore].pos.x + TIME_INTERVAL*nCntScore,
			g_aTimer[nCntScore].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTimer[nCntScore].pos.x + TIME_INTERVAL*nCntScore + TIME_WIDTH,
			g_aTimer[nCntScore].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTimer[nCntScore].pos.x + TIME_INTERVAL*nCntScore,
			g_aTimer[nCntScore].pos.y + TIME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTimer[nCntScore].pos.x + TIME_INTERVAL*nCntScore + TIME_WIDTH,
			g_aTimer[nCntScore].pos.y + TIME_HEIGHT, 0.0f);

		// �������W
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = g_aTimer[nCntScore].col;

		int nNumber = g_nTimer % (int)powf(10.0f, (MAX_TIME - nCntScore)*1.0f) / (int)powf(10.0f, (MAX_TIME - 1 - nCntScore)*1.0f);

		// �e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber*0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber*0.1f, 1.0f);

		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// ���ԏ��̎擾
//=============================================================================
int *GetTimer(void)
{
	return &g_nTimer;
}
