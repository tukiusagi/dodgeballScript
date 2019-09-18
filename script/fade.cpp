//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	TEXTURE_FADE	"data/TEXTURE/fade0.jpg"	// �ǂݍ��ރt�F�[�h�e�N�X�`��(���͎g�p���Ȃ�)

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

FADE					g_fade;						// �t�F�[�h���
MODE					g_modeNext;					// ���̉�ʁi���[�h�j
D3DXCOLOR				g_colorFade;				// �t�F�[�h�F

HWND					g_fade_hWnd;				// ���ʎq�ۑ��p

//=============================================================================
// ����������
//=============================================================================
void InitFade(MODE modeNext, HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ������
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);// ������ʁi�s�����j
	g_fade_hWnd = hWnd;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FADE, &g_pTextureFade);

	// ���_���̐ݒ�
	MakeVertexFade(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	// �e�N�X�`���̊J��
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if(g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;// ���_�o�b�t�@�ւ̃|�C���^

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)// �t�F�[�h�C������
		{
			g_colorFade.a -= 0.0375f;// ��ʂ𓧖��ɂ��Ă���
			if (g_colorFade.a < 0.0f)
			{
				// �t�F�[�h�C���I��
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT)// �t�F�[�h�A�E�g����
		{
			g_colorFade.a += 0.0375f;// ��ʂ�s�����ɂ��Ă���
			if (g_colorFade.a > 1.0f)
			{
				// �t�F�[�h�C���ɐ؂�ւ�
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				// ���[�h�ݒ�
				SetMode(g_modeNext,g_fade_hWnd);
			}
		}
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̍X�V
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = g_colorFade;

		// ���_�f�[�^���A�����b�N
		g_pVtxBuffFade->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);
	
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// �������W�̐ݒ�
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = g_colorFade;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}

