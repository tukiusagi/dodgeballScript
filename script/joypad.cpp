//=============================================================================
//
// �W���C�p�b�h���� [joypad.cpp]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#include "joypad.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�J�n�܂ł̑҂�����
#define	INTERVAL_REPEAT		(1)		// ���s�[�g�Ԋu

#define JOY_MAX_RANGE		(1024)	// �p�b�h�L���͈�
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECTINPUT8			g_pJoypad = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevJoypad = NULL;					// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

BYTE					g_aJoyKeyState[NUM_KEY_MAX];			// �W���C�p�b�h�̓��͏�񃏁[�N
int						g_nCountWaitRepeat = 0;					// ���s�[�g�҂�����
int						g_nIntervalRepeat = 0;					// ���s�[�g�Ԋu

LPDIRECTINPUTDEVICE8	g_apDIDevJoypad[NUM_JOYPAD_MAX] = {};						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
DIJOYSTATE2				g_aJoypadState[NUM_JOYPAD_MAX];								// �W���C�p�b�h��Ԃ��󂯎�郏�[�N
bool					g_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];				// �W���C�p�b�h�̉�����Ԃ�ێ����郏�[�N
bool					g_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// �W���C�p�b�h�̃g���K�[��Ԃ�ێ����郏�[�N
bool					g_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// �W���C�p�b�h�̃����[�X��Ԃ�ێ����郏�[�N
bool					g_aKeyStateRepeatJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];		// �W���C�p�b�h�̃��s�[�g��Ԃ�ێ����郏�[�N
int						g_aKeyStateRepeatCntJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];	// �W���C�p�b�h�̃��s�[�g�J�E���^
D3DXVECTOR3             g_aKeyStateAxis[NUM_JOYPAD_MAX];
int						g_nCntPad = 0;
int						g_nNumPad = 4;

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (g_pJoypad == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pJoypad, NULL);

		g_nCountWaitRepeat = COUNT_WAIT_REPEAT;
		g_nIntervalRepeat = INTERVAL_REPEAT;
	}

	for (g_nCntPad = 0; g_nCntPad < NUM_JOYPAD_MAX; g_nCntPad++)
	{
		hr = g_pJoypad->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || g_apDIDevJoypad[g_nCntPad] == NULL)
		{
			//MessageBox(hWnd, "�W���C�p�b�h������܂���", "�x��", MB_ICONWARNING);// �m�F�p
			return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = g_apDIDevJoypad[g_nCntPad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = g_apDIDevJoypad[g_nCntPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		hr = g_apDIDevJoypad[g_nCntPad]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h��������܂���ł���", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �f�o�C�X�̐ݒ�
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);

		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

        // �w�����͈̔͂��w��
		g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);	

		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �x�����͈̔͂��w��
		g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);	

		diprg.diph.dwObj = DIJOFS_Z;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

        // �y�����͈̔͂��w��
		g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);	

		diprg.diph.dwObj = DIJOFS_RZ;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -JOY_MAX_RANGE;
		diprg.lMax = JOY_MAX_RANGE;

		// �q�y�����͈̔͂��w��
		g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);	

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_apDIDevJoypad[g_nCntPad]->Acquire();

		// ���[�N�N���A
		memset(&g_aJoypadState[g_nCntPad], 0, sizeof g_aJoypadState[g_nCntPad]);
		memset(g_aKeyStateJoypad[g_nCntPad], 0, sizeof g_aKeyStateJoypad[g_nCntPad]);
		memset(g_aKeyStateTriggerJoypad[g_nCntPad], 0, sizeof g_aKeyStateTriggerJoypad[g_nCntPad]);
		memset(g_aKeyStateReleaseJoypad[g_nCntPad], 0, sizeof g_aKeyStateReleaseJoypad[g_nCntPad]);
		memset(g_aKeyStateRepeatJoypad[g_nCntPad], 0, sizeof g_aKeyStateRepeatJoypad[g_nCntPad]);
	}

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitJoypad(void)
{
	int nCntJoypad;

	// �W���C�p�b�h�̏I������
	for (nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (g_apDIDevJoypad[nCntJoypad] != NULL)
		{// �f�o�C�X�I�u�W�F�N�g�̊J��
			g_apDIDevJoypad[nCntJoypad]->Release();
			g_apDIDevJoypad[nCntJoypad] = NULL;
		}
	}

	if (g_pJoypad != NULL)
	{
		g_pJoypad->Release();
		g_pJoypad = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateJoypad(void)
{
	HRESULT hr;
	int nCntJoypad;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (nCntJoypad = 0; nCntJoypad < NUM_JOYPAD_MAX; nCntJoypad++)
	{
		if (g_apDIDevJoypad[nCntJoypad] == NULL)
		{
			return;
		}

		// �O��̃f�[�^��ۑ�
		for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = g_aKeyStateJoypad[nCntJoypad][nCntKey];
		}

		// �f�o�C�X����f�[�^���擾
		hr = g_apDIDevJoypad[nCntJoypad]->GetDeviceState(sizeof(g_aJoypadState[nCntJoypad]), &g_aJoypadState[nCntJoypad]);
		if (SUCCEEDED(hr))
		{
			// �L�[���ݒ�
			SetKeyStateJoypad(nCntJoypad);

			//	���ʒu���L�^
			g_aKeyStateAxis[nCntJoypad].x = (float)g_aJoypadState[nCntJoypad].lX / JOY_MAX_RANGE;
			g_aKeyStateAxis[nCntJoypad].y = (float)g_aJoypadState[nCntJoypad].lY / JOY_MAX_RANGE;
			g_aKeyStateAxis[nCntJoypad].z = (float)g_aJoypadState[nCntJoypad].lZ / JOY_MAX_RANGE;

			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				// �g���K�[���̍쐬
				g_aKeyStateTriggerJoypad[nCntJoypad][nCntKey] =
					(aKeyStateOld[nCntKey] ^ g_aKeyStateJoypad[nCntJoypad][nCntKey]) & g_aKeyStateJoypad[nCntJoypad][nCntKey];

				// �����[�X���̍쐬
				g_aKeyStateReleaseJoypad[nCntJoypad][nCntKey] =
					(aKeyStateOld[nCntKey] ^ g_aKeyStateJoypad[nCntJoypad][nCntKey]) & !g_aKeyStateJoypad[nCntJoypad][nCntKey];

				// ���s�[�g���̍쐬
				g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = g_aKeyStateTriggerJoypad[nCntJoypad][nCntKey];
				if (g_aKeyStateJoypad[nCntJoypad][nCntKey])
				{
					g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey]++;
					if (g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] < g_nCountWaitRepeat)
					{
						if (g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] == 1
							|| g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] >= g_nCountWaitRepeat)
						{
							g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = g_aKeyStateJoypad[nCntJoypad][nCntKey];
						}
						else
						{
							g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = 0;
						}
					}
					else
					{
						if (((g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] - g_nCountWaitRepeat) % g_nIntervalRepeat) == 0)
						{
							g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = g_aKeyStateJoypad[nCntJoypad][nCntKey];
						}
						else
						{
							g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = 0;
						}
					}
				}
				else
				{
					g_aKeyStateRepeatCntJoypad[nCntJoypad][nCntKey] = 0;
					g_aKeyStateRepeatJoypad[nCntJoypad][nCntKey] = 0;
				}
			}
		}
		else
		{
			g_apDIDevJoypad[nCntJoypad]->Acquire();
		}
	}
}
//=============================================================================
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[NUM_JOYPAD_MAX];	// �e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS	diDevCaps;							// �f�o�C�X���
	HRESULT		hRes;

	for (int nCnt = 0; nCnt < NUM_JOYPAD_MAX; nCnt++)
	{
		if (pad_discrimination[nCnt] == lpddi->guidInstance)
		{
			return DIENUM_CONTINUE;			// �񋓂𑱂���
		}
	}

	// �W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = g_pJoypad->CreateDevice(lpddi->guidInstance, &g_apDIDevJoypad[g_nCntPad], NULL);
	if (FAILED(hRes))
	{
		return DIENUM_CONTINUE;			// �񋓂𑱂���
	}

	// �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = g_apDIDevJoypad[g_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (g_apDIDevJoypad[g_nCntPad])
		{
			g_apDIDevJoypad[g_nCntPad]->Release();
			g_apDIDevJoypad[g_nCntPad] = NULL;
		}
		return DIENUM_CONTINUE;			// �񋓂𑱂���
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[g_nCntPad] = lpddi->guidInstance;

	return DIENUM_STOP;		// ���̃f�o�C�X���g���̂ŗ񋓂��I������
}

BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;

	DIPROPRANGE diprg;

	// �X�e�B�b�N���̒l�͈̔͂�ݒ�i-32768�`32767�j
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -32768;
	diprg.lMax = 32767;

	if (g_apDIDevJoypad[g_nCntPad] != NULL)
	{
		hr = g_apDIDevJoypad[g_nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr))
		{
			return DIENUM_STOP;
		}
	}
	
	return DIENUM_CONTINUE;
}

//=============================================================================
// �W���C�p�b�h�̃L�[���ݒ�
//=============================================================================
void SetKeyStateJoypad(int nIDPad)
{
	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{// �\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{// �\���L�[[�E]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((g_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100)
		|| (g_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{// �\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (g_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && g_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{// �\���L�[[��]��������Ă���
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		g_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (g_aJoypadState[nIDPad].rgbButtons[nKey])
		{// �{�^����������Ă���
			g_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			g_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//=============================================================================
// �W���C�p�b�h���擾�Q������
//=============================================================================
bool GetJoypadPress(int nIDPad, JOYPADKEY key)
{
	return g_aKeyStateJoypad[nIDPad][key];
}

//=============================================================================
// �W���C�p�b�h���擾�Q����
//=============================================================================
bool GetJoypadTrigger(int nIDPad, JOYPADKEY key)
{
	return g_aKeyStateTriggerJoypad[nIDPad][key];
}

//=============================================================================
// �W���C�p�b�h���擾�Q����
//=============================================================================
bool GetJoypadRelease(int nIDPad, JOYPADKEY key)
{
	return g_aKeyStateReleaseJoypad[nIDPad][key];
}

//=============================================================================
// �W���C�p�b�h���擾�Q�J��Ԃ�
//=============================================================================
bool GetJoypadRepeat(int nIDPad, JOYPADKEY key)
{
	return g_aKeyStateRepeatJoypad[nIDPad][key];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[(��)�̒l�擾
//=============================================================================
int GetJoypadTriggerLeft(int nIDPad)
{
	return g_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[(�E)�̒l�擾
//=============================================================================
int GetJoypadTriggerRight(int nIDPad)
{
	return g_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N(��)�̒l�擾
//=============================================================================
void GetJoypadStickLeft(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = g_aJoypadState[nIDPad].lX;
	*pValueV = -g_aJoypadState[nIDPad].lY;
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N(�E)�̒l�擾
//=============================================================================
void GetJoypadStickRight(int nIDPad, int *pValueH, int *pValueV)
{
	*pValueH = g_aJoypadState[nIDPad].lZ;
	*pValueV = -g_aJoypadState[nIDPad].lRz;
}

//=============================================================================
// �v���C���[���̐ݒ�
//=============================================================================
void SetNumPad(int nNum)
{
	g_nNumPad = nNum;
}

//=============================================================================
// �v���C���[���̎擾
//=============================================================================
int GetNumPad(void)
{
	return g_nNumPad;
}