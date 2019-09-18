//=============================================================================
//
// �{�[������ [ball.cpp]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#include "ball.h"
#include "player.h"
#include "game.h"
#include "shadow.h"
#include "fade.h"
#include "meshwall.h"
#include "meshfield.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BALL_MODEL_FILE "data/MODEL/ball.x"			//�ǂݍ��݃��f���t�@�C����[�A�q��]
#define ATTACK_SPEED	(0.01)		// �U�����肪���鑬�x
#define BALL_SHADOW	(5.0f)			//�e�̑傫��
#define BALL_SPEED	(30.0f)			//�{�[���𓊂��鑬��
#define BALL_INERTIA (0.03f)		//�{�[���̊���

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MoveLimitBall(D3DXVECTOR3 *pos, float fRadius);	//�ړ�����

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPD3DXMESH			g_pMeshBall;			//���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pBuffMatBall;		//�}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatBall;		//�}�e���A�����̐�

LPDIRECT3DTEXTURE9	g_pTextureBall = NULL;				//�e�N�X�`���ւ̃|�C���^

BALL				g_aBall;					//�{�[���̏��

//=============================================================================
// ����������
//=============================================================================
void InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����̏����ݒ�
	g_aBall.pos = D3DXVECTOR3(-100.0f, 50.0f, 0.0f);
	g_aBall.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBall.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBall.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_aBall.fRadius = BALL_RADIUS;
	g_aBall.state = BALLSTATE_NORMAL;

	//�e�̍쐬
	g_aBall.nIdxShadow = SetShadow(D3DXVECTOR3(g_aBall.pos.x, g_aBall.pos.y, g_aBall.pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(BALL_MODEL_FILE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBall,
		NULL,
		&g_nNumMatBall,
		&g_pMeshBall);


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureBall);
}

//=============================================================================
// �I������
//=============================================================================
void UninitBall(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureBall != NULL)
	{
		g_pTextureBall->Release();
		g_pTextureBall = NULL;
	}

	// ���b�V���̊J��
	if (g_pMeshBall != NULL)
	{
		g_pMeshBall->Release();
		g_pMeshBall = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatBall != NULL)
	{
		g_pBuffMatBall->Release();
		g_pBuffMatBall = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBall(void)
{
	if (g_aBall.state == BALLSTATE_THROW)
	{
		if (g_aBall.move.x <= ATTACK_SPEED && g_aBall.move.z <= ATTACK_SPEED && 
			g_aBall.move.x >= -ATTACK_SPEED && g_aBall.move.z >= -ATTACK_SPEED)
		{
			g_aBall.state = BALLSTATE_NORMAL;
		}
	}

	// ����(����)
	g_aBall.move.x += (0 - g_aBall.move.x)*BALL_INERTIA;
	g_aBall.move.y += (0 - g_aBall.move.y)*BALL_INERTIA;
	g_aBall.move.z += (0 - g_aBall.move.z)*BALL_INERTIA;

	g_aBall.pos += g_aBall.move;

	if (g_aBall.bHold)
	{	//������Ă�
		CatchBall(*GetPlayerPos(g_aBall.nParent), *GetPlayerMove(g_aBall.nParent), *GetPlayerRadius(g_aBall.nParent));
	}

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_aBall.nIdxShadow, g_aBall.pos);

	//�d��
	//g_aBall.move.y -= 0.08f;

	MoveLimitBall(&g_aBall.pos, g_aBall.fRadius);

	if (g_aBall.pos.y < 0)
	{	//�n�ʂɂ߂荞�܂Ȃ��悤��
		g_aBall.pos.y = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �X�P�[���𔽉f
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p
	D3DXMATERIAL *pMat;						//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;					//�}�e���A���̃|�C���^

		// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aBall.mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBall.rot.y, g_aBall.rot.x, g_aBall.rot.z);
	D3DXMatrixMultiply(&g_aBall.mtxWorld, &g_aBall.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aBall.pos.x, g_aBall.pos.y, g_aBall.pos.z);
	D3DXMatrixMultiply(&g_aBall.mtxWorld, &g_aBall.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aBall.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatBall->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatBall; nCntMat++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBall);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �`��
		g_pMeshBall->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �{�[������
//=============================================================================
void CatchBall(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius)
{
	float fLength = (fRadius + g_aBall.fRadius);
	D3DXVECTOR3 vec = move;	//�v���C���[�̈ړ������x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	//�{�[����̂̑O��
	if (vec.x >= 0.01f || vec.z >= 0.01f || vec.x <= -0.01f || vec.z <= -0.01f)
	{	//���x���鎞����
		g_aBall.pos.x = pos.x + vec.x * fLength;
		g_aBall.pos.z = pos.z + vec.z * fLength;
	}

	//�ꏏ�ɓ���
	g_aBall.move = move;
}

//=============================================================================
// �{�[��������
//=============================================================================
void ThrowBall(D3DXVECTOR3 move)
{
	D3DXVECTOR3 vec = move;	//�v���C���[�̈ړ������x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	//�����Ă�����ɓ�����
	g_aBall.move = vec * BALL_SPEED;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionBall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fRadius)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_aBall.pos.x)*(pPos->x - g_aBall.pos.x) + (pPos->z - g_aBall.pos.z)*(pPos->z - g_aBall.pos.z);	//2�_�Ԃ̋���
	float fLength = (fRadius + g_aBall.fRadius)*(fRadius + g_aBall.fRadius);	//���a
	float fDiff = (fRadius + g_aBall.fRadius) - (float)sqrt(fDistance);	//��
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_aBall.pos.x), 0.0f, (pPos->z - g_aBall.pos.z));	//�x�N�g��
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		bLand = true;
		pPos->x = pPosOld->x + vec.x * fDiff;
		pPos->z = pPosOld->z + vec.z * fDiff;

	}
	return bLand;
}

//=============================================================================
//	�ړ�����
//=============================================================================
void MoveLimitBall(D3DXVECTOR3 *pos, float fRadius)
{
	//���E
	if (pos->x > MESHFIELD_SIZE * 2 - fRadius)
	{
		pos->x = MESHFIELD_SIZE * 2 - fRadius;
	}
	if (pos->x < -MESHFIELD_SIZE * 2 + fRadius)
	{
		pos->x = -MESHFIELD_SIZE * 2 + fRadius;
	}

	//����O
	if (pos->z > MESHFIELD_SIZE * 2 - fRadius)
	{
		pos->z = MESHFIELD_SIZE * 2 - fRadius;
	}
	if (pos->z < -MESHFIELD_SIZE * 2 + fRadius)
	{
		pos->z = -MESHFIELD_SIZE * 2 + fRadius;
	}
}

//=============================================================================
// �{�[���̏���
//=============================================================================
void DeleteBall(int nCntBall)
{
}

//=============================================================================
//	�{�[���̎擾
//=============================================================================
BALL *GetBall(void)
{
	return &g_aBall;
}