//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife);

#endif
