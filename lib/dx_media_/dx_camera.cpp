
//**************************************************************************
//カメラクラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#include "dx_Camera.h"


//コンストラクタ
Dx_Camera::Dx_Camera(float lx,float ly,float lz)
{
	//射影タイプの設定
	this->mat_type = this->DX_PERS_MAT;

	//視点座標の設定
	this->SetLookPos(lx,ly,lz);
	//初期座標の設定
	this->SetInitPos(4.0f,0,0);
}

//初期化関数
void Dx_Camera::Init()
{
	this->SetEyePos(this->init_pos);

	this->InitRotation();

	this->SetupMatrix();
}
//制御関数
void Dx_Camera::Step()
{
	// 位置ベクトルの設定
	view_mat._41 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._11, view_mat._12, view_mat._13), &eye_pos);
	view_mat._42 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._21, view_mat._22, view_mat._23), &eye_pos);
	view_mat._43 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._31, view_mat._32, view_mat._33), &eye_pos);

	/*回転関連*/
	this->Rotation();

	/*カメラ本体設定*/
	this->SetupMatrix();


}

//
void Dx_Camera::SetupMatrix()
{
	//ビュー行列の設定
	D3DXMatrixLookAtLH(&this->view_mat,&this->eye_pos,&this->look_pos,&DxVec3(0.0f,1.0f,0.0f));

	//射影タイプによって設定する
	switch(this->mat_type)
	{
		//遠近射影行列を使用する場合
	case MatrixType::DX_PERS_MAT:
			//遠近射影行列の設定
		D3DXMatrixPerspectiveFovLH(&this->projection_mat, D3DXToRadian(33.0f), float(gplib::system::WINW ) / float(gplib::system::WINH ), 0.1f, 50000.0f);
//		D3DXMatrixPerspectiveFovLH(&this->projection_mat, D3DXToRadian(45.0f), 1920.0f / 1080.0f, 0.1f, 50000.0f);
		break;
		//正射影行列を使用する場合
	case MatrixType::DX_ORTHO_MAT:
			//正射影行列の設定
			D3DXMatrixOrthoLH(&this->projection_mat,8,8,0,20);
			break;
	}
}

//回転関数
void Dx_Camera::Rotation()
{
	//回転状態のときのみ制御
	if(this->rot_state!=0)
	{
		D3DXMATRIX mat;
		//カメラ座標と視点座標の差を取得
		DxVec3 sub = this->eye_pos - this->look_pos;

		//縦軸回転
		if(this->rot_state&VERTICAL_ROT)
		{
			//回転行列の設定
			D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->vertical_add));
			//差ベクトルを回転
			D3DXVec3TransformCoord(&sub,&sub,&mat);
			D3DXVec3TransformCoord(&this->cross_axis,&this->cross_axis,&mat);
			//現在の制御角度に増加量を加算
			this->now_vertical_rot += this->vertical_add;
			//現在の制御角度と終了角度の差がを比較増加量より少ない時回転終了
			if(abs(this->vertical_end-this->now_vertical_rot) < abs(this->vertical_add) || this->vertical_add==0.0f)
			{
				//ステータスからVERTICAL_ROTを削除
				this->rot_state ^= VERTICAL_ROT;
				//現在のYaw角度の調整
				if(this->now_vertical_rot >= 360)
				{
					this->now_vertical_rot -= 360;
				}
				if(this->now_vertical_rot < 0)
				{
					this->now_vertical_rot += 360;
				}
			}
		}

		//横軸回転
		if(this->rot_state&CROSS_ROT)
		{
			//回転行列の設定
			D3DXMatrixRotationAxis(&mat,&this->cross_axis,D3DXToRadian(this->cross_add));
			//差ベクトルを回転
			D3DXVec3TransformCoord(&sub,&sub,&mat);
			//現在の制御角度に増加量を加算
			this->now_cross_rot += this->cross_add;
			//現在の制御角度と終了角度の差がを比較増加量より少ない時回転終了
			if(abs(this->cross_end-this->now_cross_rot) < abs(this->cross_add) || this->cross_add==0.0f)
			{
				//ステータスからCROSS_ROTを削除
				this->rot_state ^= CROSS_ROT;
				//現在のYaw角度の調整
				if(this->now_cross_rot >= 180)
				{
					this->now_cross_rot -= 360;
				}
				if(this->now_cross_rot < -180)
				{
					this->now_cross_rot += 360;
				}
			}
		}

		D3DXVec3Add(&this->eye_pos,&this->look_pos,&sub);
	}
	this->dot_ver_cross = D3DXVec3Dot(&this->vertical_axis,&this->cross_axis);
}
void Dx_Camera::InitRotation()
{
	//縦軸設定
	this->vertical_axis = DxVec3(0,1,0);
	//横軸設定
	this->cross_axis = DxVec3(0,0,1);

	//回転状態
	this->rot_state = 0;
	this->now_vertical_rot = 0;
	this->now_cross_rot = 0;
}
//現在の回転状態を取得
short Dx_Camera::GetRotState()
{
	return this->rot_state;
}
//指定角度まで自動回転
void Dx_Camera::StartVerticalRot(float add_deg,short frame_val)
{
	if(!(this->rot_state&VERTICAL_ROT))
	{
		//終了角度を設定
		this->vertical_end = this->now_vertical_rot + add_deg;
		//増加量を設定
		this->vertical_add = add_deg / frame_val;

		D3DXMATRIX mat;
		//カメラ座標と視点座標の差を取得
		DxVec3 sub = this->eye_pos - this->look_pos;
		//回転行列の設定
		D3DXMatrixRotationY(&mat,D3DXToRadian(this->vertical_end));
		//差ベクトルを回転
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//回転状態にY軸回転スタートを設定
		this->rot_state |= VERTICAL_ROT;
	}
}
//指定角度まで自動回転
void Dx_Camera::StartCrossRot(float add_deg,short frame_val)
{
	if(!(this->rot_state&CROSS_ROT))
	{
		//終了角度を設定
		this->cross_end = this->now_cross_rot + add_deg;
		//増加量を設定
		this->cross_add = add_deg / frame_val;

		D3DXMATRIX mat;
		//カメラ座標と視点座標の差を取得
		DxVec3 sub = this->eye_pos - this->look_pos;
		//回転行列の設定
		D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->cross_end));
		//差ベクトルを回転
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//回転状態にY軸回転スタートを設定
		this->rot_state |= this->CROSS_ROT;
	}
}
//
void Dx_Camera::SetVerticalRot(float deg)
{
	if(!(this->rot_state&VERTICAL_ROT))
	{
		//終了角度を設定
		this->vertical_end = deg;
		//増加量を設定
		this->vertical_add = deg - this->now_vertical_rot;

		D3DXMATRIX mat;
		//カメラ座標と視点座標の差を取得
		DxVec3 sub = this->eye_pos - this->look_pos;
		//回転行列の設定
		D3DXMatrixRotationY(&mat,D3DXToRadian(this->vertical_end));
		//差ベクトルを回転
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//回転状態にY軸回転スタートを設定
		this->rot_state |= VERTICAL_ROT;
	}
}
//
void Dx_Camera::SetCrossRot(float deg)
{
	if(!(this->rot_state&CROSS_ROT))
	{
		//終了角度を設定
		this->cross_end = deg;
		//増加量を設定
		this->cross_add = deg - this->now_cross_rot;

		D3DXMATRIX mat;
		//カメラ座標と視点座標の差を取得
		DxVec3 sub = this->eye_pos - this->look_pos;
		//回転行列の設定
		D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->cross_end));
		//差ベクトルを回転
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//回転状態にY軸回転スタートを設定
		this->rot_state |= this->CROSS_ROT;
	}
}

//初期座標の設置		（float型）
void Dx_Camera::SetInitPos(float ix, float iy, float iz)
{
	this->init_pos = this->look_pos + DxVec3(ix,iy,iz);
	this->Init();
	this->CheckCrossAxis();
}
//初期座標の設置		（DxVec3型）
void Dx_Camera::SetInitPos(DxVec3 init)
{
	this->init_pos = this->look_pos + init;
	this->Init();
	this->CheckCrossAxis();
}
//カメラ座標の設置
void Dx_Camera::SetEyePos(DxVec3 eye_pos)
{
	this->eye_pos = eye_pos;
}
//カメラ座標の取得
DxVec3 Dx_Camera::GetEyePos()
{
	return this->eye_pos;
}
//視点座標の設置		（float型）
void Dx_Camera::SetLookPos(float lx, float ly, float lz)
{
	this->look_pos = DxVec3(lx,ly,lz);
}
//視点座標の設置		（DxVec3型）
void Dx_Camera::SetLookPos(DxVec3 look)
{
	this->look_pos = look;
}
//視点座標の取得
DxVec3 Dx_Camera::GetLookPos()
{
	return this->look_pos;
}

//回転用横軸の設定
void Dx_Camera::CheckCrossAxis()
{
	//
	DxVec3 vec1,vec2,cross;
	//座標から視点座標までのベクトル
	vec1 = this->eye_pos-this->look_pos;
	//
	vec2 = this->vertical_axis;

	D3DXVec3Cross(&cross,&vec1,&vec2);
	D3DXVec3Normalize(&this->cross_axis,&cross);
}

//現在のVertical角度の取得
float Dx_Camera::GetNowVertical()
{
	return this->now_vertical_rot;
}
//現在のCross角度の取得
float Dx_Camera::GetNowCross()
{
	return this->now_cross_rot;
}
//現在の角度情報の取得
DxVec2 Dx_Camera::GetNowDeg()
{
	return DxVec2(this->now_vertical_rot,this->now_cross_rot);
}

//ビュー行列の取得
D3DXMATRIX Dx_Camera::GetViewMat()
{
	return this->view_mat;
}
//射影行列の取得
D3DXMATRIX Dx_Camera::GetProjectionMat()
{
	return this->projection_mat;
}

//射影タイプの設定
void	Dx_Camera::SetMatrixType(MatrixType type)
{
	this->mat_type = type;
}

void Dx_Camera::RotationQuaternion(const float inX, const float inY, const float inZ)
{
	// 角軸のクォータニオンを求める
	D3DXQUATERNION rot_x, rot_y, rot_z;
	::D3DXQuaternionRotationAxis(&rot_x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXToRadian(inX));
	::D3DXQuaternionRotationAxis(&rot_y, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXToRadian(-inY));
	::D3DXQuaternionRotationAxis(&rot_z, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXToRadian(-inZ));
	// 回転
	m_Rotation *= rot_x * rot_y * rot_z;
	// ビュー行列の軸の設定
	::D3DXMatrixRotationQuaternion(&view_mat, &m_Rotation);
}

void Dx_Camera::LookAtRotation(const D3DXVECTOR3& inAt, const D3DXVECTOR2& inRotation)
{
	// 注視点を原点としたときの視点の座標を求め、ビュー行列をワールド行列にする
	view_mat._41 = eye_pos.x - inAt.x;
	view_mat._42 = eye_pos.y - inAt.y;
	view_mat._43 = eye_pos.z - inAt.z;
	// 角軸の回転行列を作成する
	D3DXMATRIX rot_x, rot_y;
	::D3DXMatrixRotationX(&rot_x, D3DXToRadian(inRotation.x));
	::D3DXMatrixRotationY(&rot_y, D3DXToRadian(inRotation.y));
	// カメラを回転させる
	view_mat *= rot_y * rot_x;
	// 視点を戻す
	eye_pos = D3DXVECTOR3(view_mat._41, view_mat._42, view_mat._43) + inAt;
	// クォータニオンの保存
	::D3DXQuaternionRotationMatrix(&m_Rotation, &view_mat);
}
//
//void Dx_Camera::Step()
//{
//	// 位置ベクトルの設定
//	view_mat._41 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._11, view_mat._12, view_mat._13), &eye_pos);
//	view_mat._42 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._21, view_mat._22, view_mat._23), &eye_pos);
//	view_mat._43 = -::D3DXVec3Dot(&D3DXVECTOR3(view_mat._31, view_mat._32, view_mat._33), &eye_pos);
//}


