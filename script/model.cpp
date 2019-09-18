//=============================================================================
//
// ���f������ [model.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "model.h"
#include "camera.h"
#include "shadow.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MODELFILE0		"data/MODEL/bill_00.x"					// �ǂݍ��ރ��f��
#define MODELFILE1		"data/MODEL/bill_01.x"					// �ǂݍ��ރ��f��
#define MODELFILE2		"data/MODEL/bill_02.x"					// �ǂݍ��ރ��f��
#define MODELFILE3		"data/MODEL/bill_03.x"					// �ǂݍ��ރ��f��
#define MODELFILE4		"data/MODEL/bill_04.x"					// �ǂݍ��ރ��f��

#define MODELFILE5		"data/MODEL/10���~.x"					// �ǂݍ��ރ��f��
#define MODELFILE6		"data/MODEL/30���~.x"					// �ǂݍ��ރ��f��
#define MODELFILE7		"data/MODEL/50���~.x"					// �ǂݍ��ރ��f��
#define MODELFILE8		"data/MODEL/100���~.x"					// �ǂݍ��ރ��f��

#define MODELFILE9		"data/MODEL/tikinnresu.x"				// �ǂݍ��ރ��f��

#define MODELFILE10		"data/MODEL/mart.x"						// �ǂݍ��ރ��f��
#define MODELFILE11		"data/MODEL/Wac.x"						// �ǂݍ��ރ��f��

#define TEXTUREFILE0	"data/TEXTURE/bill_window.png"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE1	"data/TEXTURE/auto_door.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE2	"data/TEXTURE/bill_rooftop.png"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE3	"data/TEXTURE/bill_window1.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE4	"data/TEXTURE/bill_window2.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE5	"data/TEXTURE/SH028_L.jpg"				// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE6	"data/TEXTURE/SH033_L.jpg"				// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE7	"data/TEXTURE/bill_window3.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE8	"data/TEXTURE/bill_window4.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE9	"data/TEXTURE/bill_wall.png"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE10	"data/TEXTURE/Signboard.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE11	"data/TEXTURE/Signboard10.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE12	"data/TEXTURE/Signboard30.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE13	"data/TEXTURE/Signboard50.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE14	"data/TEXTURE/Signboard100.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE15	"data/TEXTURE/chicken_race0.jpg"		// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE16	"data/TEXTURE/chicken_race1.jpg"		// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE17	"data/TEXTURE/convenience0.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE18	"data/TEXTURE/convenience1.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE19	"data/TEXTURE/conveni_signboard.jpg"	// �ǂݍ��ރe�N�X�`��
#define MAX_TEXTURE		(21)									// �e�N�X�`����
#define MAX_SET_TEXTURE	(20)									// �ݒu�e�N�X�`����
#define MAX_MODEL		(128)									// ���f���̐�

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		move;						// �ړ���
	D3DXVECTOR3		rot;						// ����(��])
	D3DXMATRIX		mtxWorld;					// ���[���h�}�g���b�N�X
	bool			bUse;						// �g�p���Ă��邩
	int				nIdxShadow;					// �e��ID
	MODELTYPE		type;						// ���
} MODEL;

typedef struct
{
	DWORD			nNumMat;					// �}�e���A�����̐�
	LPD3DXMESH		pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMat;					// �}�e���A�����ւ̃|�C���^
	D3DXVECTOR3		vtxMin;						// �ő�l
	D3DXVECTOR3		vtxMax;						// �ŏ��l
} MODEL_MODEL;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_pTextureModel[MAX_TEXTURE] = {};			// �e�N�X�`���ւ̃|�C���^

MODEL				g_aModel[MAX_MODEL];						// �v���C���[�̏��			
MODEL_MODEL			g_aModelModel[MODELTYPE_MAX];

//=============================================================================
// ����������
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �����ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
	{
		g_aModel[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntEnemy].bUse = false;
		g_aModel[nCntEnemy].type = MODELTYPE_BILL0;
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODELFILE0, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[0].pBuffMat, NULL,
		&g_aModelModel[0].nNumMat,
		&g_aModelModel[0].pMesh);

	D3DXLoadMeshFromX(MODELFILE1, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[1].pBuffMat, NULL,
		&g_aModelModel[1].nNumMat,
		&g_aModelModel[1].pMesh);

	D3DXLoadMeshFromX(MODELFILE2, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[2].pBuffMat, NULL,
		&g_aModelModel[2].nNumMat,
		&g_aModelModel[2].pMesh);

	D3DXLoadMeshFromX(MODELFILE3, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[3].pBuffMat, NULL,
		&g_aModelModel[3].nNumMat,
		&g_aModelModel[3].pMesh);

	D3DXLoadMeshFromX(MODELFILE4, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[4].pBuffMat, NULL,
		&g_aModelModel[4].nNumMat,
		&g_aModelModel[4].pMesh);

	D3DXLoadMeshFromX(MODELFILE5, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[5].pBuffMat, NULL,
		&g_aModelModel[5].nNumMat,
		&g_aModelModel[5].pMesh);

	D3DXLoadMeshFromX(MODELFILE6, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[6].pBuffMat, NULL,
		&g_aModelModel[6].nNumMat,
		&g_aModelModel[6].pMesh);

	D3DXLoadMeshFromX(MODELFILE7, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[7].pBuffMat, NULL,
		&g_aModelModel[7].nNumMat,
		&g_aModelModel[7].pMesh);

	D3DXLoadMeshFromX(MODELFILE8, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[8].pBuffMat, NULL,
		&g_aModelModel[8].nNumMat,
		&g_aModelModel[8].pMesh);
	
	D3DXLoadMeshFromX(MODELFILE9, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[9].pBuffMat, NULL,
		&g_aModelModel[9].nNumMat,
		&g_aModelModel[9].pMesh);

	D3DXLoadMeshFromX(MODELFILE10, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[10].pBuffMat, NULL,
		&g_aModelModel[10].nNumMat,
		&g_aModelModel[10].pMesh);

	D3DXLoadMeshFromX(MODELFILE11, D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aModelModel[11].pBuffMat, NULL,
		&g_aModelModel[11].nNumMat,
		&g_aModelModel[11].pMesh);

	// ���f���̒��_���̍ő�l�E�ŏ��l�̐ݒ�
	int		nNumVertices;							// ���_��
	DWORD	sizeFVF;								// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVertexBuffer;

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		g_aModelModel[nCntModel].vtxMax= D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
		g_aModelModel[nCntModel].vtxMin= D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

		// ���_�����擾
		nNumVertices = g_aModelModel[nCntModel].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aModelModel[nCntModel].pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_aModelModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// �S�Ă̒��_�����r���čő�l�E�ŏ��l�𔲂��o��
			if (g_aModelModel[nCntModel].vtxMax.x < vtx.x)
			{
				g_aModelModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (g_aModelModel[nCntModel].vtxMin.x > vtx.x)
			{
				g_aModelModel[nCntModel].vtxMin.x = vtx.x;
			}

			if (g_aModelModel[nCntModel].vtxMax.y < vtx.y)
			{
				g_aModelModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (g_aModelModel[nCntModel].vtxMin.y > vtx.y)
			{
				g_aModelModel[nCntModel].vtxMin.y = vtx.y;
			}

			if (g_aModelModel[nCntModel].vtxMax.z < vtx.z)
			{
				g_aModelModel[nCntModel].vtxMax.z = vtx.z;
			}
			if (g_aModelModel[nCntModel].vtxMin.z > vtx.z)
			{
				g_aModelModel[nCntModel].vtxMin.z = vtx.z;
			}
			pVertexBuffer += sizeFVF;//	�T�C�Y���|�C���^��i�߂�
		}
		// �A�����b�N
		g_aModelModel[nCntModel].pMesh->UnlockVertexBuffer();
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_pTextureModel[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE1, &g_pTextureModel[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE2, &g_pTextureModel[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE3, &g_pTextureModel[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE4, &g_pTextureModel[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE5, &g_pTextureModel[5]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE6, &g_pTextureModel[6]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE7, &g_pTextureModel[7]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE8, &g_pTextureModel[8]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE9, &g_pTextureModel[9]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE10, &g_pTextureModel[10]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE11, &g_pTextureModel[11]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE12, &g_pTextureModel[12]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE13, &g_pTextureModel[13]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE14, &g_pTextureModel[14]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE15, &g_pTextureModel[15]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE16, &g_pTextureModel[16]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE17, &g_pTextureModel[17]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE18, &g_pTextureModel[18]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE19, &g_pTextureModel[19]);
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	// ���b�V���̊J��
	for (int nCntMesh = 0; nCntMesh < MODELTYPE_MAX; nCntMesh++)
	{
		if (g_aModelModel[nCntMesh].pMesh != NULL)
		{
			g_aModelModel[nCntMesh].pMesh->Release();
			g_aModelModel[nCntMesh].pMesh = NULL;
		}
	}
	
	// �}�e���A���̊J��
	for (int nCntMat = 0; nCntMat < MODELTYPE_MAX; nCntMat++)
	{
		if (g_aModelModel[nCntMat].pBuffMat != NULL)
		{
			g_aModelModel[nCntMat].pBuffMat->Release();
			g_aModelModel[nCntMat].pBuffMat = NULL;
		}
	}
	
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_SET_TEXTURE; nCntTex++)
	{
		if (g_pTextureModel[nCntTex] != NULL)
		{
			g_pTextureModel[nCntTex]->Release();
			g_pTextureModel[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL *pMat;							// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;						// �}�e���A���f�[�^�ւ̃|�C���^
	MODEL *pModel;

	pModel = &g_aModel[0];
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse)// �g�p����Ă���ꍇ
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);
	
			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModelModel[g_aModel[nCntModel].type].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelModel[g_aModel[nCntModel].type].nNumMat; nCntMat++)
			{// �e�N�X�`���̐ݒ�
				switch (g_aModel[nCntModel].type)
				{
				case MODELTYPE_BILL0:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[0]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[1]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[2]);
					}
					break;

				case MODELTYPE_BILL1:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[3]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[1]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[2]);
					}
					break;

				case MODELTYPE_BILL2:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[4]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[5]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[6]);
					}
					break;

				case MODELTYPE_BILL3:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[7]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[1]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[2]);
					}
					break;

				case MODELTYPE_BILL4:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[8]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[9]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[2]);
					}
					break;

				case MODELTYPE_10_BOARD:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[17]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[10]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[11]);
					}
					break;

				case MODELTYPE_30_BOARD:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[17]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[10]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[12]);
					}
					break;

				case MODELTYPE_50_BOARD:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[17]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[10]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[13]);
					}
					break;

				case MODELTYPE_100_BOARD:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[17]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[10]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[14]);
					}
					break;

				case MODELTYPE_CLIFF:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[15]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[16]);
					}
					break;

				case MODELTYPE_CONVENI:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[1]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[17]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[18]);
					}
					else if (nCntMat == 3)
					{
						pDevice->SetTexture(0, g_pTextureModel[19]);
					}
					else if (nCntMat == 4)
					{
						// �e�N�X�`���̐ݒ�(
						pDevice->SetTexture(0, g_pTextureModel[20]);
					}
					break;

				case MODELTYPE_WAC:
					if (nCntMat == 0)
					{
						pDevice->SetTexture(0, g_pTextureModel[0]);
					}
					else if (nCntMat == 1)
					{
						pDevice->SetTexture(0, g_pTextureModel[1]);
					}
					else if (nCntMat == 2)
					{
						pDevice->SetTexture(0, g_pTextureModel[18]);
					}
					else if (nCntMat == 3)
					{
						pDevice->SetTexture(0, g_pTextureModel[2]);
					}
					else if (nCntMat == 4)
					{
						// �e�N�X�`���̐ݒ�(
						pDevice->SetTexture(0, g_pTextureModel[20]);
					}
					break;
				}
				
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �`��
				g_aModelModel[g_aModel[nCntModel].type].pMesh->DrawSubset(nCntMat);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// ���f���̐ݒ�
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE type)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_MODEL; nCntEnemy++)
	{
		if (!g_aModel[nCntEnemy].bUse)// �G���g�p����Ă��Ȃ��ꍇ
		{
			g_aModel[nCntEnemy].pos = pos;
			g_aModel[nCntEnemy].rot = rot;
			g_aModel[nCntEnemy].type = type;
			g_aModel[nCntEnemy].bUse = true;

			//// �e�̍쐬
			//g_aModel[nCntEnemy].nIdxShadow = SetShadow(g_aModel[nCntEnemy].pos, g_aModel[nCntEnemy].rot,
			//	D3DXVECTOR3(g_aModelModel[g_aModel[nCntEnemy].type].vtxMax.x- g_aModelModel[g_aModel[nCntEnemy].type].vtxMin.x,
			//		0.0f, g_aModelModel[g_aModel[nCntEnemy].type].vtxMax.z - g_aModelModel[g_aModel[nCntEnemy].type].vtxMin.z));

			break;
		}
	}
}

//=============================================================================
// ���f���Ƃ̓����蔻��
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size)
{
	bool bRand = false;// ���n����

	// �����蔻��
	for (int nCountBlock = 0; nCountBlock < MAX_MODEL; nCountBlock++)
	{
		if (g_aModel[nCountBlock].bUse)// �u���b�N���g�p����Ă���ꍇ
		{
			if (pPos->y <= g_aModel[nCountBlock].pos.y + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.y&&
				pPosOld->y >= g_aModel[nCountBlock].pos.y + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.y)
			{// ����Ă��邩
				if (pPos->x + size.x > g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.x&&
					pPos->x - size.x < g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.x&&
					pPos->z + size.z > g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.z&&
					pPos->z - size.z < g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.z)
				{// X,Z���W���͈͓��̏ꍇ
					pPos->y = g_aModel[nCountBlock].pos.y + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.y;
					bRand = true;
				}
			}
			else if (pPos->y + size.y > g_aModel[nCountBlock].pos.y + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.y &&
				pPos->y < g_aModel[nCountBlock].pos.y + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.y)
			{// Y���W���͈͓��̏ꍇ
				if (pPos->x + size.x > g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.x&&
					pPos->x - size.x < g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.x)
				{// �㉺
					if (pPos->z - size.z <= g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.z&&
						pPosOld->z - size.z >= g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.z)
					{// �v���C���[���u���b�N�ɏォ��Ԃ�����
						pPos->z = size.z + g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.z;
					}
					else if (pPos->z + size.z >= g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.z&&
						pPosOld->z + size.z <= g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.z)
					{// �v���C���[���u���b�N�ɉ�����Ԃ�����
						pPos->z = g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.z - size.z;
					}
				}

				if (pPos->z + size.z > g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.z&&
					pPos->z - size.z < g_aModel[nCountBlock].pos.z + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.z)
				{// ���E
					if (pPos->x - size.x <= g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.x&&
						pPosOld->x - size.x >= g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.x)
					{// �v���C���[���u���b�N�ɉE����Ԃ�����
						pPos->x = size.x + g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMax.x;
					}
					else if (pPos->x + size.x >= g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.x&&
						pPosOld->x + size.x <= g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.x)
					{// �v���C���[���u���b�N�ɍ�����Ԃ�����
						pPos->x = g_aModel[nCountBlock].pos.x + g_aModelModel[g_aModel[nCountBlock].type].vtxMin.x - size.x;
					}
				}
			}
		}
	}

	return bRand;
}

//=============================================================================
// ���f���̏ꏊ���̎擾����
//=============================================================================
D3DXVECTOR3 *GetModelPos(void)
{
	return&g_aModel[0].pos;
}