
//**************************************************************************
//アニメーションメッシュ関連
// DxHierarcy		: フレーム及びメッシュコンテナ製作用クラス
// DxFrame			: 派生フレーム構造体
// DxMeshContainer	: 派生メッシュコンテナ構造体
// DxAnimeMesh		: アニメーションメッシュ構造体
//**************************************************************************


#include "dx_anime_mesh.h"


//**************************************************************************
//フレーム及びメッシュコンテナ製作用クラス
//**************************************************************************
//コンストラクタ
Dx_Hierarchy::Dx_Hierarchy(TextureManager *t_manager)
{
	//テクスチャ管理クラスのアドレスを設定
	this->t_manager = t_manager;
}

//フレームの生成
HRESULT Dx_Hierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	//生成用オブジェクトの宣言
	DxFrame *pFrame;
	//生成したオブジェクトの登録先を初期化
    *ppNewFrame = NULL;

	//オブジェクトの生成
	pFrame = new DxFrame;
	//変数がNULLだった場合
    if ( pFrame == NULL)
	{
		//エラーの意味 : Direct3D が呼び出しを完了するための十分なメモリを割り当てることができませんでした。
		return E_OUTOFMEMORY;
	}
 
	//フレーム名が存在しない場合
	if(Name==NULL)
	{
		//空白を設定
		Name = "";
	}
	//フレーム名の領域確保
	pFrame->Name=new TCHAR[lstrlen(Name) + 1];
	//領域が確保されなかった場合
    if (!pFrame->Name)
	{
		//エラーの意味 : Direct3D サブシステム内で原因不明のエラーが発生しました。
		return E_FAIL;
	}
	//名前をコピーする
	strcpy(pFrame->Name,Name);

	//変換行列に単位行列を設定
	D3DXMatrixIdentity( &pFrame->TransformationMatrix);
	//メッシュ内の最終ワールド行列に単位行列を設定
	//**注意** ここで設定しているのは派生クラスにしかないのでD3DXFRAMEを使用した場合にはエラーになる！！
    D3DXMatrixIdentity( &pFrame->CombinedTransformationMatrix);

	//メッシュコンテナへのポインタを初期化
    pFrame->pMeshContainer = NULL;
	//兄弟フレームへのポインタを初期化
    pFrame->pFrameSibling = NULL;
	//子フレームへのポインタを初期化
    pFrame->pFrameFirstChild = NULL;

	//生成したオブジェクトのアドレスを繋ぎ変える
    *ppNewFrame = pFrame;

	//処理が無事に終了
    return S_OK;
}
//メッシュコンテナの生成
HRESULT Dx_Hierarchy::CreateMeshContainer(
	LPCTSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials, 
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER *ppMeshContainer )
{
	HRESULT hr = S_OK;
	//生成用オブジェクトの宣言
	DxMeshContainer *pMeshContainer = NULL;
	//生成したオブジェクトの登録先を初期化
	*ppMeshContainer = NULL;

	int iFacesAmount;
	unsigned iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;

	// patch meshes を扱う事はできない
	//if( pMeshData->Type != D3DXMESHTYPE_MESH ) return E_FAIL;

	// FVF で記述されたメッシュ以外は読めない
	//if( pMesh->GetFVF() == 0 ) return E_FAIL;

	//DWORD					dwBoneAmt=0;

	//メッシュデータの持つメッシュポインタを取得
	pMesh = pMeshData->pMesh;

	//オブジェクトの生成
	pMeshContainer = new DxMeshContainer;
	//変数がNULLだった場合
    if (pMeshContainer == NULL)
	{
		//エラーの意味 : Direct3D が呼び出しを完了するための十分なメモリを割り当てることができませんでした。
		return E_OUTOFMEMORY;
	}
	//
	ZeroMemory(pMeshContainer, sizeof(DxMeshContainer));

	//メッシュコンテナ名の領域確保
	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];
	//領域が確保されなかった場合
    if (!pMeshContainer->Name)
	{
		//エラーの意味 : Direct3D サブシステム内で原因不明のエラーが発生しました。
		return E_FAIL;
	}
	//名前をコピーする
	strcpy(pMeshContainer->Name,Name);

	//メッシュに関連付けられているデバイスを取得
	if(FAILED(pMesh->GetDevice(&pDevice)))
	{
		/*hr = E_FAIL;
		SAFE_RELEASE( pDevice );
		if( pMeshContainer ) DestroyMeshContainer( pMeshContainer );
		return hr;*/
	}

	//メッシュに含まれる面の数を取得
	iFacesAmount = pMesh->GetNumFaces();

    // 当該メッシュが法線を持たない場合は法線を追加する
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
		//メッシュデータの種類を「メッシュ」に変更
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		//FVFコードを使ってメッシュのコピーを生成
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pDevice, &pMeshContainer->MeshData.pMesh );
		//処理が失敗した場合
        if (FAILED(hr))
		{
			//エラーの意味 : Direct3D サブシステム内で原因不明のエラーが発生しました。
			return E_FAIL;
		}

        pMesh = pMeshContainer->MeshData.pMesh;
		//pMeshに含まれる各頂点の法線を計算して出力
        D3DXComputeNormals( pMesh, NULL );

    } else{
		//メッシュコンテナ内のメッシュデータにメッシュポインタを設定
        pMeshContainer->MeshData.pMesh = pMesh;
		//メッシュデータの種類を「メッシュ」に設定
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		//???
        pMesh->AddRef();
    }

	//メッシュのマテリアル設定
	//マテリアル数を設定
    pMeshContainer->NumMaterials	= max(1, NumMaterials);
	//マテリアルの数だけマテリアルの領域を確保
    pMeshContainer->pMaterials		= new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//マテリアルの数だけテクスチャの領域確保
    pMeshContainer->ppTextures		= new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	//面の数だけ領域を確保
    pMeshContainer->pAdjacency		= new DWORD[iFacesAmount*3];

	//それぞれの領域確保に失敗した場合
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		//エラーの意味 : Direct3D が呼び出しを完了するための十分なメモリを割り当てることができませんでした。
		return E_OUTOFMEMORY;
	}

	//配列に0を設定
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	//配列にpAbjacencyを設定
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);

	//マテリアルが存在する時
    if (NumMaterials > 0)
	{
		//
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		//
        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			//テクスチャのファイル名が存在する場合
            if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//ファイルまでの相対パスを設定
				char path[256] ="res/x/";
				//ファイル名を取得
				LPSTR fName	= pMeshContainer->pMaterials[iMaterial].pTextureFilename;

				//テクスチャ情報の生成
				pMeshContainer->ppTextures[iMaterial] = 
					this->t_manager->LoadTextureFromFile(fName,path);
				//テクスチャ名の破棄
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
	else
	{
		//テクスチャ名の破棄
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		//マテリアルに0を設定
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));

		//
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular  = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

	//スキニング情報が存在する場合、ボーンを生成
	if(pSkinInfo!=NULL)
	{
		//メッシュコンテナ内のスキニング情報を設定
		pMeshContainer->pSkinInfo = pSkinInfo;
		//???
		pSkinInfo->AddRef();

		//スキニング情報からボーンを生成
		this->CreateBone(pMeshContainer,pDevice);
	}

	//ローカルに生成したメッシュコンテナを呼び出し側にコピーする
	*ppMeshContainer = pMeshContainer;

	//処理が無事に終了
    return hr;
}
//ボーンの生成
HRESULT Dx_Hierarchy::CreateBone(DxMeshContainer *pMeshContainer,LPDIRECT3DDEVICE9 lpD3DDev)
{
	DWORD dwBoneAmt=0;
	LPD3DXSKININFO pSkinInfo = pMeshContainer->pSkinInfo;

	pMeshContainer->lpMesh = pMeshContainer->MeshData.pMesh;
	pMeshContainer->MeshData.pMesh->AddRef();

	// Bone Offset Matrices 保存用のメモリを確保
	dwBoneAmt = pSkinInfo->GetNumBones();
	pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
	if( !pMeshContainer->pBoneOffsetMatrices ) return E_OUTOFMEMORY;

	// Bone Offset Matrices 読み込み
	for (DWORD i= 0; i < dwBoneAmt; i++){
		memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));	
	}

	HRESULT hr = S_OK;

	//スキン情報がないなら終了
	if( !pMeshContainer->pSkinInfo ) return hr;

	//初期化しておく
	if(pMeshContainer->MeshData.pMesh)			SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	if(pMeshContainer->pBoneCombinationBuf)		SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	hr = pMeshContainer->lpMesh->CloneMeshFVF(
			D3DXMESH_MANAGED, pMeshContainer->lpMesh->GetFVF(),
			lpD3DDev , &pMeshContainer->MeshData.pMesh );
	if( FAILED( hr ) ) return hr;

	//メッシュコンテナに所属している頂点数の取得
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->NumAttributeGroups );
	if( FAILED( hr ) ) return hr;

	//メッシュコンテナに所属している頂点情報の格納
	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
	if( !pMeshContainer->pAttributeTable ){
		hr = E_OUTOFMEMORY;
		return hr;
	}
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL );
	if( FAILED( hr ) ) return hr;

	// 他のメッシュによってボーン行列用のメモリが確保しきれていない場合は確保
	if( pMeshContainer->NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() ){
		pMeshContainer->NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		delete []pMeshContainer->pBoneMatrices;
		pMeshContainer->pBoneMatrices = new D3DXMATRIXA16[pMeshContainer->NumBoneMatricesMax];
		if( !pMeshContainer->pBoneMatrices ){
			hr = E_OUTOFMEMORY;
			return hr;
		}
	}

   return hr;
}

//フレームの破棄
HRESULT Dx_Hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	DxFrame *frame = (DxFrame*)pFrameToFree;

	//ファイル名の領域を破棄
	SAFE_DELETE_ARRAY(frame->Name);
	//フレームを破棄
	SAFE_DELETE(frame);

	//処理が無事に終了
	return S_OK;
}
//メッシュコンテナの破棄
HRESULT Dx_Hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	DxMeshContainer *pMeshContainer = (DxMeshContainer*)pMeshContainerToFree;

	
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs);
	SAFE_DELETE(pMeshContainer->pAttributeTable);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneMatrices);

	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	//Adjacencyの領域破棄
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	//マテリアル郡の領域破棄
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	//テクスチャ郡の領域破棄
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	//
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	//スキニング情報の開放
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	//テクスチャ名の領域破棄
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	//メッシュコンテナの破棄
	SAFE_DELETE(pMeshContainer);

	//処理が無事に終了
	return S_OK;
}

//**************************************************************************
//アニメーションメッシュ構造体
//**************************************************************************
//コンストラクタ
DxAnimeMesh::DxAnimeMesh() : DxBaseMesh()
{
	//フレーム階層の最上位ノードの初期化
	this->pFrameRoot=NULL;
	//アニメーションコントローラの初期化
	this->pAnimController=NULL;
}
//ファイル名の取得
string DxAnimeMesh::GetFileName()
{
	return this->file_name;
}
