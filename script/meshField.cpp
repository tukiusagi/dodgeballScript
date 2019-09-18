//=============================================================================
//
// �|���S������ [meshField1.cpp]
// Author:RYO KANDA
//
//=============================================================================
#include "meshField.h"
#include "joypad.h"

//=============================================================================
// �\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3		pos;						// �l�p�`�̈ʒu

} POS;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureMeshField1 = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMeshField1 = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField1 = NULL;

D3DXVECTOR3	g_posMeshField1;			// �|���S���̈ʒu
D3DXVECTOR3	g_rotMeshField1;			// �|���S���̌���(��])
D3DXMATRIX	g_mtxWorldMeshField1;		// ���[���h�}�g���b�N�X

int	g_nNumVertexMeshField1;				// �����_
int g_nNumIndexMeshField1;				// ���C���f�b�N�X
int g_nNumPolygonMeshField1;			// ���|���S��

POS g_aPos[MESHFIELD_NUMLENGTH * MESHFIELD_NUMSIDE];

//=============================================================================
// ����������
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/yukiField.jpg", &g_pTextureMeshField1);

	for (int nCount = 0; nCount < MESHFIELD_NUMLENGTH; nCount++)
	{
		for (int nCnt = 0; nCnt < MESHFIELD_NUMSIDE; nCnt++)
		{
			g_aPos[MESHFIELD_NUMSIDE * nCount + nCnt].pos = D3DXVECTOR3(MESHFIELD_SIZE * (nCnt - MESHFIELD_NUMSIDE / 2.0f), 0.0f, MESHFIELD_SIZE * (MESHFIELD_NUMLENGTH / 2.0f - nCount));
		}
	}

	// �ʒu�E��]�̏����ݒ�
	g_posMeshField1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumVertexMeshField1 = (MESHFIELD_NUMLENGTH + 1) * (MESHFIELD_NUMSIDE + 1);
	g_nNumIndexMeshField1 = (MESHFIELD_NUMLENGTH - 1) * 2 + (MESHFIELD_NUMSIDE + 1) * 2 * MESHFIELD_NUMLENGTH;
	g_nNumPolygonMeshField1 = (MESHFIELD_NUMLENGTH - 1) * 4 + 2 * MESHFIELD_NUMLENGTH * MESHFIELD_NUMSIDE;

	// ���_���W
	MakeVertexMeshField(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField1 != NULL)
	{
		g_pVtxBuffMeshField1->Release();
		g_pVtxBuffMeshField1 = NULL;
	}

	// �C���f�b�N�X�̊J��
	if (g_pIdxBuffMeshField1 != NULL)
	{
		g_pIdxBuffMeshField1->Release();
		g_pIdxBuffMeshField1 = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureMeshField1 != NULL)
	{
		g_pTextureMeshField1->Release();
		g_pTextureMeshField1 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	D3DXMATRIX	mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField1);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField1.y, g_rotMeshField1.x, g_rotMeshField1.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField1, &g_mtxWorldMeshField1, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField1.x, g_posMeshField1.y, g_posMeshField1.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField1, &g_mtxWorldMeshField1, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField1);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField1, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField1);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField1);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshField1, 0, g_nNumPolygonMeshField1);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexMeshField1, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField1, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	g_pVtxBuffMeshField1->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̐ݒ�
	for (int nCount = 0; nCount < MESHFIELD_NUMLENGTH + 1; nCount++)
	{
		for (int nCnt = 0; nCnt < MESHFIELD_NUMSIDE + 1; nCnt++, pVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3(MESHFIELD_SIZE * (nCnt - MESHFIELD_NUMSIDE / 2.0f), 0.0f, MESHFIELD_SIZE * (MESHFIELD_NUMLENGTH / 2.0f - nCount));
		
			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ԁA�΁A�A�A���t�@�l

			// �e�N�X�`��(UV)���W
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCnt, 1.0f * nCount);
		}
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffMeshField1->Unlock();


	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshField1, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField1, NULL);

	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�o�b�t�@�̃|�C���^���擾
	g_pIdxBuffMeshField1->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int nCount = 0, nCnt = 0; nCount < g_nNumIndexMeshField1; nCount += 2)
	{
		if (nCount != 0 && (nCount / 2 - (nCnt / (MESHFIELD_NUMSIDE + 1) - 1)) % (MESHFIELD_NUMSIDE + 1) == 0 && nCnt % (MESHFIELD_NUMSIDE + 1) == 0)
		{// �k�ޕ�
			pIdx[nCount] = nCnt - 1;
			pIdx[nCount + 1] = nCnt + 1 + MESHFIELD_NUMSIDE;
		}
		else
		{
			pIdx[nCount] = (MESHFIELD_NUMSIDE + 1) + nCnt;
			pIdx[nCount + 1] = nCnt;

			nCnt++;
		}
	}

	//���_�f�[�^���A�����b�N
	g_pIdxBuffMeshField1->Unlock();
}

//=============================================================================
// ���̐F�̕ύX
//=============================================================================
void ChangeColor(D3DXCOLOR col, int nNumber)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	g_pVtxBuffMeshField1->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nNumber + nNumber / MESHFIELD_NUMSIDE;

	pVtx[0].col = col;	// �ԁA�΁A�A�A���t�@�l
	pVtx[1].col = col;	// �ԁA�΁A�A�A���t�@�l

	pVtx += MESHFIELD_NUMSIDE + 1;

	pVtx[0].col = col;	// �ԁA�΁A�A�A���t�@�l
	pVtx[1].col = col;	// �ԁA�΁A�A�A���t�@�l

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffMeshField1->Unlock();
}
