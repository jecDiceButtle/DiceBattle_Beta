
//**************************************************************************
//メッシュ関連の基本構造体
//基本型　：　なし
//派生型　：　DxMesh,DxAnimeMesh
//**************************************************************************

#pragma once

#include "dx.h"

//メッシュ関連の基本構造体
typedef struct DxBaseMesh
{
	//ファイル名
	string file_name;

	//コンストラクタ
	DxBaseMesh();
	//ファイル名の取得
	virtual string GetFileName();
}DxBaseMesh;
