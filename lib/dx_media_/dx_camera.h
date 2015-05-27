
//**************************************************************************
//カメラクラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#pragma once

#include "dx.h"

//カメラクラス
class Dx_Camera
{
	public:
		//射影タイプ
		enum MatrixType
		{
			//正射影行列
			DX_ORTHO_MAT,
			//遠近射影行列
			DX_PERS_MAT
		};
		/*共通変数*/
		enum {VERTICAL_ROT=1,CROSS_ROT=2};

	private:
		//射影タイプ
		MatrixType mat_type;

		//ビュー行列
		D3DXMATRIX view_mat;
		//射影行列
		D3DXMATRIX projection_mat;

		//初期座標
		D3DXVECTOR3 init_pos;
		//視点座標
		D3DXVECTOR3 eye_pos;
		//注視点座標
		D3DXVECTOR3 look_pos;

		//行列関連の設定
		void SetupMatrix();

		/*回転関連*/
		//回転用縦軸
		D3DXVECTOR3 vertical_axis;
		//回転用横軸
		D3DXVECTOR3 cross_axis;
		//回転状態
		short rot_state;
		/*縦軸回転用変数*/
		//縦軸制御変数
		float now_vertical_rot;
		//縦軸回転終了角度
		float vertical_end;
		//1フレームあたりの増加量
		float vertical_add;
		/*横軸回転用変数*/
		//横軸制御変数
		float now_cross_rot;
		//横軸回転終了角度
		float cross_end;
		//1フレームあたりの増加量
		float cross_add;

		float dot_ver_cross;

		//回転関数
		void Rotation();
		//回転関連の初期化
		void InitRotation();

		//クォータニオンによるカメラの回転角度
		D3DXQUATERNION m_Rotation;
	public:
		//コンストラクタ
		Dx_Camera(float lx=0.0f,float ly=0.0f,float lz=0.0f);

		//初期化
		void Init();
		//制御関数
		void Step();

		/*回転関連*/
		//現在の回転状態を取得
		short GetRotState();

		//縦軸回転開始		（加算角度,何フレームで回転するか）
		void StartVerticalRot(float add_deg,short frame_val=1);
		//横軸回転開始		（加算角度,何フレームで回転するか）
		void StartCrossRot(float add_deg,short frame_val=1);
		//縦軸の角度指定
		void SetVerticalRot(float deg);
		//横軸の角度指定
		void SetCrossRot(float deg);

		//初期座標の設置		（float型）
		void SetInitPos(float ix,float iy,float iz);
		//初期座標の設置		（D3DXVECTOR3型）
		void SetInitPos(D3DXVECTOR3 init);

		//視点座標の設置
		void SetEyePos(D3DXVECTOR3 eye_pos);
		//視点座標の取得
		D3DXVECTOR3 GetEyePos();

		//注視点座標の設置		（float型）
		void SetLookPos(float lx,float ly,float lz);
		//注視点座標の設置		（D3DXVECTOR3型）
		void SetLookPos(D3DXVECTOR3 look);
		//注視点座標の取得
		D3DXVECTOR3 GetLookPos();

		//回転用横軸の設定
		void CheckCrossAxis();

		//現在のVertical角度の取得
		float GetNowVertical();
		//現在のCross角度の取得
		float GetNowCross();
		//現在の角度情報の取得
		D3DXVECTOR2 GetNowDeg();

		//ビュー行列の取得
		D3DXMATRIX GetViewMat();
		//射影行列の取得
		D3DXMATRIX GetProjectionMat();

		//射影タイプの設定
		void	SetMatrixType(MatrixType type);

		//クォータニオンを利用した回転
		void RotationQuaternion(const float inX, const float inY, const float inZ);

		//視点を中心に回転
		void LookAtRotation(const D3DXVECTOR3& inAt, const D3DXVECTOR2& inRotation);

};
