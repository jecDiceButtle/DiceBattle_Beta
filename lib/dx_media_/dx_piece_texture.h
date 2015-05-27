
//**************************************************************************
//部分テクスチャ構造体
//基本型　：　DxTexture
//派生型　：　なし
//**************************************************************************

#pragma once

#include "dx_texture.h"

//?
typedef struct DxPieceTexture : public DxTexture
{
	//テクスチャ上の表示領域
	RECT tex_pos;
	//切り取ったテクスチャから見た中心座標
	DxVec2 tex_center;
	//カラー情報
	D3DCOLOR color;

	//コンストラクタ
	DxPieceTexture();
}DxPieceTexture;