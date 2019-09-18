//=============================================================================
//
// �e���� [shadow.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "shadow.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0	"data/TEXTURE/shadow000.jpg"			// �ǂݍ��ރe�N�X�`��
#define MAX_TEXTURE		(1)										// �e�N�X�`����
#define MAX_SHADOW		(256)									// �e�̍ő吔

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;						// �ʒu
	D3DXVECTOR3	rot;						// ��]
	D3DXMATRIX	mtxWorld;					// ���[���h�}�g���b�N�X
	bool		bUse;						// �g�p���Ă��邩
} SHADOW;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_apTextureShadow = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

SHADOW					g_aShadow[MAX_SHADOW];					// �e�̏��

//=============================================================================
// ����������
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e���̏����ݒ�
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].bUse = false;
	}
	// ���_���̍쐬
	MakeVertexShadow(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	// �e�N�X�`���̉��
	if (g_apTextureShadow != NULL)
	{
		g_apTextureShadow->Release();
		g_apTextureShadow = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	SHADOW *pShadow;

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);// Z�t�@���N�V�����̐ݒ�

	pShadow = &g_aShadow[0];
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if (pShadow->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y,
				g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x,
				g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureShadow);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);
		}
	}
	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);// Z�t�@���N�V�����̐ݒ�
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_apTextureShadow);

	VERTEX_3D *pVtx;

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// ���W�̐ݒ�
		pVtx[0].pos = pVtx[1].pos = pVtx[2].pos = pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̍쐬
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	VERTEX_3D *pVtx;
	int nCntShadow;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;

			// ���W�̐ݒ�Q��ɂ���قǉe���傫���Ȃ�
			pVtx[0].pos = D3DXVECTOR3(-size.x - pos.y / 25, 0.1f, +size.z + pos.y / 25);
			pVtx[1].pos = D3DXVECTOR3(+size.x + pos.y / 25, 0.1f, +size.z + pos.y / 25);
			pVtx[2].pos = D3DXVECTOR3(-size.x - pos.y / 25, 0.1f, -size.z - pos.y / 25);
			pVtx[3].pos = D3DXVECTOR3(+size.x + pos.y / 25, 0.1f, -size.z - pos.y / 25);

			// ���_�J���[�̐ݒ�Q��ɂ���قǉe�������Ȃ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - pos.y / 1000);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - pos.y / 1000);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - pos.y / 1000);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - pos.y / 1000);

			break;
		}
		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//=============================================================================
// �e�̈ʒu��ݒ�
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	if (g_aShadow[nIdxShadow].bUse == false)
	{
		g_aShadow[nIdxShadow].bUse = true;
	}
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.y = pos.y + 0.01f;
	g_aShadow[nIdxShadow].pos.z = pos.z;
}

//=============================================================================
// �e���폜
//=============================================================================
void DeleteShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
