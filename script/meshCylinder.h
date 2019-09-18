//=============================================================================
//
// ���b�V���R���C�_�[���� [meshCylinder.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

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
	D3DXVECTOR3	pos;		// �ʒu
	D3DXVECTOR3	rot;		// ����(��])
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	float fRadius;			// ���a
	int nNumWidth;			// ���̕�����
	int nNumHeight;			// �c�̕�����
	int nNumPolygon;		// ���|���S����
	int nNumVertex;			// �����_��
	int nNumIndex;			// ���C���f�b�N�X��
	//D3DXVECTOR3	sizePolygon;// 1������̃|���S���̑傫��
	D3DXVECTOR2 texPolygon;	// 1������̃e�N�X�`�����W
	float fAngle;			// 1���_������̊p�x
} MESHCYLINDER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif
