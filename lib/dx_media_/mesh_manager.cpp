
//**************************************************************************
//メッシュデータ管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************


#include "mesh_manager.h"


//コンストラクタ
MeshManager::MeshManager(LPDIRECT3DDEVICE9 dev,TextureManager *t_manager)
{
	//デバイスポインタの設定
	this->dev = dev;
	//テクスチャ管理クラスポインタの設定
	this->t_manager = t_manager;

	//Dx_Hierarchyの生成
	this->hierarchy = new Dx_Hierarchy(t_manager);

	//登録メッシュ数の初期化
	this->mesh_val = 0;
}

//デストラクタ
MeshManager::~MeshManager()
{
	//
	vector<DxBaseMesh*>::iterator it;

	//vector内のオブジェクトをすべて破棄および開放
	for(it=this->mesh_data.begin();it!=this->mesh_data.end();it++)
	{
		//登録オブジェクトがDxMeshの場合
		if(dynamic_cast<DxMesh*>(*it)!=0)
		{
			this->ReleaseMesh((*it));
			continue;
		}
		//登録オブジェクトがDxAnimeMeshの場合
		if(dynamic_cast<DxAnimeMesh*>(*it)!=0)
		{
			this->ReleaseAnimeMesh((*it));
			continue;
		}
	}
	this->mesh_data.clear();

	//
	SAFE_DELETE(this->hierarchy);
	//
	this->t_manager = NULL;
	//
	this->dev = NULL;
}

//**************************************************************************
//外部参照用ロード関数
//**************************************************************************
//Xファイルからの３Ｄモデルデータをロード　　（簡易版、アニメーション制御不可）
DxMesh* MeshManager::LoadMeshFromX(const string& fName,const string& path)
{
	//生成用オブジェクトの宣言
	DxMesh *create = NULL;

	//重複ロード防止のためファイル名チェックを行う
	if((create=(DxMesh*)this->CheckFileName(fName))!=NULL)
	{
		//登録済みオブジェクトを戻り値とする
		return create;
	}

	//オブジェクトの生成
	create = new DxMesh;
	//生成用オブジェクトにパス及びファイル名からロード
	if(FAILED(this->LoadMeshFromX(create,fName,path)))
	{
		//ロードに失敗した場合、領域確保した情報を破棄
		SAFE_DELETE(create);
		gplib::debug::BToMR("%sの読み込みに失敗", fName.c_str());
		return NULL;
	}

	//メッシュ郡に登録
	this->mesh_data.push_back(create);
	//メッシュデータの総数を加算
	this->mesh_val++;

	//新たに生成されたオブジェクトのアドレスを返す
	return create;
}
//Xファイルからの３Ｄモデルデータをロード　　（詳細版、アニメーション制御可能）
DxAnimeMesh* MeshManager::LoadAnimeMeshFromX(const string& fName,const string& path)
{
	//生成用オブジェクトの宣言
	DxAnimeMesh *create = NULL;

	//重複ロード防止のためファイル名チェックを行う
	if((create=(DxAnimeMesh*)this->CheckFileName(fName))!=NULL)
	{
		//登録済みオブジェクトを戻り値とする
		this->LoadAnimeFromX(create->pAnimController,create,fName,path);
		return create;
	}

	//オブジェクトの生成
	create = new DxAnimeMesh;

	//生成用オブジェクトにパス及びファイル名からロード
	if(FAILED(this->LoadAnimeMeshFromX(create,fName,path)))
	{
		//ロードに失敗した場合、領域確保した情報を破棄
		SAFE_DELETE(create);
		gplib::debug::BToMR("%sの読み込みに失敗", fName.c_str());
		return NULL;
	}

	//メッシュ郡に登録
	this->mesh_data.push_back(create);
	//メッシュデータの総数を加算
	this->mesh_val++;

	//生成したオブジェクトのアドレスを戻す
	return create;
}

//**************************************************************************
//内部参照用ロード関数
//**************************************************************************
//Xファイルからの３Ｄモデルデータをロード　　（簡易版、アニメーション制御不可）
HRESULT MeshManager::LoadMeshFromX(DxMesh *mesh,const string& fName,const string& path)
{
	//ロード用バッファ
	LPD3DXBUFFER		lpD3DBuffer;
	D3DXMATERIAL		*d3dxMat;

	string pathfilename = path + fName;
	//メッシュデータのロード
	auto ret = D3DXLoadMeshFromX(pathfilename.c_str(), D3DXMESH_SYSTEMMEM, this->dev,
		nullptr, &lpD3DBuffer, NULL, &mesh->matCount, &mesh->lpMesh);
	if(FAILED(ret))
	{
		return E_FAIL;
	}

	//ファイル名の設置
	mesh->file_name+=fName;

	//ロード用バッファからマテリアル情報郡を取得
	d3dxMat = (D3DXMATERIAL*)lpD3DBuffer->GetBufferPointer();

	//マテリアル数分のマテリアル配列を生成
	mesh->pMat=new D3DMATERIAL9[mesh->matCount];
	//マテリアル数分のテクスチャ配列を生成
	mesh->pTex = new LPDIRECT3DTEXTURE9[mesh->matCount];

	//マテリアル数分繰り返す
	for(DWORD i=0;i<mesh->matCount;i++)
	{
		//マテリアル情報の取得
		mesh->pMat[i] = d3dxMat[i].MatD3D;
		mesh->pMat[i].Ambient = mesh->pMat[i].Diffuse;
		//テクスチャ情報の初期化
		mesh->pTex[i] = NULL;
		//ファイル名を取得
		LPSTR tex_name = d3dxMat[i].pTextureFilename;
		//マテリアル情報に名前の情報が存在する場合、テクスチャ情報を取得
		if(d3dxMat[i].pTextureFilename!=NULL)
		{
			mesh->pTex[i] = this->t_manager->LoadTextureFromFile(tex_name,path);
		}
	}

	//ロード用バッファ内の情報を開放
	lpD3DBuffer->Release();

	return S_OK;
}
//Xファイルからの３Ｄモデルデータをロード　　（詳細版、アニメーション制御可能）
HRESULT MeshManager::LoadAnimeMeshFromX(DxAnimeMesh *mesh,const string& fName,const string& path)
{
	//読み込み用アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER pAnimControllerTmp = NULL;

	string str = path + fName;

	//Xファイルからフレーム階層とアニメーション情報をロード
  if(FAILED(D3DXLoadMeshHierarchyFromX(
		str.c_str(),
		D3DXMESH_MANAGED,
		this->dev,
		this->hierarchy,
		NULL,
		&mesh->pFrameRoot,
		&pAnimControllerTmp)))
	{
		return E_FAIL;
	}

	//ファイル名の設置
	mesh->file_name=fName;

	if(FAILED(LoadAnimeFromX(pAnimControllerTmp,mesh,fName,path)))
	{
		return E_FAIL;
	}
	SAFE_RELEASE(pAnimControllerTmp);
	return S_OK;
}

HRESULT MeshManager::LoadAnimeFromX(LPD3DXANIMATIONCONTROLLER &pAnimControllerTmp,DxAnimeMesh *mesh,
																		const string& fName,const string& path)
{
	//読み込み用アニメーションコントローラ
//	LPD3DXANIMATIONCONTROLLER pAnimControllerTmp = NULL;

	/*アニメーションコントローラ関連*/
	//アニメーションコントローラの設定
	if(pAnimControllerTmp!=NULL)
	{
		//トラック数をアニメーションセット数に増やす
		unsigned TracksNum   = pAnimControllerTmp->GetMaxNumTracks();
		unsigned AnimSetsNum = pAnimControllerTmp->GetMaxNumAnimationSets();

		//
		if(TracksNum < AnimSetsNum)
		{
			TracksNum = AnimSetsNum;
		}

		//読み込んだアニメーションコントローラから新たに設定された複製を生成
		if(FAILED(pAnimControllerTmp->CloneAnimationController(
					pAnimControllerTmp->GetMaxNumAnimationOutputs(),
					pAnimControllerTmp->GetMaxNumAnimationSets(),
					TracksNum,
					pAnimControllerTmp->GetMaxNumEvents(),
					&mesh->pAnimController)))
		{
		}
		//読み込み用アニメーションコントローラの開放
//		SAFE_RELEASE(pAnimControllerTmp);

		//トラックに全てのアニメーションセットを読み込む
		for(unsigned i = 1; i < AnimSetsNum; i++ )
		{
			//読み込み用オブジェクトの宣言
			LPD3DXANIMATIONSET pAnimSet = NULL;
			//アニメーションコントローラからアニメーションセットを読み込む
			if(FAILED(mesh->pAnimController->GetAnimationSet(i, &pAnimSet)))
			{
			}
			//指定トラックにアニメーションセットを設定
			if(FAILED(mesh->pAnimController->SetTrackAnimationSet(i, pAnimSet)))
			{
			}
			//トラックに設定済みのアニメーションセットを開放
			SAFE_RELEASE(pAnimSet);
		}
	}
	else
	{
		mesh->pAnimController = NULL;
	}

	//フレームの全階層をチェックしてボーンの関連付けを行う
	if(FAILED((this->SetupBoneFromFrame(mesh->pFrameRoot, mesh->pFrameRoot))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//**************************************************************************
//ボーン設定関連
//**************************************************************************
//フレーム内を検索してメッシュコンテナ内のボーンを設定
HRESULT MeshManager::SetupBoneFromFrame(LPD3DXFRAME pFrameRoot,LPD3DXFRAME pFrame)
{
	//メッシュコンテナが存在する場合
	if(pFrame->pMeshContainer!=NULL)
	{
		//メッシュコンテナのボーンを設定
		if(FAILED(this->SetupBoneFromMeshContainer(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	//兄弟フレームが存在する場合
	if(pFrame->pFrameSibling!=NULL)
	{
		//兄弟フレームを検索
		if(FAILED(this->SetupBoneFromFrame(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	//子フレームが存在する場合
	if(pFrame->pFrameFirstChild!=NULL)
	{
		//子フレームを検索
		if(FAILED(this->SetupBoneFromFrame(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//メッシュコンテナ内のボーンを設定
HRESULT MeshManager::SetupBoneFromMeshContainer(LPD3DXFRAME pFrameRoot,LPD3DXMESHCONTAINER pMeshCont)
{
	DxMeshContainer *pMeshContainer = (DxMeshContainer*)pMeshCont;

	//スキンメッシュのときはボーン行列をセット
	if(pMeshContainer->pSkinInfo)
	{
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[NumBones];
		if( !pMeshContainer->ppBoneMatrixPtrs ) return false;

		LPCSTR str;

		for( DWORD i = 0; i < NumBones; i++ )
		{
			str = pMeshContainer->pSkinInfo->GetBoneName(i);
			DxFrame *pFrame = (DxFrame*)D3DXFrameFind(pFrameRoot, str );
			if( !pFrame) return false;
			pMeshContainer->ppBoneMatrixPtrs[i] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}


//**************************************************************************
//破棄及び開放用関数
//**************************************************************************
//DxMeshに含まれるデータの破棄および開放
void MeshManager::ReleaseMesh(DxBaseMesh *base_mesh)
{
	DxMesh *mesh = (DxMesh*)base_mesh;

	//テクスチャ情報の領域を破棄
	SAFE_DELETE_ARRAY(mesh->pTex);
	//マテリアル情報の領域を破棄
	SAFE_DELETE_ARRAY(mesh->pMat);
	//ファイル名の破棄
//	SAFE_DELETE_ARRAY(mesh->file_name);
	//メッシュデータの開放
	SAFE_RELEASE(mesh->lpMesh);

	//DxMesh領域の破棄
	SAFE_DELETE(mesh);
}
//DxAnimeMeshに含まれるデータの破棄および開放
void MeshManager::ReleaseAnimeMesh(DxBaseMesh *base_mesh)
{
	DxAnimeMesh *ani_mesh = (DxAnimeMesh*)base_mesh;

	//ファイル名の破棄
//	SAFE_DELETE_ARRAY(ani_mesh->file_name);

	//アニメーションコントローラの開放
	SAFE_RELEASE(ani_mesh->pAnimController);

	//フレーム階層の最上位ノードに含まれる全ノードの破棄
	D3DXFrameDestroy(ani_mesh->pFrameRoot,this->hierarchy);
	//フレーム階層の最上位ノードにNULLを設定
	ani_mesh->pFrameRoot = NULL;

	//DxAnimeMesh領域の破棄
	SAFE_DELETE(ani_mesh);
}

//**************************************************************************
//その他の関数
//**************************************************************************
//登録済みメッシュデータのファイル名をチェックし重複ロードを防止する
DxBaseMesh* MeshManager::CheckFileName(const string& fName)
{
	string str1="";
	string str2="";

	str1+=fName;

	for(short i=0;i<this->mesh_val;i++)
	{
		str2+=this->mesh_data[i]->file_name;
		if((str1 == str2))
		{
			return this->mesh_data[i];
		}
	}

	return NULL;
}
