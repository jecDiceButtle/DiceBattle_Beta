
//**************************************************************************
//メッシュ構造体
//基本型　：　DxBaseMesh
//派生型　：　なし
//**************************************************************************

#include "dx_mesh.h"


//コンストラクタ
DxMesh::DxMesh() : DxBaseMesh()
{
	this->lpMesh = NULL;
	this->matCount = 0;
	this->pMat = NULL;
	this->pTex = NULL;
}
//ファイル名の取得
string DxMesh::GetFileName()
{
	return this->file_name;
}