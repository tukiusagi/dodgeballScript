//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PARTICLE_TEXTURE	"data/TEXTURE/pics2269.png"	//�ǂݍ��ރe�N�X�`����
#define PARTICLE_SIZE		(10.0f)	//�T�C�Y
#define PARTICLE_MAX		(1000)	//�\���ő吔

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �p�[�e�B�N���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;			//�ʒu
	D3DXVECTOR3	move;			//�ړ���
	D3DXVECTOR3	rot;			//����
	D3DXCOLOR	col;			//�F
	float		fRadius;		// ���a
	int			nLife;			// ���C�t
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	bool		bUse;			//�g�p���
}PARTICLE;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffParticle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//�e�N�X�`���̃|�C���^

PARTICLE g_aParticle[PARTICLE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		PARTICLE_TEXTURE,
							&g_pTextureParticle);

	// ���_���̍쐬
	MakeVertexParticle(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureParticle != NULL)
	{			  
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffParticle != NULL)
	{			  
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)	//�e���g�p����Ă���
		{
			g_aParticle[nCntParticle].nLife--;
			g_aParticle[nCntParticle].fRadius -= g_aParticle[nCntParticle].fRadius/ g_aParticle[nCntParticle].nLife;
			g_aParticle[nCntParticle].col.a -= 0.03f;
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			if (g_aParticle[nCntParticle].fRadius <= 0)
			{
				g_aParticle[nCntParticle].fRadius = 0;
			}
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntParticle * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			// ���_�f�[�^���A�����b�N����
			g_pVtxBuffParticle->Unlock();

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// �v�Z�p�}�g���b�N�X

	PARTICLE *pParticle;

	pParticle = &g_aParticle[0];

	//���݂̃r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pParticle->mtxWorld);

			//�t�s��̐ݒ�
			pParticle->mtxWorld._11 = mtxView._11;
			pParticle->mtxWorld._12 = mtxView._21;
			pParticle->mtxWorld._13 = mtxView._31;
			pParticle->mtxWorld._21 = mtxView._12;
			pParticle->mtxWorld._22 = mtxView._22;
			pParticle->mtxWorld._23 = mtxView._32;
			pParticle->mtxWorld._31 = mtxView._13;
			pParticle->mtxWorld._32 = mtxView._23;
			pParticle->mtxWorld._33 = mtxView._33;

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pParticle->rot.y, pParticle->rot.x, pParticle->rot.z);

			D3DXMatrixMultiply(&pParticle->mtxWorld, &pParticle->mtxWorld, &mtxRot);	//���[���h�s�񁁃��[���h�s�񁖉�]�s��

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pParticle->pos.x, pParticle->pos.y, pParticle->pos.z);

			D3DXMatrixMultiply(&pParticle->mtxWorld, &pParticle->mtxWorld, &mtxTrans);	//���[���h�s�񁁃��[���h�s�񁖈ړ��s��

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pParticle->mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
									nCntParticle * 4,		//�J�n����C���f�b�N�X
									2);					//�v���~�e�B�u�̐�(�|���S���̐�)
		}
	}
	//�ʏ탂�[�h�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * PARTICLE_MAX,	//�m�ۂ���o�b�t�@�T�C�Y(�T�C�Y*���_��)
								D3DUSAGE_WRITEONLY,		//�Œ�
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,		//�Œ�
								&g_pVtxBuffParticle,
								NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		//���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// �p�[�e�B�N���̐ݒ�
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nLife = nLife;
			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);	//������Ƃ��̓��b�N�������Ă���I

			pVtx += nCntParticle * 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

			//���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			//���_�f�[�^���A�����b�N
			g_pVtxBuffParticle->Unlock();

			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}