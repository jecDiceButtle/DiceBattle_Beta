
//**************************************************************************
//メッシュ構造体
//基本型　：　DxBaseMesh
//派生型　：　なし
//**************************************************************************

#pragma once

#include "dx_base_mesh.h"

//メッシュ構造体
typedef struct DxMesh : public DxBaseMesh
{
	//メッシュ情報
	LPD3DXMESH				lpMesh;
	//マテリアル数
	DWORD					matCount;
	//マテリアル郡
	D3DMATERIAL9			*pMat;
	//テクスチャ郡
	LPDIRECT3DTEXTURE9		*pTex;

	//コンストラクタ
	DxMesh();
	//ファイル名の取得
	string GetFileName();
}DxMesh;
