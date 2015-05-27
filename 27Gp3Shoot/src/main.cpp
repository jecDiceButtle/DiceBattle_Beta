#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/counter.hpp"
#include <iostream>
#include <Windows.h>
#include "../../lib/gplib.h"
//3D�����O�����C�u����
#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
//Xinput���C�u����
#include "../../lib/ci_ext/xinput.hpp" 
//���S�X�N���[��
#include "stage.h"
#include "Title.hpp"
#include "Logo.hpp"

#include "effect.h"



/////////////////////////////////////////////////////////////////////////
//
//�@�@�@window����
//
//////////////////////////////////////////////////////////////////////////

namespace gplib{
	namespace system{
		const char USERNAME[256] = "Dice Battle";
		const int	WINW = 1280;
		const int	WINH = 720;
		bool	WindowMode = true;								//�t���X�N���[���Ή�
		int		KeyboardMode = 0;									//pad disable:1 pad enable:0
	}
}
int ci_ext::Object::uid_ = 0;

/////////////////////////////////////////////////////////////////////////
//
//�@�@�@3D�F3d�֘A����
//
//////////////////////////////////////////////////////////////////////////

//3D�J����
std::shared_ptr<Dx_Camera> dxCamera = nullptr;
//���b�V���S�̊Ǘ��N���X
std::shared_ptr<DX3DMESHMANAGER> meshManage = nullptr;



/////////////////////////////////////////////////////////////////////////
//
//�@�@�@�Q�[���N���X�錾
//
//////////////////////////////////////////////////////////////////////////


//�Q�[���N���X
//���[�g�ɂȂ�N���X�ŁAmain�Ő錾����B
class Game : public ci_ext::Object
{
	int frame_;
public:
	Game(const std::string& name)
		:
		Object(name),
		frame_(0)
	{
	}

	//�R���X�g���N�^�ł͎�����weak_ptr���擾�ł��Ȃ��̂ŕʓr�|�C���^��o�^����֐����쐬
	void initGame(const std::weak_ptr<Game>& this_object_pointer)
	{
		//�܂��������g�̃|�C���^������
		setWeakPtr(this_object_pointer);

		//�ŏ��ɐ�������I�u�W�F�N�g���L�q
		//insertAsChild(new game::Logo("Loge")); //!)�����ł̓��S�Ɉړ�
		insertAsChild(new game::Title("Title")); //!)�����ł̓��S�Ɉړ�
		//insertAsChild(new game::CSceneStage("scene_stage"));
	}

	//�A�N�Z�T
	int frame() const { return frame_; }

	//����@
	void render() override {}
	void update() override { ++frame_; }
};

/////////////////////////////////////////////////////////////////////////
//�@�@�@?)��L�q
//		�܂�ǂ��������ƁH
//////////////////////////////////////////////////////////////////////////

//�N���X�ɓ���ł��邪�A�����������Ȃ�̂ŃO���[�o���ɂ���
std::shared_ptr<Game> root;


/////////////////////////////////////////////////////////////////////////
//
//�@�@�@�G���g���|�C���g�@windows�֘A�̏���
//
//////////////////////////////////////////////////////////////////////////

//�G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return gplib::system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}

/////////////////////////////////////////////////////////////////////////
//
//�@�@�@�Q�[�����[�v
//			�Q�[�����s����ŕK�v�Ȃ��̗̂p��(���[�v���߁Ainput����)
//////////////////////////////////////////////////////////////////////////


void gplib::system::GameLoop()
{
	//  std::cout << "==" << root->frame() << "���" << std::endl;
	XInput::CheckKey();

	//���t���[���Ă�
	//�Q�c�`�揈��
	//	graph::Draw_Clear();  //2DClear��3Dclear���ꊇ�ōs���֐�(�����ł͉��ŕ����ď����Ă���)
	graph::Draw_3DClear();

	root->updateWithChildren();
	effect::Step();

	root->renderWithChildren();
	graph::Draw_2DClear();
	effect::Draw();
	graph::Draw_2DRefresh();

	graph::Draw_3DRefresh();

}

/////////////////////////////////////////////////////////////////////////
//
//�@�@�@�Q�[������
//			���[�h��J���������ȂǁH
//////////////////////////////////////////////////////////////////////////
void gplib::system::OnCreate()
{

	//���[�g�I�u�W�F�N�g����
	root = std::make_shared<Game>("Root");
	root->initGame(root);

	//�G�t�F�N�g���[�h
	effect::Init();
	effect::LoadReses();

	//�t�H���g
	gplib::font::Draw_CreateFont(0, 30, "���C���I");
	gplib::font::Draw_CreateFont(1, 40, "���C���I");
	gplib::font::Draw_CreateFont(2, 50, "���C���I");




	/////////////////////////////////////////////////////////////////////////
	//�J�����̏���
	//////////////////////////////////////////////////////////////////////////

	//�J����������
	gplib::camera::InitCamera(gplib::system::WINW / 2, gplib::system::WINH / 2, 0, 0);

	//3D�J�����ݒ�@�J�����ʒu�̓f�t�H���g�@�����_��0,0,0
	float eye_pos_x = 0.0f;
	float eye_pos_y = 100.0f;
	float eye_pos_z = -100.0f;
	float eye_look_x = 0.0f;
	float eye_look_y = 0.0f;
	float eye_look_z = 0.0f;

	/////////////////////////////////////////////////////////////////////////�@�@
	//?)���̃J�����͉��Ɏg���񂾁H
	//////////////////////////////////////////////////////////////////////////
	dxCamera = std::shared_ptr<Dx_Camera>(new Dx_Camera);
	dxCamera->SetEyePos(DxVec3(eye_pos_x, eye_pos_y, eye_pos_z));
	dxCamera->SetLookPos(eye_look_x, eye_look_y, eye_look_z);

	/////////////////////////////////////////////////////////////////////////�@�@
	//?)meshManage�Ƃ͈��
	//////////////////////////////////////////////////////////////////////////
	//�X�e�[�W�RD�I�u�W�F�N�g
	meshManage = std::shared_ptr<DX3DMESHMANAGER>(new DX3DMESHMANAGER);
	//�J��������r���[�s��Ǝˉe�s���ݒ�
	meshManage->getG()->Setup3DEnv(dxCamera);

	meshManage->loadMesh("player1", "balance_025.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::ANIMMESH);
	meshManage->loadMesh("dice", "dice.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::MESH);
	meshManage->loadMesh("board", "board.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::MESH);

	meshManage->loadMesh("kuzira", "kuzira.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::MESH);
	meshManage->loadMesh("kinoko", "kinoko.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::MESH);
	meshManage->loadMesh("tori", "tori.x", "res/x/", DX3DMESHMANAGER::MESHTYPE::MESH);
}

void gplib::system::OnDestroy(){
	//SAFE_DELETE(meshManage);
	//�J�����̔j��
	//SAFE_DELETE(dxCamera);
}