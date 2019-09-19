//=============================================================================
//
// ������ [field.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "field.h"

#include "keyboard.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0	"data/TEXTURE/.jpg"					// �ǂݍ��ރe�N�X�`��1
#define MAX_TEXTURE		(1)										// �e�N�X�`����
#define MAX_FIELD		(128)									// �ǂ̐�
#define FIELD_WIDTH		(500)									// �ǂ̕�
#define FIELD_HEIGHT	(500)									// �ǂ̍���

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3	pos;		// �ʒu
	D3DXVECTOR3	rot;		// ����(��])
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	bool bUse;				// �ǂ��g�p���Ă��邩
	int nIdxPlayer;			// ����v���C���[�̔ԍ�
} FIELD;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice);
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_apTextureField[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

FIELD					g_aField[MAX_FIELD];				// �ǂ̏��

//=============================================================================
// ����������
//=============================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �����ݒ�
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_aField[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCntField].bUse = false;
		g_aField[nCntField].nIdxPlayer = nCntField;
	}

	// ���_���̍쐬
	MakeVertexField(pDevice);

	// �ǂ̐ݒu
	SetField(D3DXVECTOR3(-250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	SetField(D3DXVECTOR3(250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	SetField(D3DXVECTOR3(-250.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	SetField(D3DXVECTOR3(250.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_apTextureField[nCntTex] != NULL)
		{
			g_apTextureField[nCntTex]->Release();
			g_apTextureField[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	FIELD *pField = &g_aField[0];

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pField++)
	{
		if (!pField->bUse) { continue; }
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pField->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pField->rot.y, pField->rot.x, pField->rot.z);
		D3DXMatrixMultiply(&pField->mtxWorld, &pField->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pField->pos.x, pField->pos.y, pField->pos.z);
		D3DXMatrixMultiply(&pField->mtxWorld, &pField->mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pField->mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureField[0]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntField * 4, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_apTextureField[0]);

	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pVtx += 4)
	{
		// ���W�̐ݒ�
		pVtx[0].pos = pVtx[1].pos = pVtx[2].pos = pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffField->Unlock();
}

//=============================================================================
// �ǂ̐ݒ�
//=============================================================================
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pVtx += 4)
	{
		if (g_aField[nCntField].bUse) { continue; }
		g_aField[nCntField].pos = pos;
		g_aField[nCntField].rot = rot;
		g_aField[nCntField].bUse = true;

		// ���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-FIELD_WIDTH / 2, 0.0f, +FIELD_HEIGHT/2);
		pVtx[1].pos = D3DXVECTOR3(+FIELD_WIDTH / 2, 0.0f, +FIELD_HEIGHT/2);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_WIDTH / 2, 0.0f, -FIELD_HEIGHT/2);
		pVtx[3].pos = D3DXVECTOR3(+FIELD_WIDTH / 2, 0.0f, -FIELD_HEIGHT/2);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(3.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(3.0f, 1.0f);
		break;
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffField->Unlock();
}

//=============================================================================
// ���̐F�̕ύX
//=============================================================================
void ChangeFieldColor(int nGiveNum, int nReceiveNum)
{
	VERTEX_3D *pVtx;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	switch (nGiveNum)
	{
	case 0:
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 1:
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 2:
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 3:
		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < 4; nCntField++, pVtx += 4)
	{
		if (g_aField[nCntField].nIdxPlayer == nReceiveNum)
		{
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;
			g_aField[nCntField].nIdxPlayer = nGiveNum;
		}
	}

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffField->Unlock();
}

//=============================================================================
// ���Ƃ̐ڐG����
//=============================================================================
void CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size, int nCol)
{
	for (int nCntField = 0; nCntField < 4; nCntField++)
	{
		if (g_aField[nCntField].nIdxPlayer == nCol) // �v���C���[�Ɠ����F�̏ꍇ�̓X�L�b�v����
		{
			bool bA = false;

			if (g_aField[1].nIdxPlayer == g_aField[2].nIdxPlayer &&
				(nCntField == 1 || nCntField == 2))
			{
				bA = true;
			}
			else if (g_aField[0].nIdxPlayer == g_aField[3].nIdxPlayer &&
				(nCntField == 0 || nCntField == 3))
			{
				bA = true;
			}

			if (!bA)
				continue;

			bool bUp = false;

			if (pPos->x + size.x > -10.0f&&
				pPos->x - size.x < +10.0f)
			{// �㉺
				if (pPos->z - size.z <= 10.0f&&
					pPosOld->z - size.z >= 10.0f)
				{// �ォ��Ԃ������ꍇ
					bUp = true;
				}
				else if (pPos->z + size.z >= -10.0f&&
					pPosOld->z + size.z <= -10.0f)
				{// ������Ԃ������ꍇ
					bUp = true;
				}
			}
			if (pPos->z + size.x > -10.0f&&
				pPos->z - size.x < +10.0f)
			{// ���E
				if (pPos->x - size.x <= +10.0f&&
					pPosOld->x - size.x >= +10.0f)
				{// �E����Ԃ������ꍇ
					bUp = true;
				}
				else if (pPos->x + size.x >= -10.0f&&
					pPosOld->x + size.x <= -10.0f)
				{// ������Ԃ������ꍇ
					bUp = true;
				}
			}

			if(bUp)
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				if (nCol == g_aField[nCnt].nIdxPlayer && 
					((pPos->x < 0.0f) ? (pPos->z < 0.0f) ? nCnt == nCntField : nCnt != nCntField : (pPos->z < 0.0f) ? nCnt == nCntField : nCnt != nCntField))
				{
					D3DXVec3Normalize(pPos, &g_aField[nCnt].pos);
					*pPos *= 71;

					break;
				}
			}

			continue;
		}
		
		if (pPos->x + size.x > g_aField[nCntField].pos.x - 250.0f&&
			pPos->x - size.x < g_aField[nCntField].pos.x + 250.0f)
		{// �㉺
			if (pPos->z - size.z <= g_aField[nCntField].pos.z + 250.0f&&
				pPosOld->z - size.z >= g_aField[nCntField].pos.z + 250.0f)
			{// �ォ��Ԃ������ꍇ
				pPos->z = g_aField[nCntField].pos.z + 250.0f + size.z;
			}
			else if (pPos->z + size.z >= g_aField[nCntField].pos.z - 250.0f&&
				pPosOld->z + size.z <= g_aField[nCntField].pos.z - 250.0f)
			{// ������Ԃ������ꍇ
				pPos->z = g_aField[nCntField].pos.z - 250.0f - size.z;
			}
		}

		if (pPos->z + size.x > g_aField[nCntField].pos.z - 250.0f&&
			pPos->z - size.x < g_aField[nCntField].pos.z + 250.0f)
		{// ���E
			if (pPos->x - size.x <= g_aField[nCntField].pos.x + 250.0f&&
				pPosOld->x - size.x >= g_aField[nCntField].pos.x + 250.0f)
			{// �E����Ԃ������ꍇ
				pPos->x = g_aField[nCntField].pos.x + 250.0f + size.x;
			}
			else if (pPos->x + size.x >= g_aField[nCntField].pos.x - 250.0f&&
				pPosOld->x + size.x <= g_aField[nCntField].pos.x - 250.0f)
			{// ������Ԃ������ꍇ
				pPos->x = g_aField[nCntField].pos.x - 250.0f - size.x;
			}
		}
	}
}
