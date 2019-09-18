//=============================================================================
//
// �J�������� [camera.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �J�����̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			// ���݂̎��_
	D3DXVECTOR3 posVDest;		// �ړI�̎��_
	D3DXVECTOR3 posVOld;		// �O��̎��_
	D3DXVECTOR3 posR;			// ���݂̒����_(�Q�Ɠ_)
	D3DXVECTOR3 posRDest;		// �ړI�̒����_(�Q�Ɠ_)
	D3DXVECTOR3 posMove;		// �ꏊ�̈ړ���
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXVECTOR3 rot;			// ���݂̊p�x
	D3DXVECTOR3 rotOld;			// �O��̊p�x
	D3DXVECTOR3 rotDest;		// �ړI�̊p�x
	D3DXVECTOR3 rotDiff;		// �p�x�̍�
	D3DXVECTOR3 rotMove;		// �����̈ړ���
	float		fDistance;		// ����
	bool		bWraparound;	// ��荞�݂��Ă��邩
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;		// �r���[�}�g���b�N�X
} CAMERA;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(int nCount);
void SetCameraPosR(D3DXVECTOR3 pos, int nCnt);
void SetCameraPosV(D3DXVECTOR3 pos, int nCnt);
void SetCameraRot(D3DXVECTOR3 rot, int nCnt);
void RotMoveCameraR(float rotY, int nCnt);
void RotMoveCameraV(float rotY, int nCnt);

CAMERA *GetCamera(int nNum);

#endif
