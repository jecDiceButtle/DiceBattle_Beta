
//**************************************************************************
//メッシュデータ管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#pragma once
#include "dx.h"

#include "dx_base_mesh.h"
#include "dx_mesh.h"
#include "dx_anime_mesh.h"

#include "texture_manager.h"

//メッシュデータ管理用クラス
class MeshManager
{
	private:
		//デバイスアドレス
		LPDIRECT3DDEVICE9 dev;
		//テクスチャ管理クラス
		TextureManager *t_manager;
		//
		Dx_Hierarchy *hierarchy;

		//登録メッシュ郡
		std::vector<DxBaseMesh*> mesh_data;
		//登録メッシュ数
		short mesh_val;

	public:
		//コンストラクタ
		MeshManager(LPDIRECT3DDEVICE9 dev,TextureManager *t_manager);
		//デストラクタ
		~MeshManager();

		/*メッシュデータ関連*/
		//Xファイルからの３Ｄモデルデータをロード　　（簡易版、アニメーション制御不可）
		DxMesh* LoadMeshFromX(const string& fName,const string& path);

		/*アニメーションメッシュデータ関連*/
		//Xファイルからの３Ｄモデルデータをロード　　（詳細版、アニメーション制御可能）
		DxAnimeMesh* LoadAnimeMeshFromX(const string& fName,const string& path);

	private:
		/*メッシュデータ関連*/
		//Xファイルからの３Ｄモデルデータをロード　　（簡易版、アニメーション制御不可）
		HRESULT LoadMeshFromX(DxMesh *mesh,const string& fName,const string& path);
		//DxMeshの内部データの破棄および開放
		void ReleaseMesh(DxBaseMesh *base_mesh);

		/*アニメーションメッシュデータ関連*/
		//Xファイルからの３Ｄモデルデータをロード　　（詳細版、アニメーション制御可能）
		HRESULT LoadAnimeMeshFromX(DxAnimeMesh *mesh,const string& fName,const string& path);
		//アニメのみ別に読み込み作成
		HRESULT LoadAnimeFromX(LPD3DXANIMATIONCONTROLLER &pAnimControllerTmp,DxAnimeMesh *mesh,const string& fName,const string& path);

		//DxAnimeMeshの内部データの破棄および開放
		void ReleaseAnimeMesh(DxBaseMesh *base_mesh);

		/*ボーン設定関連*/
		//フレーム内を検索してメッシュコンテナ内のボーンを設定
		HRESULT SetupBoneFromFrame(LPD3DXFRAME pFrameRoot,LPD3DXFRAME pFrame);
		//メッシュコンテナ内のボーンを設定
		HRESULT SetupBoneFromMeshContainer(LPD3DXFRAME pFrameRoot,LPD3DXMESHCONTAINER pMeshCont);

		//登録済みメッシュデータのファイル名をチェックし重複ロードを防止する
		DxBaseMesh* CheckFileName(const string& fName);
};