//=============================================================================
//
// �{�[������ [ball.h]
// Author : YONEYAMA TAKESHI
//
//=============================================================================
#ifndef _BALL_H_
#define _BALL_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BALL_RADIUS	(30)

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	BALLSTATE_NORMAL = 0 ,
	BALLSTATE_HOLD,
	BALLSTATE_THROW,
	BALLSTATE_MAX
}BALLSTATE;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	LPD3DXMESH		pMesh;
	LPD3DXBUFFER	pBuffMat;
	DWORD			nNumMat;
	D3DXMATRIX		mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;			//�ʒu
	D3DXVECTOR3		posOld;		
	D3DXVECTOR3		move;			//�ړ�
	D3DXVECTOR3		rot;			//��]���
	D3DXCOLOR		col;			//�F
	int				nParent;		//�e(�����Ă���v���C���[)
	int				nIdxShadow;		//�e
	int				nDamage;		//�^����_���[�W��
	float			fRadius;		//���a
	bool			bHold;			//�����Ă�
	BALLSTATE		state;			//�{�[���̏��
} BALL;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitBall(void);
void UninitBall(void);
void UpdateBall(void);
void DrawBall(void);

void ThrowBall(D3DXVECTOR3 move);
BALL *GetBall(void);

void CatchBall(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius);
bool CollisionBall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,float Radius);


#endif
