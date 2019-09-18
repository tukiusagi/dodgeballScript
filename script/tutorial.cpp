//=============================================================================
//
// �`���[�g���A����ʏ��� [tutorial.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "tutorial.h"
#include "camera.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "keyboard.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0		"data/TEXTURE/�`�L���`���[.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE1		"data/TEXTURE/�������`���[.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE2		"data/TEXTURE/�w���`���[.jpg"			// �ǂݍ��ރe�N�X�`��
#define TEXTUREFILE3		"data/TEXTURE/���H.jpg"					// �ǂݍ��ރe�N�X�`��
#define MAX_TEXTURE			(1)										// �g�p����e�N�X�`����
#define MAX_SET_TEXTURE		(1)										// �Z�b�g����e�N�X�`����
#define MAX_TUTORIAL		(1)										// �`���[�g���A���̍ő吔
#define DESCRIPTION_POS_X	(0)										// ������X���W
#define DESCRIPTION_POS_Y	(0)										// ������Y���W
#define TEXTURE_POS_X		(660)									// �`���[�g���A����X���W
#define TEXTURE_POS_Y		(10)									// �`���[�g���A����Y���W
#define TEXTURE_WIDTH		(180)									// �`���[�g���A���̕�
#define TEXTURE_HEIGHT		(40)									// �`���[�g���A���̍���
#define TEXT_POS_X			(900)									// �e�L�X�g��X���W
#define TEXT_POS_Y			(640)									// �e�L�X�g��Y���W
#define TEXT_WIDTH			(360)									// �e�L�X�g�̕�
#define TEXT_HEIGHT			(60)									// �e�L�X�g�̍���

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureTutorial[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTutorial = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

TUTOSTATE					g_tutoState;							// �`���[�g���A���̏��
int							g_nCntTutoState;						// �`���[�g���A����ԃJ�E���^�[

//=============================================================================
// �`���[�g���A����ʂ̏�����
//=============================================================================
void InitTutorial(HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ������
	g_tutoState = TUTOSTATE_NORMAL;// �ʏ��Ԃɐݒ�
	g_nCntTutoState = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_pTextureTutorial[0]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SET_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
// �`���[�g���A����ʂ̏I������
//=============================================================================
void UninitTutorial()
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_pTextureTutorial[nCntTex] != NULL)
		{
			g_pTextureTutorial[nCntTex]->Release();
			g_pTextureTutorial[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=============================================================================
// �`���[�g���A����ʂ̍X�V����
//=============================================================================
void UpdateTutorial()
{
	if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(0, JOYPADKEY_START) || GetJoypadTrigger(0, JOYPADKEY_A) ||
		GetJoypadTrigger(1, JOYPADKEY_START) || GetJoypadTrigger(1, JOYPADKEY_A) ||
		GetJoypadTrigger(2, JOYPADKEY_START) || GetJoypadTrigger(2, JOYPADKEY_A) ||
		GetJoypadTrigger(3, JOYPADKEY_START) || GetJoypadTrigger(3, JOYPADKEY_A))
	{
		if (GetFade() != FADE_OUT)
		{// ���[�h�̐ݒ�
			SetFade(MODE_GAME);
		}
	}
}

//=============================================================================
// �`���[�g���A����ʂ̕`�揈��
//=============================================================================
void DrawTutorial()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X���擾����

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[�~���O�Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial[0]);

	// �|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �`���[�g���A����Ԃ̐ݒ�
//=============================================================================
void SetTutoState(TUTOSTATE state)
{
	g_tutoState = state;
	g_nCntTutoState = 0;
}

//=============================================================================
// �`���[�g���A����Ԃ̎擾
//=============================================================================
TUTOSTATE *GetTutoState(void)
{
	return &g_tutoState;
}