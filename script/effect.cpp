//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "effect.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_EFFECT		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`��
#define	MAX_EFFECT			(4096)							// �G�t�F�N�g�̍ő吔

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR	col;		// �F
	float		fRadius;	// ���a
	int			nLife;		// ���C�t
	bool		bUse;		// �g�p���Ă��邩�ǂ���
	float		fNumCol;	// �v�Z�p�֐�
	float		fNumRad;	// �v�Z�p�֐�
} EFFECT;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

EFFECT					g_aEffect[MAX_EFFECT];		// �G�t�F�N�g���

//=============================================================================
// ����������
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ������
	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
		g_aEffect[nCntEffect].fNumCol = 0.0f;
		g_aEffect[nCntEffect].fNumRad = 0.0f;
	}

	// ���_���̍쐬
	MakeVertexEffect(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̊J��
	if(g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if(g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)// �G�t�F�N�g���g�p����Ă��邩
		{
			// �ʒu�̍X�V
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			// ���_�J���[�̍X�V
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fNumCol;	// �A���t�@�l�����X�Ɍ��炷
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fNumRad;	// ���a�����X�ɏ���������
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)// ���C�t��0�ɂȂ�����
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	EFFECT *pEffect;

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);// Z�C�l�[�u���𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);// Z�t�@���N�V�����̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// Z���C�g�C�l�[�u���𖳌��ɂ���

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ���C�e�B���O�𖳌��ɂ���

	pEffect = &g_aEffect[0];
	for (int nCntBullet = 0; nCntBullet < MAX_EFFECT; nCntBullet++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntBullet].mtxWorld);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��̐ݒ�
			g_aEffect[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntBullet].mtxWorld._33 = mtxView._33;

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntBullet].rot.y,
				g_aEffect[nCntBullet].rot.x, g_aEffect[nCntBullet].rot.z);
			D3DXMatrixMultiply(&g_aEffect[nCntBullet].mtxWorld, &g_aEffect[nCntBullet].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntBullet].pos.x,
				g_aEffect[nCntBullet].pos.y, g_aEffect[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntBullet].mtxWorld, &g_aEffect[nCntBullet].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntBullet].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		// Z�C�l�[�u����L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�t�@���N�V�������f�t�H���g�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z���C�g�C�l�[�u����L���ɂ���

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�e�B���O��L���ɂ���
}

//=============================================================================
// �G�t�F�N�g�ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if(!g_aEffect[nCntEffect].bUse)
		{// �g�p���Ă��Ȃ��ꍇ
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;
			g_aEffect[nCntEffect].fNumCol = 1.0f / g_aEffect[nCntEffect].nLife;
			g_aEffect[nCntEffect].fNumRad = g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			
			break;
		}
		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTextureEffect);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = pVtx[1].pos = pVtx[2].pos = pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
