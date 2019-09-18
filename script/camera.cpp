//=============================================================================
//
// �J�������� [camera.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "camera.h"
#include "joypad.h"
#include "keyboard.h"
#include "player.h"
#include "title.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CAMERA_MOVE	(0.25)	// �J�����̈ړ���
#define MAX_CAMERA	(4)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
CAMERA g_camera[MAX_CAMERA];	// �J�������

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	if (*GetMode() != *GetModeOld())
	{
		for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
		{
			g_camera[nCnt].posV = D3DXVECTOR3(0.0f, 300.0f, -800.0f);
			g_camera[nCnt].posVDest = g_camera[nCnt].posV;
			g_camera[nCnt].posVOld = g_camera[nCnt].posV;
			g_camera[nCnt].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCnt].posRDest = g_camera[nCnt].posR;
			g_camera[nCnt].posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCnt].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_camera[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCnt].rotDest = g_camera[nCnt].rot;
			g_camera[nCnt].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCnt].rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_camera[nCnt].fDistance = g_camera[nCnt].posR.z - g_camera[nCnt].posV.z;
			g_camera[nCnt].bWraparound = false;
		}
	}
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	int nStickH, nStickV;

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		// �O��̎��_�̈ʒu�ۑ�
		g_camera[0].posVOld = g_camera[0].posV;

		// �A�i���O�X�e�B�b�N�E�̎擾
		GetJoypadStickRight(0, &nStickH, &nStickV);

		g_camera[0].posRDest.x = -25;
		g_camera[0].posRDest.y = 90;
		g_camera[0].posRDest.z = 25;

		// �p�x�����l�𒴂����ꍇ�̐��l����
		if (g_camera[0].rotDest.y > D3DX_PI)
		{
			g_camera[0].rotDest.y -= D3DX_PI * 2;
		}

		if (g_camera[0].rotDest.y < -D3DX_PI)
		{
			g_camera[0].rotDest.y += D3DX_PI * 2;
		}

		if (g_camera[0].bWraparound == true)
		{
			if (g_camera[0].rotDest.y - g_camera[0].rot.y < 0.01 && g_camera[0].rotDest.y - g_camera[0].rot.y>-0.01)
			{
				g_camera[0].bWraparound = false;
			}
		}

		// �����̍������߂�
		g_camera[0].rotDiff.y = g_camera[0].rotDest.y - g_camera[0].rot.y;

		// �p�x�����l�𒴂����ꍇ�̐��l����
		if (g_camera[0].rotDiff.y > D3DX_PI)
		{
			g_camera[0].rotDiff.y -= D3DX_PI * 2;
		}

		if (g_camera[0].rotDiff.y < -D3DX_PI)
		{
			g_camera[0].rotDiff.y += D3DX_PI * 2;
		}

		// ��荞�ݒ��̏ꍇ
		if (g_camera[0].bWraparound == true)
		{
			g_camera[0].rot.y += (g_camera[0].rotDiff.y)*0.2f;
		}

		// �p�x�����l�𒴂����ꍇ�̐��l����
		if (g_camera[0].rotDest.y > D3DX_PI)
		{
			g_camera[0].rotDest.y -= D3DX_PI * 2;
		}

		if (g_camera[0].rotDest.y < -D3DX_PI)
		{
			g_camera[0].rotDest.y += D3DX_PI * 2;
		}

		// �p�x�����l�𒴂����ꍇ�̐��l����
		if (g_camera[0].rot.y > D3DX_PI)
		{
			g_camera[0].rot.y -= D3DX_PI * 2;
		}

		if (g_camera[0].rot.y < -D3DX_PI)
		{
			g_camera[0].rot.y += D3DX_PI * 2;
		}

		g_camera[0].posVDest.x = sinf(-g_camera[0].rot.y) * g_camera[0].fDistance;
		g_camera[0].posVDest.y = 120;
		g_camera[0].posVDest.z = -cosf(-g_camera[0].rot.y) * g_camera[0].fDistance;

		g_camera[0].posR += (g_camera[0].posRDest - g_camera[0].posR)*0.1f;

		g_camera[0].posV.x += (g_camera[0].posVDest.x - g_camera[0].posV.x)*0.1f;
		g_camera[0].posV.z += (g_camera[0].posVDest.z - g_camera[0].posV.z)*0.1f;

		// �����_�̉�]
		if (GetKeyboardPress(DIK_RIGHT) || nStickH > 0)
		{// ������
			g_camera[0].rotMove.y -= 0.005f;
		}

		if (GetKeyboardPress(DIK_LEFT) || nStickH < 0)
		{// �E����
			g_camera[0].rotMove.y += 0.005f;
		}

		// �����̍X�V
		g_camera[0].rot += g_camera[0].rotMove;

		// ����
		g_camera[0].rotMove.y += (0 - g_camera[0].rotMove.y)*0.125f;

		// �p�x�����l�𒴂����ꍇ�̐��l����
		if (g_camera[0].rot.y > D3DX_PI)
		{
			g_camera[0].rot.y -= D3DX_PI * 2;
		}

		if (g_camera[0].rot.y < -D3DX_PI)
		{
			g_camera[0].rot.y += D3DX_PI * 2;
		}

		if (GetKeyboardPress(DIK_UP) || nStickV > 0)
		{// ��
			g_camera[0].posMove.y += CAMERA_MOVE;
		}
		if (GetKeyboardPress(DIK_DOWN) || nStickV < 0)
		{// ��
			g_camera[0].posMove.y -= CAMERA_MOVE;
		}

		// ���_�̏ꏊ�̍X�V
		g_camera[0].posV.y += g_camera[0].posMove.y;

		if (g_camera[0].posV.y <= 10)
		{
			g_camera[0].posV.y = g_camera[0].posVOld.y;
		}

		// ����
		g_camera[0].posMove.y += (0 - g_camera[0].posMove.y)*0.1f;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(int nCount)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nCount].mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&g_camera[nCount].mtxProjection,
		D3DXToRadian(45.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,										// NearZ�l/������͈�/��������
		4500.0f);									// Far Z�l/������͈�/�����܂�

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCount].mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nCount].mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(
		&g_camera[nCount].mtxView,
		&g_camera[nCount].posV,		// �J�����̎��_
		&g_camera[nCount].posR,		// �J�����̒����_
		&g_camera[nCount].vecU);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nCount].mtxView);
}

//=============================================================================
// �����_�̉�]
//=============================================================================
void RotMoveCameraR(float rotY, int nCnt)
{
	g_camera[nCnt].rot.y += rotY;

	// �p�x�����l�𒴂����ꍇ�̐��l����
	if (g_camera[nCnt].rot.y > D3DX_PI)
	{
		g_camera[nCnt].rot.y -= D3DX_PI * 2;
	}

	g_camera[nCnt].posV.x = g_camera[nCnt].posR.x + sinf(g_camera[nCnt].rot.y) * -g_camera[nCnt].fDistance;
	g_camera[nCnt].posV.z = g_camera[nCnt].posR.z + cosf(g_camera[nCnt].rot.y) * -g_camera[nCnt].fDistance;
}

//=============================================================================
// �����_�̉�]
//=============================================================================
void RotMoveCameraV(float rotY, int nCnt)
{
	g_camera[nCnt].rot.y += rotY;

	// �p�x�����l�𒴂����ꍇ�̐��l����
	if (g_camera[nCnt].rot.y > D3DX_PI)
	{
		g_camera[nCnt].rot.y -= D3DX_PI * 2;
	}

	g_camera[nCnt].posR.x = g_camera[nCnt].posV.x + sinf(g_camera[nCnt].rot.y) * g_camera[nCnt].fDistance;
	g_camera[nCnt].posR.z = g_camera[nCnt].posV.z + cosf(g_camera[nCnt].rot.y) * g_camera[nCnt].fDistance;
}

//=============================================================================
// �J�����̒����_�̐ݒ�
//=============================================================================
void SetCameraPosR(D3DXVECTOR3 pos, int nCnt)
{
	g_camera[nCnt].posR = pos;
	g_camera[nCnt].posRDest = g_camera[nCnt].posR;
}

//=============================================================================
// �J�����̎��_�̐ݒ�
//=============================================================================
void SetCameraPosV(D3DXVECTOR3 pos, int nCnt)
{
	g_camera[nCnt].posV = pos;
	g_camera[nCnt].posVDest = g_camera[nCnt].posV;
}

//=============================================================================
// �J�����̎��_�̐ݒ�
//=============================================================================
void SetCameraRot(D3DXVECTOR3 rot, int nCnt)
{
	g_camera[nCnt].rot = rot;
	g_camera[nCnt].rotDest = g_camera[nCnt].rot;
}

//=============================================================================
// �J�������̎擾
//=============================================================================
CAMERA *GetCamera(int nNum)
{
	return &g_camera[nNum];
}