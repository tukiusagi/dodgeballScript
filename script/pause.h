//=============================================================================
//
// ポーズ画面処理 [pause.h]
// Author : YUKI TAKAHASHI
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 構造体定義＿[マクロ定義が上にないとマジックナンバーがわからない]
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
} ARROW;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

ARROW *GetArrow(void);
#endif