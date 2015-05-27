#include "camera.h"
#include "../../lib/gplib.h"

//3Dカメラ
extern LPDIRECT3DDEVICE9 pD3DDevice;
//メッシュ全体管理クラス
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;

namespace game
{
	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************/	

	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//

	Camera::Camera(const std::string& objectName)
		:
		Object(objectName),
		kakudo(0)
	{

		v = DxVec3(75.f, 60.f, -85.f);	//カメラの初期位置
		v2 = DxVec3(0.f, 0.f, 0.f);		//カメラの速度初期化
		lookpos = DxVec3(0.f,20.f,0.f);

		Cflag = FREE;
	}

	void Camera::input(){
		/*if (gplib::input::CheckPush(gplib::input::KEY_UP)) Cflag = UP;
		if (gplib::input::CheckPush(gplib::input::KEY_DOWN)) Cflag = DOWN;
		if (gplib::input::CheckPush(gplib::input::KEY_RIGHT)) Cflag = RIGHT;
		if (gplib::input::CheckPush(gplib::input::KEY_LEFT)) Cflag = LEFT;

		if (gplib::input::CheckPull(gplib::input::KEY_UP)) Cflag = FREE;
		if (gplib::input::CheckPull(gplib::input::KEY_DOWN)) Cflag = FREE;
		if (gplib::input::CheckPull(gplib::input::KEY_RIGHT)) Cflag = FREE;
		if (gplib::input::CheckPull(gplib::input::KEY_LEFT)) Cflag = FREE;


		if (Cflag == UP){
			v2 = DxVec3{ 0.f, 2.f, 0.f };
		}
		if (Cflag == DOWN){
			v2 = DxVec3{ 0.f, -2.f, 0.f };
		}
		if (Cflag == RIGHT){
			v2 = DxVec3{ 2.f, 0.f, 0.f };
		}
		if (Cflag == LEFT){
			v2 = DxVec3{ -2.f, 0.f, 0.f };
		}
		if (Cflag == FREE){
			v2 = DxVec3{ 0.f, 0.f, 0.f };
		}*/
	}

	void Camera::kaiten(){
		kakudo += D3DXToRadian(1.f);


		v2 = DxVec3((float)cos(kakudo) * 2.f, 0.f,(float)sin(kakudo) * 3.f);
		if (sin(kakudo) < 0)
		{
			bool flag = false;
		}
	}

	void Camera::camera(DxVec3 velocity){
		/*v += velocity;*/


//#ifdef _DEBUG
//
//		std::string str = std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
//
//		gplib::graph::Draw_2DClear();
//		gplib::font::Draw_FontText(0, 0, 0.f, str, ARGB(255, 255, 0, 0), 0);
//		gplib::graph::Draw_2DRefresh();
//
//#endif
//
		//カメラ位置あわせ
		//プレイヤーに合わせる必要があるためここで
		dxCamera->SetEyePos(v);
		//視点はプレイヤーの後ろ-５０
		dxCamera->SetLookPos(lookpos);
		//カメラからビュー行列と射影行列を設定
		meshManage->getG()->Setup3DEnv(dxCamera);
	}

	void Camera::render()
	{

	}

	void Camera::update()
	{
		input();
		//kaiten();
		camera(v2);
	}
}