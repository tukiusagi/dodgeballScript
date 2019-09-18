//=============================================================================
//
// �J�E���g�_�E������ [time.cpp]
// Author : YONEYAMA TAKESHI
//
//=============================================================================
#include "time.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_TIME	"data/TEXTURE/�K��number001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define TIME_POS_X		(1280.0f)							// �|���S���̕\���ʒu(����X)
#define TIME_POS_Y		(300.0f)							//�|���S���̕\���ʒu(����Y)
#define TIME_SIZE_X		(300)								// �|���S���̃T�C�Y(����)
#define TIME_SIZE_Y		(300)								// �|���S���̃T�C�Y(�c��)

#define MAX_TIME (1)			//�^�C���̍ő吔
#define FPS_TIME (60)			//1�b�J�E���g

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

int						g_nTime;				// �^�C���J�E���g

TIME					g_anTime[MAX_TIME];

//=============================================================================
// �^�C������������
//=============================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTime = FPS_TIME * 4;

	int nCntTime;
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_anTime[nCntTime].bUse = true;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TIME,		// �t�@�C���̖��O
								&g_pTextureTime);	// �ǂݍ��ރ������[
	
	// ���_���̍쐬
	MakeVertexTime(pDevice);
}

//=============================================================================
// �^�C���I������
//=============================================================================
void UninitTime(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//=============================================================================
// �^�C���X�V����
//=============================================================================
void UpdateTime(void)
{
	AddTime(-1);
	int nCntTime;
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		if (g_nTime <= FPS_TIME)
		{//1�b�ȉ��ɂȂ����������
			g_anTime[nCntTime].bUse = false;
		}
	}
}

//=============================================================================
// �^�C���`�揈��
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntTime;

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		if (g_anTime[nCntTime].bUse == true)			//�g�p���Ȃ�`��
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTime);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
		}
	}

	// �A���t�@�e�X�g�߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	int nCntTime;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTime,
								NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_anTime[nCntTime].pos = D3DXVECTOR3((TIME_POS_X / 2) + TIME_SIZE_X * nCntTime, TIME_POS_Y, 0.0f);
		g_anTime[nCntTime].nNm = 0;

		pVtx[0].pos = D3DXVECTOR3(g_anTime[nCntTime].pos.x - (TIME_SIZE_X / 2.0f), g_anTime[nCntTime].pos.y - (TIME_SIZE_Y / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_anTime[nCntTime].pos.x + (TIME_SIZE_X / 2.0f), g_anTime[nCntTime].pos.y - (TIME_SIZE_Y / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_anTime[nCntTime].pos.x - (TIME_SIZE_X / 2.0f), g_anTime[nCntTime].pos.y + (TIME_SIZE_Y / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_anTime[nCntTime].pos.x + (TIME_SIZE_X / 2.0f), g_anTime[nCntTime].pos.y + (TIME_SIZE_Y / 2.0f), 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		 
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_anTime[nCntTime].nNm, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * g_anTime[nCntTime].nNm, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * g_anTime[nCntTime].nNm, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * g_anTime[nCntTime].nNm, 1.0f);

		pVtx += 4;
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// �^�C���̕ύX
//=============================================================================
void AddTime(int nValue)
{
	VERTEX_2D *pVtx;

	int nCntTime;

	g_nTime += nValue;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_anTime[nCntTime].nNm = (g_nTime / 60) % (int)powf(10.0f, 1.0f - nCntTime * 1.0f) / (int)powf(10.0f, 1.0f - 1.0f - nCntTime * 1.0f);

		pVtx[0].col = D3DXCOLOR(0.0f, 0.2f * g_anTime[nCntTime].nNm, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.2f * g_anTime[nCntTime].nNm, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.2f * g_anTime[nCntTime].nNm, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.2f * g_anTime[nCntTime].nNm, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_anTime[nCntTime].nNm, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * g_anTime[nCntTime].nNm, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * g_anTime[nCntTime].nNm, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * g_anTime[nCntTime].nNm, 1.0f);

		pVtx += 4;
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffTime->Unlock();

	if (g_nTime % 60 == 0 && g_nTime > 60)
	{
		//PlaySound(SOUND_LABEL_SE_COUNT);
	}
}