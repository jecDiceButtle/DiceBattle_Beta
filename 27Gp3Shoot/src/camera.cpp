#include "camera.h"
#include "../../lib/gplib.h"

//3D�J����
extern LPDIRECT3DDEVICE9 pD3DDevice;
//���b�V���S�̊Ǘ��N���X
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;

namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************/	

	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	Camera::Camera(const std::string& objectName)
		:
		Object(objectName),
		kakudo(0)
	{

		v = DxVec3(45.f, 55.f, -60.f);	//�J�����̏����ʒu
		v2 = DxVec3(0.f, 0.f, 0.f);		//�J�����̑��x������
		lookpos = DxVec3(20.f, 10.f, 15.f);

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
		//�J�����ʒu���킹
		//�v���C���[�ɍ��킹��K�v�����邽�߂�����
		dxCamera->SetEyePos(v);
		//���_�̓v���C���[�̌��-�T�O
		dxCamera->SetLookPos(lookpos);
		//�J��������r���[�s��Ǝˉe�s���ݒ�
		meshManage->getG()->Setup3DEnv(dxCamera);
	}
	static int flag = 1;
	static int flag2 = 1;
	void Camera::render()
	{
		//gplib::graph::Draw_2DClear();
		//

		////�^�[���\��
		//std::string str1 = "���_�F" + to_string(lookpos.x) + " " + to_string(lookpos.y) + " " + to_string(lookpos.z) + " ";
		//
		//gplib::font::Draw_FontTextNC(120, 60, 0.f, str1, ARGB(255, 0, 0, 0), 0);

		//std::string str2 = "�J�����ʒu�F" + to_string(v.x) + " " + to_string(v.y) + " " + to_string(v.z) + " ";

		//gplib::font::Draw_FontTextNC(120, 100, 0.f, str2, ARGB(255, 0, 0, 0), 0);

		//gplib::graph::Draw_2DRefresh();
	}

	void Camera::update()
	{
		//input();
		////kaiten();
		camera(v2);

		//if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
		//{
		//	if (flag == 0)
		//	{
		//		flag = 1;
		//		return;
		//	}
		//	if (flag == 1){ 
		//		flag = 0;
		//		return;
		//	}
		//}
		//if (gplib::input::CheckPush(gplib::input::KEY_BTN1))
		//{
		//	if (flag2 == 0){
		//		flag2 = 1; 
		//		return;
		//	}
		//	if (flag2 == 1){ 
		//		flag2 = 2; 
		//		return;
		//	}
		//	if (flag2 == 2){ 
		//		flag2 = 0; 
		//		return;
		//	}
		//}
		//if (gplib::input::CheckPush(gplib::input::KEY_UP))
		//{
		//	switch ((flag2))
		//	{
		//	case 0:
		//		if (flag == 0) v.x += 5.f;
		//		if (flag == 1) lookpos.x += 5.f;
		//		break;
		//	case 1:
		//		if (flag == 0) v.y += 5.f;
		//		if (flag == 1) lookpos.y += 5.f;
		//		break;
		//	case 2:
		//		if (flag == 0) v.z += 5.f;
		//		if (flag == 1) lookpos.z += 5.f;
		//		break;
		//	default:
		//		break;
		//	}
		//}
		//if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
		//{
		//	switch ((flag2))
		//	{
		//	case 0:
		//		if (flag == 0) v.x -= 5.f;
		//		if (flag == 1) lookpos.x -= 5.f;
		//		break;
		//	case 1:
		//		if (flag == 0) v.y -= 5.f;
		//		if (flag == 1) lookpos.y -= 5.f;
		//		break;
		//	case 2:
		//		if (flag == 0) v.z -= 5.f;
		//		if (flag == 1) lookpos.z -= 5.f;
		//		break;
		//	default:
		//		break;
		//	}

		//}
		
	}
}