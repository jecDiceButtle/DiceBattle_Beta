
//**************************************************************************
//アニメーションメッシュ関連
// DxHierarcy		: フレーム及びメッシュコンテナ製作用クラス
// DxFrame			: 派生フレーム構造体
// DxMeshContainer	: 派生メッシュコンテナ構造体
// DxAnimeMesh		: アニメーションメッシュ構造体
//**************************************************************************

#pragma once

#include "dx.h"

#include "dx_base_mesh.h"
#include "texture_manager.h"

#include "dx_anime_cont_auxiliary.h"

#include <string>
using namespace std;

//派生フレーム構造体
typedef struct DxFrame: public D3DXFRAME
{
	//メッシュ内の最終ワールド行列
    D3DXMATRIX					CombinedTransformationMatrix;
}DxFrame;

//派生メッシュコンテナ構造体
typedef struct DxMeshContainer :public D3DXMESHCONTAINER
{
	//テクスチャ郡
	LPDIRECT3DTEXTURE9*			ppTextures;

	//メッシュのポインタ
	LPD3DXMESH					lpMesh;
	//所属頂点情報
	LPD3DXATTRIBUTERANGE		pAttributeTable;
	//所属頂点数
	DWORD						NumAttributeGroups;

	LPD3DXBUFFER				pBoneCombinationBuf;
	D3DXMATRIX**				ppBoneMatrixPtrs;
	//初期位置の行列
	D3DXMATRIX*					pBoneOffsetMatrices;
	//ボーンの数
	UINT						NumBoneMatricesMax;
	//ボーン行列
	D3DXMATRIXA16*				pBoneMatrices;
}DxMeshContainer;

//フレームやメッシュコンテナの割り当てまたは開放を行うクラス
//派生クラスにすることで独自のフレームやメッシュコンテナの割り当てなどができるようになる
class Dx_Hierarchy : public ID3DXAllocateHierarchy
{
	private:
		//テクスチャ管理クラス
		TextureManager *t_manager;

	public:
		//コンストラクタ
		Dx_Hierarchy(TextureManager *t_manager);
		//独自フレームの生成
		STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
		//独自メッシュコンテナの生成
		STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
			CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
		//ボーンの生成
		HRESULT CreateBone(DxMeshContainer *pMeshContainer,LPDIRECT3DDEVICE9 lpD3DDev);

		//独自フレームの破棄および開放
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
		//独自メッシュコンテナの破棄および開放
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );
};

//アニメーションメッシュ構造体
typedef struct DxAnimeMesh : public DxBaseMesh
{
	//フレーム階層の最上位ノード
	LPD3DXFRAME					pFrameRoot;
	//アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER	pAnimController;

	//コンストラクタ
	DxAnimeMesh();
	//ファイル名の取得
	string GetFileName();
}DxAnimeMesh;