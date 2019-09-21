//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "player.h"
#include "camera.h"
#include "fade.h"
#include "field.h"
#include "game.h"
#include "joypad.h"
#include "keyboard.h"
#include "meshField.h"
#include "model.h"
#include "sound.h"
#include "shadow.h"
#include "time.h"
#include "ball.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_FILENAME		"data/LOAD_TEXT/motion_boy.txt"		//���f���t�@�C����
#define TEXTUREFILE0		"data/TEXTURE/red.jpg"				// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE1		"data/TEXTURE/blue.jpg"				// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE2		"data/TEXTURE/yellow.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE3		"data/TEXTURE/green.jpg"			// �ǂݍ��ރe�N�X�`��
#define MAX_TEXTURE			(5)									// �e�N�X�`����
#define MAX_USETEXTURE		(4)									// �g�p�e�N�X�`����
#define MAX_MODEL			(15)								// ���f���̐�
#define MAX_KEY				(8)									// ���f���̐�
#define PLAYER_MOVE			(0.4f)								// �v���C���[�̈ړ���
#define PLAYER_REST			(1475)								// �v���C���[�̈ړ��͈�
#define PLAYER_GRAVITY		(5)									// �d��
#define PLAYER_JUMP_MOVE	(11.25)								// �W�����v��

//=============================================================================
// ���[�V�����̎��
//=============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g����
	MOTIONTYPE_RUNNING,		// �����j���O
	MOTIONTYPE_ACTION,		// �A�N�V����
	MOTIONTYPE_JUMP,		// �W�����v
	MOTIONTYPE_LAND,		// �����h
	MOTIONTYPE_BLOWAWAY,	// �������
	MOTIONTYPE_NEUTRAL_BALL,// �{�[���������Ă���Ƃ��̃j���[�g����
	MOTIONTYPE_RUNNING_BALL,// �{�[���������Ă���Ƃ��̃����j���O
	MOTIONTYPE_CATCH,		// �{�[���L���b�`
	MOTIONTYPE_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE;

//=============================================================================
// �\���̒�`
////=============================================================================
typedef struct
{// ���f��
	LPD3DXMESH		pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffMat;					// �}�e���A�����ւ̃|�C���^
	DWORD			nNumMat;					// �}�e���A�����̐�
	D3DXMATRIX		mtxWorld;					// ���[���h�}�g���b�N�X
	char			cPass[32];					// ���f���t�@�C���̃p�X
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		posSet;						// �����ʒu
	D3DXVECTOR3		rot;						// ����(��])
	D3DXVECTOR3		rotDest;					// ���B�������(��])
	int				nIdxModelParent;			// �e�̔ԍ�
	int				nIdxShadow;					// �e��ID
} MODEL;

typedef struct
{// �L�[�v�f
	D3DXVECTOR3		pos;						// ���݂̈ʒu
	D3DXVECTOR3		rot;						// ����(��])
} KEY;

typedef struct
{// �L�[���
	int				nNumKyeFrame;				// �L�[�t���[����
	KEY				aKey[MAX_MODEL];			// �L�[�v�f
	bool			bUse;						// �L�[���g�p���Ă��邩
} KEY_INFO;

typedef struct
{// ���[�V�������
	int				nCntFrame;					// ���݂̃t���[����
	int				nNumKey;					// ���݂̃L�[
	int				nMaxKey;					// �ő�L�[��
	bool			bLoop;						// ���[�v���邩
	KEY_INFO		aKeyInfo[MAX_KEY];			// �L�[���
} MOTION_INFO;

typedef struct
{// �v���C���[
	D3DXMATRIX		mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;						// ���݂̈ʒu
	D3DXVECTOR3		posOld;						// �O��̈ʒu
	D3DXVECTOR3		move;						// �ړ���
	D3DXVECTOR3		rot;						// ����(��])
	D3DXVECTOR3		rotDest;					// ���B�������(��])
	D3DXVECTOR3		rotDiff;					// ���B��������ƌ��݂̌����̍�
	int				nIdxShadow;					// �e��ID
	int				nNumParts;					// �p�[�c��
	int				nLife;						// �̗�
	bool			bJump;						// �W�����v���Ă��邩
	bool			bMotion;					// ���[�V�����Đ�����
	MODEL			aModel[MAX_MODEL];			// ���f�����
	MOTION_INFO		aMotionInfo[MOTIONTYPE_MAX];// ���[�V�������
	MOTIONTYPE		motionType;					// ���݂̃��[�V����
	bool			bCollAtk;					// �U�����������Ă��邩
	int				nRaidTrout;					// ����Ă���}�X�̔ԍ�
	int				nNextTrout;					// ���B����}�X�̔ԍ�
	float			fRadius;					//�����蔻��p���a
	bool			bHold;						//�{�[�������Ă邩�ۂ�
	bool			bUse;
} PLAYER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void LoadPlayerMotion(void);
void ControlPlayer(int nCnt);
void Move(CAMERA *pCamera);
void MoveLimit(D3DXVECTOR3 *pos, float fRadius, bool bHold);	//�ړ�����
void MotionPlayer(int nCnt);									// ���[�V�����̍Đ�
void CollisionPlayer(int nCnt);									// �ڐG����
void MotionChangePlayer(MOTIONTYPE motionType, int nCnt);		// ���[�V�����̐؂�ւ�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	g_paTexturePlayer[MAX_TEXTURE] = {};			// �e�N�X�`���ւ̃|�C���^

PLAYER				g_player[NUM_JOYPAD_MAX];						// �v���C���[�̏��

// �w���̐F
D3DXCOLOR g_aPlayer[NUM_JOYPAD_MAX] = {
	D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),	// 1P
	D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),	// 2P
	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),	// 3P
	D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)	// 4P
};

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �t�@�C���̓ǂݍ���
	LoadPlayerMotion();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX((LPCSTR)&g_player[0].aModel[nCntModel].cPass[0], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_player[0].aModel[nCntModel].pBuffMat, NULL,
			&g_player[0].aModel[nCntModel].nNumMat,
			&g_player[0].aModel[nCntModel].pMesh);
	}

	for (int nCnt = 1; nCnt < NUM_JOYPAD_MAX; nCnt++)
	{// �����f�[�^���i�[
		g_player[nCnt] = g_player[0];
	}

	// �������i�V�ԃv���C���[���ōő�񐔂�ύX����j
	for (int nCnt = 0; nCnt < GetNumPad(); nCnt++)
	{
		// �����ݒ�
		g_player[nCnt].posOld = g_player[nCnt].pos;
		g_player[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player[nCnt].bJump = false;
		g_player[nCnt].bMotion = true;
		g_player[nCnt].motionType = MOTIONTYPE_NEUTRAL;
		g_player[nCnt].bCollAtk = false;
		g_player[nCnt].fRadius = 30;
		g_player[nCnt].bHold = false;
		g_player[nCnt].bUse = true;

		// �e�̍쐬
		g_player[nCnt].nIdxShadow = SetShadow(g_player[nCnt].pos, g_player[nCnt].rot, D3DXVECTOR3(100.0f, 0.0f, 100.0f));

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			g_player[nCnt].aModel[nCntModel].posSet = g_player[nCnt].aModel[nCntModel].pos;
		}
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_paTexturePlayer[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE1, &g_paTexturePlayer[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE2, &g_paTexturePlayer[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE3, &g_paTexturePlayer[3]);
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		// ���b�V���̊J��
		for (int nCntMesh = 0; nCntMesh < MAX_MODEL; nCntMesh++)
		{
			if (g_player[nCnt].aModel[nCntMesh].pMesh != NULL)
			{
				g_player[nCnt].aModel[nCntMesh].pMesh->Release();
				g_player[nCnt].aModel[nCntMesh].pMesh = NULL;
			}
		}

		// �}�e���A���̊J��
		for (int nCntMat = 0; nCntMat < MAX_MODEL; nCntMat++)
		{
			if (g_player[nCnt].aModel[nCntMat].pBuffMat != NULL)
			{
				g_player[nCnt].aModel[nCntMat].pBuffMat->Release();
				g_player[nCnt].aModel[nCntMat].pBuffMat = NULL;
			}
		}
	}

	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < NUM_JOYPAD_MAX; nCntTex++)
	{
		if (g_paTexturePlayer[nCntTex] != NULL)
		{
			g_paTexturePlayer[nCntTex]->Release();
			g_paTexturePlayer[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	int nNumPlayer = GetNumPad();	// �v���C���[�̐l��

	if (*GetMode() == MODE_RESULT)
	{
		for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
		{
			g_player[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// ���[�V�����̍Đ�
			MotionPlayer(nCnt);
		}
		return;
	}

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		if (g_player[nCnt].bUse)
		{
			// �O��̈ʒu��ۑ�
			g_player[nCnt].posOld = g_player[nCnt].pos;

			// �d�͂̐ݒ�
			g_player[nCnt].pos.y -= PLAYER_GRAVITY;

			if (g_player[nCnt].motionType != MOTIONTYPE_ACTION&&g_player[nCnt].motionType != MOTIONTYPE_CATCH)
			{
				if (!GetTimeUse(0))
				{// �J�E���g�_�E�����I�������ꍇ
					// �R���g���[��
					ControlPlayer(nCnt);
				}
			}

			// �p�x�����l�𒴂����ꍇ�̐��l����
			if (g_player[nCnt].rot.y > D3DX_PI)
			{
				g_player[nCnt].rot.y -= D3DX_PI * 2;
			}

			if (g_player[nCnt].rot.y < -D3DX_PI)
			{
				g_player[nCnt].rot.y += D3DX_PI * 2;
			}

			// �p�x�����l�𒴂����ꍇ�̐��l����
			if (g_player[nCnt].rotDest.y > D3DX_PI)
			{
				g_player[nCnt].rotDest.y -= D3DX_PI * 2;
			}

			if (g_player[nCnt].rotDest.y < -D3DX_PI)
			{
				g_player[nCnt].rotDest.y += D3DX_PI * 2;
			}

			// �����̍������߂�
			g_player[nCnt].rotDiff.y = g_player[nCnt].rotDest.y - g_player[nCnt].rot.y;

			// �p�x�����l�𒴂����ꍇ�̐��l����
			if (g_player[nCnt].rotDiff.y > D3DX_PI)
			{
				g_player[nCnt].rotDiff.y -= D3DX_PI * 2;
			}

			if (g_player[nCnt].rotDiff.y < -D3DX_PI)
			{
				g_player[nCnt].rotDiff.y += D3DX_PI * 2;
			}

			// �����̍X�V
			g_player[nCnt].rot.y += (g_player[nCnt].rotDiff.y)*0.2f;

			// �ʒu�̍X�V
			g_player[nCnt].pos.x += g_player[nCnt].move.x;
			g_player[nCnt].pos.y += g_player[nCnt].move.y;
			g_player[nCnt].pos.z += g_player[nCnt].move.z;

			// �ڐG����
			CollisionPlayer(nCnt);

			if (g_player[nCnt].bUse)
			{
				// �e�̈ʒu��ݒ�
				SetPositionShadow(g_player[nCnt].nIdxShadow, g_player[nCnt].pos);
			}

			// ����(����)
			g_player[nCnt].move.x += (0 - g_player[nCnt].move.x) * 0.125f;
			g_player[nCnt].move.y += (0 - g_player[nCnt].move.y) * 0.125f;
			g_player[nCnt].move.z += (0 - g_player[nCnt].move.z) * 0.125f;

			if (g_player[nCnt].motionType != MOTIONTYPE_JUMP && !g_player[nCnt].bMotion)
			{//  �Đ���Ԃ�
				g_player[nCnt].bMotion = true;
			}

			// ���[�V�����̍Đ�
			MotionPlayer(nCnt);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL *pMat;							// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCnt = 0; nCnt < GetNumPad(); nCnt++)
	{
		// �{��
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player[nCnt].mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player[nCnt].rot.y, g_player[nCnt].rot.x, g_player[nCnt].rot.z);
		D3DXMatrixMultiply(&g_player[nCnt].mtxWorld, &g_player[nCnt].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player[nCnt].pos.x, g_player[nCnt].pos.y, g_player[nCnt].pos.z);
		D3DXMatrixMultiply(&g_player[nCnt].mtxWorld, &g_player[nCnt].mtxWorld, &mtxTrans);

		MODEL *pModel = &g_player[nCnt].aModel[0];

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player[nCnt].mtxWorld);

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pModel->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel->rot.y, pModel->rot.x, pModel->rot.z);
			D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pModel->pos.x,
				pModel->pos.y, pModel->pos.z);
			D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

			if (pModel->nIdxModelParent == -1)
			{
				D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &g_player[nCnt].mtxWorld);
			}
			else
			{
				D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &g_player[nCnt].aModel[pModel->nIdxModelParent].mtxWorld);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player[nCnt].aModel[nCntModel].nNumMat; nCntMat++)
			{
				if (g_player[nCnt].bUse)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_paTexturePlayer[nCnt]);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �`��
					pModel->pMesh->DrawSubset(nCntMat);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �R���g���[��
//=============================================================================
void ControlPlayer(int nCnt)
{
	int nStickH, nStickV;			// �A�i���O�X�e�B�b�N���̓��͏��
	CAMERA *pCamera = GetCamera(0);	// �J�������̎擾
	BALL *pBall = GetBall();

	// �A�i���O�X�e�B�b�N���̎擾
	GetJoypadStickLeft(nCnt, &nStickH, &nStickV);

	// �p�b�h
	if (nStickH != 0 || nStickV != 0)
	{
		// �ړ�
		g_player[nCnt].move.x += sinf(D3DX_PI * (float)atan2(nStickH, nStickV) / D3DX_PI + pCamera->rot.y) * 1.0f;
		g_player[nCnt].move.z += cosf(D3DX_PI * (float)atan2(nStickH, nStickV) / D3DX_PI + pCamera->rot.y) * 1.0f;

		// ����
		g_player[nCnt].rotDest.y = D3DX_PI * -(float)atan2(nStickH, -nStickV) / D3DX_PI + pCamera->rot.y;

		if (g_player[nCnt].motionType != MOTIONTYPE_RUNNING &&
			g_player[nCnt].motionType != MOTIONTYPE_RUNNING_BALL && g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{
			if (pBall->nParent == nCnt && pBall->bHold)
			{
				// ���[�V�����̐؂�ւ�
				MotionChangePlayer(MOTIONTYPE_RUNNING_BALL, nCnt);
			}
			else
			{
				// ���[�V�����̐؂�ւ�
				MotionChangePlayer(MOTIONTYPE_RUNNING, nCnt);
			}

		}
	}
	else if (g_player[nCnt].motionType == MOTIONTYPE_RUNNING || g_player[nCnt].motionType == MOTIONTYPE_RUNNING_BALL)
	{// �ړ�����߂��ꍇ
		if (pBall->nParent == nCnt && pBall->bHold)
		{
			// ���[�V�����̐؂�ւ�
			MotionChangePlayer(MOTIONTYPE_NEUTRAL_BALL, nCnt);
		}
		else
		{
			// ���[�V�����̐؂�ւ�
			MotionChangePlayer(MOTIONTYPE_NEUTRAL, nCnt);
		}
	}

	{// �������Ƃŏ���
		//�L�[�{�[�h�ړ�
		Move(pCamera);
	}

	// �W�����v
	if (!g_player[nCnt].bJump)// �W�����v���Ă��Ȃ��ꍇ
	{
		if (GetKeyboardTrigger(DIK_J) || GetJoypadTrigger(nCnt, JOYPADKEY_A))
		{
			g_player[nCnt].move.y += PLAYER_JUMP_MOVE;

			// ���[�V�����̐؂�ւ�
			MotionChangePlayer(MOTIONTYPE_JUMP, nCnt);
		}
	}

	//�{�[�������違����
	float fDistX, fDistZ;
	float fDistans = 100000;
	fDistX = pBall->pos.x - g_player[nCnt].pos.x;
	fDistZ = pBall->pos.z - g_player[nCnt].pos.z;
	fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

	if (GetJoypadTrigger(nCnt, JOYPADKEY_B))
	{
		// ���[�V�����̐؂�ւ�
		MotionChangePlayer(MOTIONTYPE_CATCH, nCnt);
		
		if (pBall->nParent == nCnt && pBall->bHold)
		{	//������
			pBall->state = BALLSTATE_THROW;
			pBall->bHold = false;
			g_player[nCnt].bHold = false;
			ThrowBall(g_player[nCnt].move);

			// ���[�V�����̐؂�ւ�
			MotionChangePlayer(MOTIONTYPE_ACTION, nCnt);
		}
		else if (g_player[nCnt].fRadius + PLAYER_RANGE > fDistans && !pBall->bHold)
		{	//����
			pBall->state = BALLSTATE_HOLD;
			pBall->bHold = true;
			g_player[nCnt].bHold = true;
			pBall->nParent = nCnt;
		}
	}
}

//=============================================================================
//	�ړ�����
//=============================================================================
void MoveLimit(D3DXVECTOR3 *pos, float fRadius, bool bHold)
{
	//if (bHold)
	//{
	//	//���E
	//	if (pos->x > MESHFIELD_SIZE * 2 - (fRadius + BALL_RADIUS*2))
	//	{
	//		pos->x = MESHFIELD_SIZE * 2 - (fRadius + BALL_RADIUS*2);
	//	}
	//	if (pos->x < -MESHFIELD_SIZE * 2 + (fRadius + BALL_RADIUS*2))
	//	{
	//		pos->x = -MESHFIELD_SIZE * 2 + (fRadius + BALL_RADIUS*2);
	//	}

	//	//����O
	//	if (pos->z > MESHFIELD_SIZE * 2 - (fRadius + BALL_RADIUS*2))
	//	{
	//		pos->z = MESHFIELD_SIZE * 2 - (fRadius + BALL_RADIUS*2);
	//	}
	//	if (pos->z < -MESHFIELD_SIZE * 2 + (fRadius + BALL_RADIUS*2))
	//	{
	//		pos->z = -MESHFIELD_SIZE * 2 + (fRadius + BALL_RADIUS*2);
	//	}
	//	return;
	//}
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
//	�ړ�����
//=============================================================================
void Move(CAMERA *pCamera)
{
	// �L�[�{�[�h
	if (GetKeyboardPress(DIK_A))
	{
		if (GetKeyboardPress(DIK_W))
		{// ����
			g_player[1].move.x += sinf(-D3DX_PI*0.75f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(-D3DX_PI*0.75f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = D3DX_PI*0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{// ����
			g_player[1].move.x += sinf(-D3DX_PI*0.25f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(-D3DX_PI*0.25f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = D3DX_PI*0.25f + pCamera->rot.y;
		}
		else
		{// ��
			g_player[1].move.x += sinf(-D3DX_PI*0.5f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(-D3DX_PI*0.5f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = D3DX_PI*0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{
		if (GetKeyboardPress(DIK_W))
		{// �E��
			g_player[1].move.x += sinf(D3DX_PI*0.75f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(D3DX_PI*0.75f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = -D3DX_PI*0.75f + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{// �E��
			g_player[1].move.x += sinf(D3DX_PI*0.25f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(D3DX_PI*0.25f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = -D3DX_PI*0.25f + pCamera->rot.y;
		}
		else
		{// �E
			g_player[1].move.x += sinf(D3DX_PI*0.5f - pCamera->rot.y) * PLAYER_MOVE;
			g_player[1].move.z -= cosf(D3DX_PI*0.5f - pCamera->rot.y) * PLAYER_MOVE;

			g_player[1].rotDest.y = -D3DX_PI*0.5f + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_W))
	{// ��
		g_player[1].move.x += sinf(D3DX_PI - pCamera->rot.y) * PLAYER_MOVE;
		g_player[1].move.z -= cosf(D3DX_PI - pCamera->rot.y) * PLAYER_MOVE;

		g_player[1].rotDest.y = D3DX_PI + pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_S))
	{// ��
		g_player[1].move.x += sinf(0 - pCamera->rot.y) * PLAYER_MOVE;
		g_player[1].move.z -= cosf(0 - pCamera->rot.y) * PLAYER_MOVE;

		g_player[1].rotDest.y = 0 + pCamera->rot.y;
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		BALL *pBall = GetBall();
		float fDistX, fDistZ;
		float fDistans = 100000;
		fDistX = pBall->pos.x - g_player[1].pos.x;
		fDistZ = pBall->pos.z - g_player[1].pos.z;
		fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

		if (g_player[1].bHold)
		{	//������
			pBall->state = BALLSTATE_THROW;
			pBall->bHold = false;
			g_player[1].bHold = false;
			ThrowBall(g_player[1].move);
		}
		else if (g_player[1].fRadius + PLAYER_RANGE > fDistans && !pBall->bHold)
		{	//����
			pBall->state = BALLSTATE_HOLD;
			pBall->bHold = true;
			g_player[1].bHold = true;
			pBall->nParent = 1;
		}
	}
}

//=============================================================================
// ���[�V�����̍Đ�
//=============================================================================
void MotionPlayer(int nCnt)
{
	if (g_player[nCnt].bMotion)
	{
		MOTION_INFO* pInfo = &g_player[nCnt].aMotionInfo[g_player[nCnt].motionType];
		// ���[�V����
		for (int nCntModel = 0; nCntModel < g_player[nCnt].nNumParts; nCntModel++)
		{
			KEY *pKeyInfo, *pNextKey;

			// ���݂̃L�[
			pKeyInfo = &pInfo->aKeyInfo[pInfo->nNumKey].aKey[nCntModel];

			// ���̃L�[
			if (pInfo->nNumKey + 1 == pInfo->nMaxKey)
			{
				pNextKey = &pInfo->aKeyInfo[0].aKey[nCntModel];
			}
			else
			{
				pNextKey = &pInfo->aKeyInfo[pInfo->nNumKey + 1].aKey[nCntModel];
			}

			// �p�[�c�̈ʒu�ݒ�
			g_player[nCnt].aModel[nCntModel].pos = g_player[nCnt].aModel[nCntModel].posSet + pKeyInfo->pos + (pNextKey->pos - pKeyInfo->pos)*
				pInfo->nCntFrame / pInfo->aKeyInfo[pInfo->nNumKey].nNumKyeFrame;

			// �p�[�c�̌����ݒ�
			g_player[nCnt].aModel[nCntModel].rot = pKeyInfo->rot + (pNextKey->rot - pKeyInfo->rot)*
				pInfo->nCntFrame / pInfo->aKeyInfo[pInfo->nNumKey].nNumKyeFrame;
		}

		pInfo->nCntFrame++;

		if (pInfo->nCntFrame == pInfo->aKeyInfo[pInfo->nNumKey].nNumKyeFrame)
		{
			pInfo->nNumKey++;
			pInfo->nCntFrame = 0;

			if (g_player[nCnt].bHold && pInfo->nNumKey >= 2)
			{// �{�[�����L���b�`�����ꍇ
				// ���[�V�����̐؂�ւ�
				MotionChangePlayer(MOTIONTYPE_NEUTRAL_BALL, nCnt);
				return;
			}
		}

		if (!pInfo->bLoop && pInfo->nNumKey + 1 == pInfo->nMaxKey)
		{// ���[�V���������[�v���Ȃ��ꍇ
			if (g_player[nCnt].motionType != MOTIONTYPE_JUMP)
			{// �W�����v�ȊO
				pInfo->nNumKey = 0;
				pInfo->nCntFrame = 0;
				g_player[nCnt].motionType = MOTIONTYPE_NEUTRAL;// �ҋ@��ԂɕύX
			}
			else
			{// �W�����v�̂�
				g_player[nCnt].bMotion = false;
			}
		}
		else if (pInfo->nNumKey == pInfo->nMaxKey && g_player[nCnt].motionType != MOTIONTYPE_JUMP)
		{// ���[�V�������I�������ꍇ
			pInfo->nNumKey = 0;
		}
	}
}

//=============================================================================
// �ڐG����
//=============================================================================
void CollisionPlayer(int nCnt)
{
	MODE *pMode = GetMode();		// ���݂̃��[�h
	bool bRand = false;				// ���n����

	// �ړ�����
	MoveLimit(&g_player[nCnt].pos, g_player[nCnt].fRadius, g_player[nCnt].bHold);

	if (g_player[nCnt].pos.y < 0)
	{
		g_player[nCnt].pos.y = 0;
		bRand = true;
	}

	if (bRand)
	{// ���n�����ꍇ
		g_player[nCnt].bJump = false;	// �W�����v�ł���悤�ɂ���

		if (g_player[nCnt].motionType == MOTIONTYPE_JUMP)
		{
			// ���[�V�����̐؂�ւ�
			MotionChangePlayer(MOTIONTYPE_LAND, nCnt);
		}
	}
	else if (!g_player[nCnt].bJump)
	{
		g_player[nCnt].bJump = true;	// �W�����v�ł��Ȃ��悤�ɂ���
	}

	// �w�n�̏Փ˔���(�����̐F�ɂ̂ݓ����)
	CollisionField(&g_player[nCnt].pos, &g_player[nCnt].posOld, D3DXVECTOR3(50.0f, 0.0f, 50.0f), nCnt);

	// �����蔻��
	if (*pMode == MODE_GAME)
	{
		BALL *pBall = GetBall();

		if (!g_player[nCnt].bHold && pBall->nParent != nCnt)// �{�[���������Ă��邩
		{
			if (CollisionBall(&g_player[nCnt].pos, &g_player[nCnt].posOld, g_player[nCnt].fRadius))
			{
				if (pBall->state == BALLSTATE_THROW)
				{
					ChangeFieldColor(pBall->nParent, nCnt);
					g_player[nCnt].bUse = false;
					DeleteShadow(g_player[nCnt].nIdxShadow);
				}
			}
		}
	}
}

//=============================================================================
// ���[�V�����̐؂�ւ�
//=============================================================================
void MotionChangePlayer(MOTIONTYPE motionType, int nCnt)
{
	g_player[nCnt].aMotionInfo[g_player[nCnt].motionType].nNumKey = 0;
	g_player[nCnt].aMotionInfo[g_player[nCnt].motionType].nCntFrame = 0;
	g_player[nCnt].motionType = motionType;
}

//=============================================================================
// ���[�V�����̃��[�h
//=============================================================================
void LoadPlayerMotion(void)
{
	FILE *pFile = NULL;		// �t�@�C���|�C���^

	// �t�@�C���I�[�v��
	pFile = fopen(PLAYER_FILENAME, "r");

	if (pFile != NULL)
	{// �t�@�C���������ɊJ�����ꍇ
		char cReedText[128];	// �ǂݎ��p
		char cHeadText[128];	// �����i�[�p
		int nLoop = 0;;			// ���[�v�i�[�p
		MOTION_INFO *pMotion = &g_player[0].aMotionInfo[0];
		MODEL *pModel = &g_player[0].aModel[0];

		while (strcmp(cHeadText, "SCRIPT") != 0)
		{// SCRIPT���ǂݍ��܂��܂Ń��[�v
			fgets(cReedText, sizeof(cReedText), pFile);
			sscanf(cReedText, "%s", &cHeadText);
		}

		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReedText, sizeof(cReedText), pFile);
				sscanf(cReedText, "%s", &cHeadText);

				if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
				{// �t�@�C����
					sscanf(cReedText, "%*s %*s %s", &pModel->cPass[0]);
					pModel++;
				}

				if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					pModel = &g_player[0].aModel[0];

					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReedText, sizeof(cReedText), pFile);
						sscanf(cReedText, "%s", &cHeadText);

						if (strcmp(cHeadText, "MOVE") == 0)
						{// �ړ���
							//sscanf(cReedText, "%s %s %f", &cDie, &cDie, &g_player[0].fAddMove);
						}

						//if (strcmp(cHeadText, "JUMP") == 0)
						//{// �W�����v��
						//	sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_player[0].nNumParts);
						//}

						//if (strcmp(cHeadText, "RADIUS") == 0)
						//{// ���a
						//	sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_player[0].nNumParts);
						//}

						if (strcmp(cHeadText, "NUM_PARTS") == 0)
						{// �p�[�c��
							sscanf(cReedText, "%*s %*s %d", &g_player[0].nNumParts);
						}
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReedText, sizeof(cReedText), pFile);
								sscanf(cReedText, "%s", &cHeadText);

								if (strcmp(cHeadText, "INDEX") == 0)
								{// �{�̂̔ԍ�
								 //sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_player[0].nNumParts);
								}

								if (strcmp(cHeadText, "PARENT") == 0)
								{// �e�̔ԍ�
									sscanf(cReedText, "%*s %*s %d", &pModel->nIdxModelParent);
								}

								if (strcmp(cHeadText, "POS") == 0)
								{// �ʒu
									sscanf(cReedText, "%*s %*s %f %f %f", &pModel->pos.x, &pModel->pos.y, &pModel->pos.z);
								}

								if (strcmp(cHeadText, "ROT") == 0)
								{// ����
									sscanf(cReedText, "%*s %*s %f %f %f", &pModel->rot.x, &pModel->rot.y, &pModel->rot.z);
								}
							}
							pModel++;
						}
					}
				}
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					KEY_INFO *pKeyInfo = &pMotion->aKeyInfo[0];
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cReedText, sizeof(cReedText), pFile);
						sscanf(cReedText, "%s", &cHeadText);

						if (strcmp(cHeadText, "LOOP") == 0)
						{// ���[�v���邩�ǂ���
							sscanf(cReedText, "%*s %*s %d", &nLoop);

							// 1�̏ꍇ���[�v����
							pMotion->bLoop = nLoop == 1 ? true : false;
						}
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{// �ő�L�[��
							sscanf(cReedText, "%*s %*s %d", &pMotion->nMaxKey);
						}
						else if (strcmp(cHeadText, "KEYSET") == 0)
						{
							KEY *pKey = &pKeyInfo->aKey[0];
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cReedText, sizeof(cReedText), pFile);
								sscanf(cReedText, "%s", &cHeadText);

								if (strcmp(cHeadText, "FRAME") == 0)
								{// �L�[�t���[����
									sscanf(cReedText, "%*s %*s %d", &pKeyInfo->nNumKyeFrame);
								}
								else if (strcmp(cHeadText, "KEY") == 0)
								{
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cReedText, sizeof(cReedText), pFile);
										sscanf(cReedText, "%s", &cHeadText);

										if (strcmp(cHeadText, "POS") == 0)
										{// �ʒu
											sscanf(cReedText, "%*s %*s %f %f %f",
												&pKey->pos.x, &pKey->pos.y, &pKey->pos.z);
										}
										else if (strcmp(cHeadText, "ROT") == 0)
										{// ����
											sscanf(cReedText, "%*s %*s %f %f %f",
												&pKey->rot.x, &pKey->rot.y, &pKey->rot.z);
										}
									}
									pKey++;
								}
							}
							pKeyInfo++;
						}
					}
					pMotion++;
				}
			}
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �J���Ȃ������ꍇ
		// �G���[���b�Z�[�W
		//MessageBox(hWnd, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B", "�x��", MB_ICONWARNING);
	}
}

//=============================================================================
// �ꏊ�̐ݒ�
//=============================================================================
void SetPlayerPos(D3DXVECTOR3 pos, int nNum)
{
	g_player[nNum].pos = pos;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void SetPlayerRot(D3DXVECTOR3 rot, int nNum)
{
	g_player[nNum].rot = rot;
	g_player[nNum].rotDest = g_player[nNum].rot;
}

//=============================================================================
// �ړ��ʂ̐ݒ�
//=============================================================================
void SetPlayerMove(D3DXVECTOR3 move, int nNum)
{
	g_player[nNum].move = move;
}

//=============================================================================
// �ꏊ���̎擾����
//=============================================================================
D3DXVECTOR3 *GetPlayerPos(int nNum)
{
	return&g_player[nNum].pos;
}

//=============================================================================
// �ꏊ���̎擾����
//=============================================================================
D3DXVECTOR3 *GetPlayerPosOld(int nNum)
{
	return&g_player[nNum].posOld;
}

//=============================================================================
// �������̎擾����
//=============================================================================
D3DXVECTOR3 *GetPlayerRot(int nNum)
{
	return&g_player[nNum].rot;
}

//=============================================================================
// �ړ��ʏ��̎擾����
//=============================================================================
D3DXVECTOR3 *GetPlayerMove(int nNum)
{
	return&g_player[nNum].move;
}

//=============================================================================
// ���a���̎擾����
//=============================================================================
float *GetPlayerRadius(int nNum)
{
	return&g_player[nNum].fRadius;
}

//=============================================================================
// �v���C���[�̐�����Ԃ̎擾����
//=============================================================================
bool GetPlayerUse(int nNum)
{
	return g_player[nNum].bUse;
}