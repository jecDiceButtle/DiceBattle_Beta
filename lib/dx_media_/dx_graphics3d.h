
//**************************************************************************
//DirectX３Ｄ描画管理クラス
//基本型　：　Dx_Graphics2D
//派生型　：　なし
//**************************************************************************

#pragma once

//-------------------------------------
//３Dlib setup
#include "dx_camera.h"
#include "media_manager.h"
#include "dx_font.h"
//
typedef D3DXVECTOR2 DxVec2;
typedef D3DXVECTOR3 DxVec3;

//３Ｄ関連の描画管理クラス
class Dx_Graphics3D
{
	private:
		LPDIRECT3DDEVICE9 device;
		/*カラー情報関連*/
		//カラー情報の動的設定開始
		void BeginColorEnable(D3DCOLOR color);
		//カラー情報の動的設定終了
		void EndColorEnable();
	
		/*描画設定項目*/
		//レンダーステートパラメータの設定
		void SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);

		/*ワールド変換行列設定関連*/
		//座標、倍率、角度からワールド変換行列を設定
		//tuika
		D3DXMATRIX SetTransformToDevice(DxVec3 *pos, DxVec3 *scale, DxVec3 *angle, D3DXMATRIX *matrot);

		//メッシュサブセットを描画
		void DrawSubset(LPD3DXMESH lpMesh,DWORD AttribId,D3DMATERIAL9 *pMat,LPDIRECT3DTEXTURE9 pTex);

		/*メッシュ関連*/
		//メッシュ描画
		void DrawMesh(DxMesh *mesh);

		/*アニメーションメッシュ関連*/
		//アニメーションメッシュ描画
		void DrawAnimeMesh(DxAnimeMesh *mesh, D3DXMATRIX mat);
		//フレーム描画
		void DrawFrame(LPD3DXFRAME pFrame);
		//メッシュ内の最終ワールド行列の更新
		void UpdateFrame(LPD3DXFRAME pFrame,LPD3DXMATRIX mat);
		//メッシュコンテナ描画
		void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame);

	public:
		//コンストラクタ
		Dx_Graphics3D(LPDIRECT3DDEVICE9 dev);
		//デストラクタ
		~Dx_Graphics3D();

		/*３Ｄ描画関連*/
		//３Ｄセットアップ　　（カメラ使用型）
		void Setup3DEnv(Dx_Camera *camera);
		void Dx_Graphics3D::Setup3DEnv(std::shared_ptr<Dx_Camera> camera);

		//３Ｄセットアップ　　（行列直接指定型）
		void Setup3DEnv(D3DXMATRIX view,D3DXMATRIX projection);

		//３Ｄモデルの描画
		//tuika
		void DrawModel(DxBaseMesh *mesh, DxVec3 *pos, DxVec3 *scale = NULL, DxVec3 *angle = NULL, D3DCOLOR *color = NULL, D3DXMATRIX *matrot = NULL);
};


