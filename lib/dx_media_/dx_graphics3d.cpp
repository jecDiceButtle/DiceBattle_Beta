
//**************************************************************************
//DirectX３Ｄ描画管理クラス
//基本型　：　Dx_Graphics2D
//派生型　：　なし
//**************************************************************************

#include "dx_graphics3d.h"


//コンストラクタ
Dx_Graphics3D::Dx_Graphics3D(LPDIRECT3DDEVICE9 device)
{
	this->device = device;
}
//デストラクタ
Dx_Graphics3D::~Dx_Graphics3D()
{
}

//**************************************************************************
//変換行列セットアップ関連
//**************************************************************************
//３Ｄセットアップ　　（カメラ使用型）
void Dx_Graphics3D::Setup3DEnv(Dx_Camera *camera)
{
	//ZBAFA
	this->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	this->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	//CULLMODE
	this->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	//LIGHT
	this->SetRenderState(D3DRS_LIGHTING,TRUE);

	this->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	//カメラの制御
	camera->Step();

	//ビュー行列の設定
	this->device->SetTransform(D3DTS_VIEW,&camera->GetViewMat());
	//射影行列の設定
	this->device->SetTransform(D3DTS_PROJECTION,&camera->GetProjectionMat());



}
//３Ｄセットアップ　　（カメラ使用型）
void Dx_Graphics3D::Setup3DEnv(std::shared_ptr<Dx_Camera> camera)
{
	//ZBAFA
	this->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	this->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//CULLMODE
	this->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//LIGHT
	this->SetRenderState(D3DRS_LIGHTING, TRUE);

	this->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//カメラの制御
	camera->Step();

	//ビュー行列の設定
	this->device->SetTransform(D3DTS_VIEW, &camera->GetViewMat());
	//射影行列の設定
	this->device->SetTransform(D3DTS_PROJECTION, &camera->GetProjectionMat());



}

//３Ｄセットアップ　　（行列直接指定型）
void Dx_Graphics3D::Setup3DEnv(D3DXMATRIX view,D3DXMATRIX projection)
{
	//ZBAFA
	this->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	this->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	//CULLMODE
	this->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//LIGHT
	this->SetRenderState(D3DRS_LIGHTING,TRUE);

	this->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	//ビュー行列の設定
	this->device->SetTransform(D3DTS_VIEW,&view);
	//射影行列の設定
	this->device->SetTransform(D3DTS_PROJECTION,&projection);
}

void Dx_Graphics3D::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	this->device->SetRenderState(State,Value);
}
//座標、倍率、角度からワールド変換行列を設定
D3DXMATRIX Dx_Graphics3D::SetTransformToDevice(DxVec3 *pos, DxVec3 *scale, DxVec3 *angle, D3DXMATRIX *matrot)
{
	D3DXMATRIX		matScale, matRot, matPos, mat;

	//メイン行列を単位行列とする
	D3DXMatrixIdentity(&mat);

	//倍率関連
	if (scale != NULL)
	{
		//スケーリング行列を設定
		D3DXMatrixScaling(&matScale, scale->x, scale->y, scale->z);
		//メイン行列にスケーリング行列を合成
		D3DXMatrixMultiply(&mat, &mat, &matScale);
	}
	//角度関連
	//tuika
	if (matrot != NULL)
	{
		////Yaw-Y軸回転、Pitch-X軸回転、Roll-Z軸回転として回転行列を設定
		//D3DXMatrixRotationYawPitchRoll(&matRot,
		//	D3DXToRadian(angle->y), D3DXToRadian(angle->x), D3DXToRadian(angle->z));

		matRot = *matrot;

		//メイン行列に回転行列を合成
		D3DXMatrixMultiply(&mat, &mat, &matRot);
	}
	else if (angle != NULL)
	{
		//Yaw-Y軸回転、Pitch-X軸回転、Roll-Z軸回転として回転行列を設定
		D3DXMatrixRotationYawPitchRoll(&matRot,
			D3DXToRadian(angle->y), D3DXToRadian(angle->x), D3DXToRadian(angle->z));

		//メイン行列に回転行列を合成
		D3DXMatrixMultiply(&mat, &mat, &matRot);
	}

	//座標関連
	{
		//位置行列を設定
		D3DXMatrixTranslation(&matPos, pos->x, pos->y, pos->z);
		//メイン行列に位置行列を合成
		D3DXMatrixMultiply(&mat, &mat, &matPos);
	}

	//デバイスにワールド変換行列を設定
	this->device->SetTransform(D3DTS_WORLD, &mat);

	return mat;
}

//**************************************************************************
//カラー情報設定関連
//**************************************************************************
//カラー情報の動的設定開始
void Dx_Graphics3D::BeginColorEnable(D3DCOLOR color)
{
	this->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	this->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//D3DRS_TEXTUREFACTORのレンダーステート値を設定
	this->SetRenderState( D3DRS_TEXTUREFACTOR, color );
	//ARG1とARG2のカラー値を乗算してカラー値を取得
	this->device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	//テクスチャのカラー値
	this->device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//動的のカラー値
	this->device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR);

	//ARG1とARG2のα値を乗算してα値を取得
	this->device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	//テクスチャのα値
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	//動的のα値
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR);
}
//カラー情報の動的設定終了
void Dx_Graphics3D::EndColorEnable()
{
	//ステージ0のD3DTSS_COLOROPをデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	//ステージ0のD3DTSS_COLORARG1をデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//ステージ0のD3DTSS_COLORARG2をデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT);

	//ステージ0のD3DTSS_ALPHAOPをデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	//ステージ0のD3DTSS_ALPHAARG1をデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	//ステージ0のD3DTSS_ALPHAARG2をデフォルト値に設定
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
}

//**************************************************************************
//３Ｄモデル描画関連
//**************************************************************************
//３Ｄモデル描画関数
void Dx_Graphics3D::DrawModel(DxBaseMesh *mesh, DxVec3 *pos, DxVec3 *scale, DxVec3 *angle, D3DCOLOR *color, D3DXMATRIX *matrot)
{
	//指定されたメッシュデータがNULLでない場合のみ描画
	if (mesh != NULL)
	{
		D3DXMATRIX mat;

		//変換行列の設定
		//tuika
		//ここでワールド変換行列を変える
		if (matrot != NULL)
		{
			mat = this->SetTransformToDevice(pos, scale, angle, matrot);
		}
		else if (angle != NULL)
		{
			mat = this->SetTransformToDevice(pos, scale, angle, matrot);
		}


		//カラー情報が設定されている場合
		if (color != NULL)
		{
			this->BeginColorEnable(*color);
		}

		//meshが派生クラスDxMeshの場合
		if (dynamic_cast<DxMesh*>(mesh) != 0)
		{
			//DxMeshの描画
			this->DrawMesh((DxMesh*)mesh);
		}
		//meshが派生クラスDxAnimeMeshの場合
		else if (dynamic_cast<DxAnimeMesh*>(mesh) != 0)
		{
			//DxAnimeMeshの描画
			this->DrawAnimeMesh((DxAnimeMesh*)mesh, mat);
		}

		//カラー情報が設定されている場合
		if (color != NULL)
		{
			this->EndColorEnable();
		}
	}
}

//**************************************************************************
//３Ｄ描画にとって核となる関数関連
//**************************************************************************
//メッシュサブセットを描画
void Dx_Graphics3D::DrawSubset(LPD3DXMESH lpMesh,DWORD AttribId,D3DMATERIAL9 *pMat,LPDIRECT3DTEXTURE9 pTex)
{
	//デバイスにマテリアルプロパティを設定
	this->device->SetMaterial(pMat);
	//デバイスのステージにテクスチャを割り当てる
	this->device->SetTexture(0,pTex);
//	this->device->SetFVF();
	//メッシュサブセットの属性IDを指定して描画
	lpMesh->DrawSubset(AttribId);
}

//**************************************************************************
//メッシュ描画関連
//**************************************************************************
//メッシュ描画
void Dx_Graphics3D::DrawMesh(DxMesh *mesh)
{
	//メッシュに含まれているマテリアルの数だけ処理を行う
	for(DWORD cnt=0;cnt<mesh->matCount;cnt++)
	{
		//メッシュサブセットを描画
		this->DrawSubset(mesh->lpMesh,cnt,&mesh->pMat[cnt],mesh->pTex[cnt]);
	}
}

//**************************************************************************
//アニメーションメッシュ描画関連
//**************************************************************************
//アニメーションメッシュ描画
void Dx_Graphics3D::DrawAnimeMesh(DxAnimeMesh *mesh, D3DXMATRIX mat)
{
	//全フレームの最終ワールド行列の更新
	this->UpdateFrame(mesh->pFrameRoot,&mat);
	//フレーム描画
	this->DrawFrame(mesh->pFrameRoot);
}
//フレーム描画
void Dx_Graphics3D::DrawFrame(LPD3DXFRAME pFrame)
{
	DxFrame *frame = (DxFrame*)pFrame;

	//メッシュコンテナが存在する場合
	if(frame->pMeshContainer!=NULL)
	{
		//メッシュコンテナを描画
		this->DrawMeshContainer(frame->pMeshContainer,frame);
	}

	//兄弟フレームが存在する場合
	if(frame->pFrameSibling!=NULL)
	{
		//兄弟フレームを描画
		this->DrawFrame(frame->pFrameSibling);
	}
	//子フレームが存在する場合
	if(frame->pFrameFirstChild!=NULL)
	{
		//子フレームを描画
		this->DrawFrame(frame->pFrameFirstChild);
	}
}
//メッシュ内の最終ワールド行列の更新
void Dx_Graphics3D::UpdateFrame(LPD3DXFRAME pFrame,LPD3DXMATRIX mat)
{
	//DxFrameにキャストする
	DxFrame *frame = (DxFrame*)pFrame;

	//基本行列データが存在する場合
	if(mat!=NULL)
	{
		//最終ワールド行列に変換行列と基本行列の合成を出力
		D3DXMatrixMultiply( &frame->CombinedTransformationMatrix, &frame->TransformationMatrix, mat );
	}
	else
	{ 
		//最終ワールド行列に自身の変換行列を出力
		frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	//兄弟フレームが存在する場合
	if(frame->pFrameSibling!=NULL)
	{
		//兄弟フレームの更新
		this->UpdateFrame(frame->pFrameSibling,mat);
	}
	//子フレームが存在する場合
	if(frame->pFrameFirstChild!=NULL)
	{
		//子フレームの更新
		this->UpdateFrame(frame->pFrameFirstChild,&frame->CombinedTransformationMatrix);
	}
}
//メッシュコンテナ描画
void Dx_Graphics3D::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame)
{
	DxMeshContainer *mesh_container = (DxMeshContainer*)pMeshContainer;
	DxFrame *frame = (DxFrame*)pFrame;

	//
	D3DMATERIAL9 mat;
	LPDIRECT3DTEXTURE9 pTex=NULL;
	//D3DXMESHDATA内のメッシュデータを抽出
	LPD3DXMESH lpMesh = mesh_container->MeshData.pMesh;

	//スキニング情報がない場合
	if(pMeshContainer->pSkinInfo==NULL)
	{
		//デバイスにフレームのワールド行列を設置
		this->device->SetTransform( D3DTS_WORLD, &frame->CombinedTransformationMatrix);

		//メッシュ描画
		for (DWORD i=0;i<mesh_container->NumMaterials;i++)
		{
			//マテリアル情報の取得
			mat = mesh_container->pMaterials[i].MatD3D;
			//テクスチャ情報の取得
			pTex = mesh_container->ppTextures[i];

			//メッシュサブセットを描画
			this->DrawSubset(lpMesh,i,&mat,pTex);
		}
	}
	//スキニング情報がある場合
	else
	{
		D3DXMATRIX matId;
		PBYTE			pVerticesSrc;
		PBYTE			pVerticesDest;

		//ボーン数を取得
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		for( DWORD i = 0; i < NumBones; i++ ){
			D3DXMatrixMultiply(
				&mesh_container->pBoneMatrices[i],
				&mesh_container->pBoneOffsetMatrices[i], 
				mesh_container->ppBoneMatrixPtrs[i]
			);
		}

		//ワールド行列をクリア
		D3DXMatrixIdentity(&matId);
		this->device->SetTransform(D3DTS_WORLD, &matId);

		//頂点バッファをロック
		mesh_container->lpMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);
		lpMesh->LockVertexBuffer( 0, (LPVOID*)&pVerticesDest);

		//スキンメッシュ作成
		mesh_container->pSkinInfo->UpdateSkinnedMesh( mesh_container->pBoneMatrices, NULL, pVerticesSrc, pVerticesDest);

		//頂点バッファのロックを解除
		mesh_container->lpMesh->UnlockVertexBuffer();
		lpMesh->UnlockVertexBuffer();

		//メッシュ描画
		for(UINT i = 0;i<mesh_container->NumAttributeGroups;i++)
		{
			//メッシュサブセットの属性IDを取得
			unsigned AttribId = mesh_container->pAttributeTable[i].AttribId;
			//マテリアル情報を取得
			mat = mesh_container->pMaterials[AttribId].MatD3D;
			//テクスチャ情報を取得
			pTex = mesh_container->ppTextures[AttribId];

			//メッシュのサブセットを描画
			this->DrawSubset(lpMesh,AttribId,&mat,pTex);
		}
	}
}
