#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/counter.hpp"
#include <iostream>
#include <Windows.h>
#include "../../lib/gplib.h"
//3D処理外部ライブラリ
#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
//Xinputライブラリ
#include "../../lib/ci_ext/xinput.hpp" 
//ロゴスクリーン
#include "stage.h"
#include "Title.hpp"
#include "Logo.hpp"

#include "effect.h"



/////////////////////////////////////////////////////////////////////////
//
//　　　window準備
//
//////////////////////////////////////////////////////////////////////////

namespace gplib{
	namespace system{
		const char USERNAME[256] = "Dice Battle";
		const int	WINW = 1280;
		const int	WINH = 720;
		bool	WindowMode = true;								//フルスクリーン対応
		int		KeyboardMode = 0;									//pad disable:1 pad enable:0
	}
}
int ci_ext::Object::uid_ = 0;

/////////////////////////////////////////////////////////////////////////
//
//　　　3D：3d関連準備
//
//////////////////////////////////////////////////////////////////////////

//3Dカメラ
std::shared_ptr<Dx_Camera> dxCamera = nullptr;
//メッシュ全体管理クラス
std::shared_ptr<DX3DMESHMANAGER> meshManage = nullptr;



/////////////////////////////////////////////////////////////////////////
//
//　　　ゲームクラス宣言
//
//////////////////////////////////////////////////////////////////////////


//ゲームクラス
//ルートになるクラスで、mainで宣言する。
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

	//コンストラクタでは自分のweak_ptrを取得できないので別途ポインタを登録する関数を作成
	void initGame(const std::weak_ptr<Game>& this_object_pointer)
	{
		//まず自分自身のポインタをつける
		setWeakPtr(this_object_pointer);

		//最初に生成するオブジェクトを記述
		//insertAsChild(new game::Logo("Loge")); //!)ここではロゴに移動
		insertAsChild(new game::Title("Title")); //!)ここではロゴに移動
		//insertAsChild(new game::CSceneStage("scene_stage"));
	}

	//アクセサ
	int frame() const { return frame_; }

	//お作法
	void render() override {}
	void update() override { ++frame_; }
};

/////////////////////////////////////////////////////////////////////////
//　　　?)謎記述
//		つまりどういうこと？
//////////////////////////////////////////////////////////////////////////

//クラスに内包できるが、改造が多くなるのでグローバルにおく
std::shared_ptr<Game> root;


/////////////////////////////////////////////////////////////////////////
//
//　　　エントリポイント　windows関連の準備
//
//////////////////////////////////////////////////////////////////////////

//エントリポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return gplib::system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}

/////////////////////////////////////////////////////////////////////////
//
//　　　ゲームループ
//			ゲームを行う上で必要なものの用意(ループ命令、input命令)
//////////////////////////////////////////////////////////////////////////


void gplib::system::GameLoop()
{
	//  std::cout << "==" << root->frame() << "回目" << std::endl;
	XInput::CheckKey();

	//毎フレーム呼ぶ
	//２Ｄ描画処理
	//	graph::Draw_Clear();  //2DClearと3Dclearを一括で行う関数(ここでは下で分けて書いてある)
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
//　　　ゲーム準備
//			ロードやカメラ準備など？
//////////////////////////////////////////////////////////////////////////
void gplib::system::OnCreate()
{

	//ルートオブジェクト生成
	root = std::make_shared<Game>("Root");
	root->initGame(root);

	//エフェクトロード
	effect::Init();
	effect::LoadReses();

	//フォント
	gplib::font::Draw_CreateFont(0, 30, "メイリオ");
	gplib::font::Draw_CreateFont(1, 40, "メイリオ");
	gplib::font::Draw_CreateFont(2, 50, "メイリオ");




	/////////////////////////////////////////////////////////////////////////
	//カメラの準備
	//////////////////////////////////////////////////////////////////////////

	//カメラ初期化
	gplib::camera::InitCamera(gplib::system::WINW / 2, gplib::system::WINH / 2, 0, 0);

	//3Dカメラ設定　カメラ位置はデフォルト　注視点は0,0,0
	float eye_pos_x = 0.0f;
	float eye_pos_y = 100.0f;
	float eye_pos_z = -100.0f;
	float eye_look_x = 0.0f;
	float eye_look_y = 0.0f;
	float eye_look_z = 0.0f;

	/////////////////////////////////////////////////////////////////////////　　
	//?)このカメラは何に使うんだ？
	//////////////////////////////////////////////////////////////////////////
	dxCamera = std::shared_ptr<Dx_Camera>(new Dx_Camera);
	dxCamera->SetEyePos(DxVec3(eye_pos_x, eye_pos_y, eye_pos_z));
	dxCamera->SetLookPos(eye_look_x, eye_look_y, eye_look_z);

	/////////////////////////////////////////////////////////////////////////　　
	//?)meshManageとは一体
	//////////////////////////////////////////////////////////////////////////
	//ステージ３Dオブジェクト
	meshManage = std::shared_ptr<DX3DMESHMANAGER>(new DX3DMESHMANAGER);
	//カメラからビュー行列と射影行列を設定
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
	//カメラの破棄
	//SAFE_DELETE(dxCamera);
}