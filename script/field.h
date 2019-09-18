//=============================================================================
//
// ���� [field.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _FIELDL_H_
#define _FIELDL_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

void ChangeFieldColor(int nGiveNum, int nReceiveNum);
void CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 size, int nCol);

#endif
