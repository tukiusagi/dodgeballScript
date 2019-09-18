//=============================================================================
//
// ���f������ [model.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// ���f���̎��
//=============================================================================
typedef enum
{
	MODELTYPE_BILL0 = 0,		// �r��0			[0]
	MODELTYPE_BILL1,			// �r��1			[1]
	MODELTYPE_BILL2,			// �r��2			[2]
	MODELTYPE_BILL3,			// �r��3			[3]
	MODELTYPE_BILL4,			// �r��4			[4]
	MODELTYPE_10_BOARD,			// 10���~�Ŕ�		[5]
	MODELTYPE_30_BOARD,			// 30���~�Ŕ�		[6]
	MODELTYPE_50_BOARD,			// 50���~�Ŕ�		[7]
	MODELTYPE_100_BOARD,		// 100���~�Ŕ�		[8]
	MODELTYPE_CLIFF,			// �R				[9]
	MODELTYPE_CONVENI,			// �R���r�j			[10]
	MODELTYPE_WAC,				// ���b�N			[11]
	MODELTYPE_MAX				// ��ނ̍ő吔
} MODELTYPE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE type);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);
//bool CollisionModelCollide(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size);

D3DXVECTOR3 *GetModelPos(void);

#endif
