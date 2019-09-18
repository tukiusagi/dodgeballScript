//=============================================================================
//
// �v���C���[���� [player.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_RANGE	(100)	//�L���b�`�͈�

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

//bool CollisionPlayerAttack(D3DXVECTOR3 pos, float fRadius);
//bool CollisionPlayerEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

void SetPlayerPos(D3DXVECTOR3 pos,int nNum);
void SetPlayerRot(D3DXVECTOR3 rot, int nNum);
void SetPlayerMove(D3DXVECTOR3 move, int nNum);

D3DXVECTOR3 *GetPlayerPos(int nNum);
D3DXVECTOR3 *GetPlayerPosOld(int nNum);
D3DXVECTOR3 *GetPlayerRot(int nNum);
D3DXVECTOR3 *GetPlayerMove(int nNum);
float *GetPlayerRadius(int nNum);
bool GetPlayerUse(int nNum);		//������Ԃ̎擾

#endif
