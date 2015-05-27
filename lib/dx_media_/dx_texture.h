
//**************************************************************************
//テクスチャ構造体
//基本型　：　なし
//派生型　：　DxPieceTexture
//**************************************************************************

#pragma once

#include "dx.h"

//テクスチャ構造体
typedef struct DxTexture
{
	//テクスチャデータ
	LPDIRECT3DTEXTURE9 lpTex;
	//テクスチャ情報
	D3DXIMAGE_INFO info;
	//ファイル名
	string file_name;

	//コンストラクタ
	DxTexture();
}DxTexture;
