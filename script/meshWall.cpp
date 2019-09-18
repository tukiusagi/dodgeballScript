//=============================================================================
//
// ���b�V���E�H�[������ [meshWall.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "meshWall.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0	"data/TEXTURE/field000.jpg"				// �ǂݍ��ރe�N�X�`��
#define MAX_TEXTURE		(1)										// �e�N�X�`����
#define POLYFON_WIDTH	(1900)									// �ǂ̕�
#define POLYGON_HEIGHT	(100)									// �ǂ̍���
#define MAX_WALL		(4)										// �ǂ̐�

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_apTextureMeshWall[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^

MESHWALL			g_MeshWall[MAX_WALL];					// ���b�V���E�H�[���̏��

//=============================================================================
// ����������
//=============================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_apTextureMeshWall[0]);

	// �����ݒ�
	g_MeshWall[0].pos = D3DXVECTOR3(0.0f, 0.0f, 950.0f);
	g_MeshWall[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshWall[0].nNumWidth = 40;
	g_MeshWall[0].nNumHeight = 40;

	g_MeshWall[1].pos = D3DXVECTOR3(950.0f, 0.0f, 0.0f);
	g_MeshWall[1].rot = D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f);
	g_MeshWall[1].nNumWidth = 40;
	g_MeshWall[1].nNumHeight = 40;

	g_MeshWall[2].pos = D3DXVECTOR3(-950.0f, 0.0f, 0.0f);
	g_MeshWall[2].rot = D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f);
	g_MeshWall[2].nNumWidth = 40;
	g_MeshWall[2].nNumHeight = 40;

	g_MeshWall[3].pos = D3DXVECTOR3(0.0f, 0.0f, -950.0f);
	g_MeshWall[3].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_MeshWall[3].nNumWidth = 40;
	g_MeshWall[3].nNumHeight = 40;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// �����_��
		g_MeshWall[nCntWall].nNumVertex = (g_MeshWall[nCntWall].nNumWidth + 1)*(g_MeshWall[nCntWall].nNumHeight + 1);

		// ���|���S����
		g_MeshWall[nCntWall].nNumPolygon = (g_MeshWall[nCntWall].nNumWidth*g_MeshWall[nCntWall].nNumHeight * 2) + ((g_MeshWall[nCntWall].nNumHeight - 1) * 4);

		// ���C���f�b�N�X��
		g_MeshWall[nCntWall].nNumIndex = g_MeshWall[nCntWall].nNumPolygon + 2;

		// 1������̃|���S���̑傫��
		g_MeshWall[nCntWall].sizePolygon = D3DXVECTOR3(1.0f*POLYFON_WIDTH / g_MeshWall[nCntWall].nNumWidth, 0.0f,
			1.0f*POLYGON_HEIGHT / g_MeshWall[nCntWall].nNumHeight);

		// 1������̃e�N�X�`�����W
		g_MeshWall[nCntWall].texPolygon = D3DXVECTOR2(10.0f / g_MeshWall[nCntWall].nNumWidth, 1.0f / g_MeshWall[nCntWall].nNumHeight);
	}
	// ���_���̍쐬
	MakeVertexMeshWall(pDevice);

	// �ǂ̐ݒu
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		SetMeshWall(g_MeshWall[nCntWall].pos, g_MeshWall[nCntWall].rot);
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	// �e�N�X�`���̊J��
	if (g_apTextureMeshWall[0] != NULL)
	{
		g_apTextureMeshWall[0]->Release();
		g_apTextureMeshWall[0] = NULL;
	}

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// �C���f�b�N�X�o�b�t�@�̉��
		if (g_MeshWall[nCntWall].pIdxBuffer)
		{
			g_MeshWall[nCntWall].pIdxBuffer->Release();
			g_MeshWall[nCntWall].pIdxBuffer = NULL;
		}

		// ���_�o�b�t�@�̊J��
		if (g_MeshWall[nCntWall].pVtxBuffer != NULL)
		{
			g_MeshWall[nCntWall].pVtxBuffer->Release();
			g_MeshWall[nCntWall].pVtxBuffer = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	MESHWALL *pMeshWall;						// ���b�V���E�H�[�����ւ̃|�C���^

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// ���ʃJ�����O����

	pMeshWall = &g_MeshWall[0];
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++,pMeshWall++)
	{
		if (pMeshWall->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_MeshWall[nCntWall].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshWall[nCntWall].rot.y, g_MeshWall[nCntWall].rot.x, g_MeshWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_MeshWall[nCntWall].mtxWorld, &g_MeshWall[nCntWall].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_MeshWall[nCntWall].pos.x, g_MeshWall[nCntWall].pos.y, g_MeshWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_MeshWall[nCntWall].mtxWorld, &g_MeshWall[nCntWall].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCntWall].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_MeshWall[nCntWall].pVtxBuffer, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_MeshWall[nCntWall].pIdxBuffer);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshWall[0]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshWall[nCntWall].nNumVertex, 0, g_MeshWall[nCntWall].nNumPolygon);
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// �J�����O�����ɖ߂�
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshWall[nCntWall].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_MeshWall[nCntWall].pVtxBuffer,
			NULL);

		// �C���f�b�N�X�o�b�t�@�𐶐�
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_MeshWall[nCntWall].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_MeshWall[nCntWall].pIdxBuffer,
			NULL);

		VERTEX_3D *pVtx;

		// ���_���̐ݒ�
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_MeshWall[nCntWall].pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < g_MeshWall[nCntWall].nNumHeight + 1; nCntV++)
		{
			for (int nCntH = 0; nCntH < g_MeshWall[nCntWall].nNumWidth + 1; nCntH++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntH + nCntV * (g_MeshWall[nCntWall].nNumWidth + 1)].pos =
					D3DXVECTOR3(-POLYFON_WIDTH / 2 + g_MeshWall[nCntWall].sizePolygon.x*nCntH,
					POLYGON_HEIGHT - g_MeshWall[nCntWall].sizePolygon.z*nCntV, 0.0f);

				// �@���x�N�g���̐ݒ�
				pVtx[nCntH + nCntV * (g_MeshWall[nCntWall].nNumWidth + 1)].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���_�J���[�̐ݒ�
				pVtx[nCntH + nCntV * (g_MeshWall[nCntWall].nNumWidth + 1)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntH + nCntV * (g_MeshWall[nCntWall].nNumWidth + 1)].tex = D3DXVECTOR2(g_MeshWall[nCntWall].texPolygon.x*nCntH,
					g_MeshWall[nCntWall].texPolygon.y*nCntV);
			}
		}
		// ���_�f�[�^���A�����b�N����
		g_MeshWall[nCntWall].pVtxBuffer->Unlock();

		WORD *pIdx;// �C���f�b�N�X�f�[�^�ւ̃|�C���^

		// �C���f�b�N�X�̐ݒ�
		// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
		g_MeshWall[nCntWall].pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		int nNum = 0;
		int nNumCnt = 0;

		// �C���f�b�N�X
		for (int nCntV = 0; nCntV < g_MeshWall[nCntWall].nNumHeight; nCntV++)
		{
			if (nCntV != 0)
			{
				pIdx[nNum] = nNumCnt - 1 + g_MeshWall[nCntWall].nNumWidth - 1 + 3;
				nNum++;
			}

			for (int nCntH = 0; nCntH < g_MeshWall[nCntWall].nNumWidth + 1; nCntH++, nNumCnt++, nNum += 2)
			{
				pIdx[nNum] = nNumCnt + g_MeshWall[nCntWall].nNumWidth + 1;
				pIdx[nNum + 1] = nNumCnt;
			}

			if (nCntV != g_MeshWall[nCntWall].nNumHeight - 1)
			{
				pIdx[nNum] = nNumCnt - 1;
				nNum++;
			}
		}
		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		g_MeshWall[nCntWall].pIdxBuffer->Unlock();
	}
}

//=============================================================================
// ���b�V���E�H�[���̐ݒ�
//=============================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (!g_MeshWall[nCntWall].bUse)
		{
			g_MeshWall[nCntWall].pos = pos;
			g_MeshWall[nCntWall].rot = rot;
			g_MeshWall[nCntWall].bUse = true;
			break;
		}
	}
}