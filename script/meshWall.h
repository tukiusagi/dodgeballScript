//=============================================================================
//
// ���b�V���E�H�[������ [meshWall.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIdxBuffer;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	pos;					// �ʒu
	D3DXVECTOR3	rot;					// ����(��])
	D3DXMATRIX	mtxWorld;				// ���[���h�}�g���b�N�X
	int nNumWidth;						// ���̕�����
	int nNumHeight;						// �c�̕�����
	int nNumPolygon;					// ���|���S����
	int nNumVertex;						// �����_��
	int nNumIndex;						// ���C���f�b�N�X��
	D3DXVECTOR3	sizePolygon;			// 1������̃|���S���̑傫��
	D3DXVECTOR2 texPolygon;				// 1������̃e�N�X�`�����W
	bool bUse;							// �g�p���Ă��邩
} MESHWALL;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
