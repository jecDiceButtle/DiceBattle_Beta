
//**************************************************************************
//メディア管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#pragma once

#include "texture_manager.h"
#include "mesh_manager.h"

//メディア管理クラス
class MediaManager
{
	private:
		//テクスチャ管理クラス
		TextureManager *tex_manager;
		//メッシュ管理クラス
		MeshManager *mesh_manager;

	public:
		//コンストラクタ
		MediaManager(LPDIRECT3DDEVICE9 dev);
		//デストラクタ
		~MediaManager();

		/*テクスチャ関連*/
		//イメージファイルからDxTextureをロード　　（簡易版）
		LPDIRECT3DTEXTURE9 LoadTextureFromFile(string fName,string path);
		//イメージファイルからDxTextureをロード　　（詳細版）
		DxTexture* LoadTextureFromFileEx(string fName,string path);

		/*３Ｄモデル関連*/
		//Xファイルからの３Ｄモデルデータをロード　　（簡易版、アニメーション制御不可）
		DxMesh* LoadMeshFromX(string fName,string path);
		//Xファイルからの３Ｄモデルデータをロード　　（詳細版、アニメーション制御可能）
		DxAnimeMesh* LoadAnimeMeshFromX(string fName,string path);
};

