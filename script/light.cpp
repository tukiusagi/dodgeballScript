//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "light.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_LIGHT	(3)	// ���C�g�̐�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
D3DLIGHT9 g_aLight[MAX_LIGHT];	// ���C�g�̏��

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;							// �ݒ�p������x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ���C�g�̊g�U����ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[1].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	g_aLight[2].Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������
	g_aLight[0].Direction = vecDir;

	vecDir = D3DXVECTOR3(-0.25f, 0.85f, -0.40f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[1].Direction = vecDir;

	vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.40f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[2].Direction = vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
}

