//=============================================================================
//
// カウントダウン処理 [time.h]
// Author : YONEYAMA TAKESHI
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int			nNm;
	bool		bUse;	//使用状態
} TIME;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

void AddTime(int nValue);

#endif
