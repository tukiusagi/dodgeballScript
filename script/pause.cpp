//=============================================================================
//
// �|�[�Y��ʏ��� [pause.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "pause.h"
#include "joypad.h"
#include "keyboard.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTUREFILE0	"data/TEXTURE/bg_pause.jpg"	// �ǂݍ��ރe�N�X�`��1
#define TEXTUREFILE1	"data/TEXTURE/pause100.jpg"	// �ǂݍ��ރe�N�X�`��2
#define TEXTUREFILE2	"data/TEXTURE/pause000.png"	// �ǂݍ��ރe�N�X�`��3
#define TEXTUREFILE3	"data/TEXTURE/pause001.png"	// �ǂݍ��ރe�N�X�`��4
#define TEXTUREFILE4	"data/TEXTURE/pause002.png"	// �ǂݍ��ރe�N�X�`��5
#define TEXTUREFILE5	"data/TEXTURE/arrow.png"	// �ǂݍ��ރe�N�X�`��6
#define MAX_TEXTURE		(6)							// �e�N�X�`����
#define BG_POS_X		(0)							// �w�i��X���W
#define BG_POS_Y		(0)							// �w�i��Y���W
#define PAUSE_POS_X		(640)						// �|�[�Y�̘g��X���W
#define PAUSE_POS_Y		(360)						// �|�[�Y�̘g��Y���W
#define CONTINUE_POS_Y	(300)						// �R���e�j���[��Y���W
#define RETEY_POS_Y		(400)						// ���g���C��Y���W
#define QUIT_POS_Y		(500)						// �I����Y���W
#define PAUSE_WIDTH		(500)						// �|�[�Y�̘g�̕�
#define CONTINUE_WIDTH	(300)						// �R���e�j���[�̕�
#define CONTINUE_HEIGHT	(50)						// �R���e�j���[�̍���
#define RETEY_WIDTH		(300)						// �R���e�j���[�̕�
#define RETEY_HEIGHT	(50)						// �R���e�j���[�̍���
#define QUIT_WIDTH		(300)						// �R���e�j���[�̕�
#define QUIT_HEIGHT		(50)						// �R���e�j���[�̍���
#define ARROW_WIDTH		(50)						// ���̕�
#define ARROW_HEIGHT	(50)						// ���̍���

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTexturePause[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffPause = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

ARROW					g_arrow;							// ���̏��
JOYPADUSE				g_PausePadUse;						// �W���C�p�b�h�̎g�p��Ԃ̏��

//=============================================================================
// �|�[�Y��ʂ̏�����
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X�̎擾

	// ������
	g_arrow.pos = D3DXVECTOR3(465, CONTINUE_POS_Y, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE0, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE1, &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE2, &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE3, &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE4, &g_pTexturePause[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTUREFILE5, &g_pTexturePause[5]);

	// ���_���̍쐬
	MakeVertexPause(pDevice);
}

//=============================================================================
// �|�[�Y��ʂ̉������
//=============================================================================
void UninitPause(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// �|�[�Y��ʂ̍X�V����
//=============================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^
	int nStickH, nStickV;

	// �A�i���O�X�e�B�b�N���̎擾
	GetJoypadStickLeft(0, &nStickH, &nStickV);

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoypadTrigger(0, JOYPADKEY_UP)
		|| GetJoypadTrigger(1, JOYPADKEY_UP) 
		|| GetJoypadTrigger(2, JOYPADKEY_UP) 
		|| GetJoypadTrigger(3, JOYPADKEY_UP) || nStickV > 0)
	{
		if (g_PausePadUse.UpThumb == false)
		{
			//PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�

			if (g_arrow.pos.y == CONTINUE_POS_Y)
			{

			}
			else if (g_arrow.pos.y == RETEY_POS_Y)
			{
				g_arrow.pos.y = CONTINUE_POS_Y;
			}
			else if (g_arrow.pos.y == QUIT_POS_Y)
			{
				g_arrow.pos.y = RETEY_POS_Y;
			}
			g_PausePadUse.UpThumb = true;
		}
	}
	else if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoypadTrigger(0, JOYPADKEY_DOWN)
		|| GetJoypadTrigger(1, JOYPADKEY_DOWN) 
		|| GetJoypadTrigger(2, JOYPADKEY_DOWN) 
		|| GetJoypadTrigger(3, JOYPADKEY_DOWN) || nStickV < 0)
	{
		if (g_PausePadUse.DownThumb == false)
		{
			//PlaySound(SOUND_LABEL_SE_POUSE_0);// ���ʉ��̐ݒ�

			if (g_arrow.pos.y == CONTINUE_POS_Y)
			{
				g_arrow.pos.y = RETEY_POS_Y;
			}
			else if (g_arrow.pos.y == RETEY_POS_Y)
			{
				g_arrow.pos.y = QUIT_POS_Y;
			}
			else if (g_arrow.pos.y == QUIT_POS_Y)
			{

			}
			g_PausePadUse.DownThumb = true;
		}
	}
	else
	{
		g_PausePadUse.UpThumb = false;
		g_PausePadUse.DownThumb = false;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * 5;// ���܂Ői�߂�

	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_arrow.pos.x - (ARROW_WIDTH / 2), g_arrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_arrow.pos.x + (ARROW_WIDTH / 2), g_arrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_arrow.pos.x - (ARROW_WIDTH / 2), g_arrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_arrow.pos.x + (ARROW_WIDTH / 2), g_arrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);

	// ���_�f�[�^���A�����b�N
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// �|�[�Y��ʂ̕`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// �f�o�C�X���擾����

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[�~���O�Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < MAX_TEXTURE; nCntPause++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		// �|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// �������Ȕw�i
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_POS_X + SCREEN_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_POS_X + SCREEN_WIDTH, BG_POS_Y + SCREEN_HEIGHT, 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 150);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �|�[�Y���j���[�̘g
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (PAUSE_WIDTH / 2), PAUSE_POS_Y - (PAUSE_WIDTH / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (PAUSE_WIDTH / 2), PAUSE_POS_Y - (PAUSE_WIDTH / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (PAUSE_WIDTH / 2), PAUSE_POS_Y + (PAUSE_WIDTH / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (PAUSE_WIDTH / 2), PAUSE_POS_Y + (PAUSE_WIDTH / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �R���e�j���[
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (CONTINUE_WIDTH / 2), CONTINUE_POS_Y - (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (CONTINUE_WIDTH / 2), CONTINUE_POS_Y - (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (CONTINUE_WIDTH / 2), CONTINUE_POS_Y + (CONTINUE_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (CONTINUE_WIDTH / 2), CONTINUE_POS_Y + (CONTINUE_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// ���g���C
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y - (RETEY_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (RETEY_WIDTH / 2), RETEY_POS_Y + (RETEY_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// �I��
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y - (QUIT_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X - (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + (QUIT_WIDTH / 2), QUIT_POS_Y + (QUIT_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;// ���_�f�[�^�̃|�C���^��4���i�߂�

	// ���
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_arrow.pos.x - (ARROW_WIDTH / 2), g_arrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_arrow.pos.x + (ARROW_WIDTH / 2), g_arrow.pos.y - (ARROW_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_arrow.pos.x - (ARROW_WIDTH / 2), g_arrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_arrow.pos.x + (ARROW_WIDTH / 2), g_arrow.pos.y + (ARROW_HEIGHT / 2), 0.0f);

	// �������W
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// �����̎擾
//=============================================================================
ARROW *GetArrow(void)
{
	return &g_arrow;
}