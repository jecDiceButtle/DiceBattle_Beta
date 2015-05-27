#pragma once

#pragma comment(lib, "xinput.lib")
#include <XInput.h> 
#include <d3dx9.h> 
#include <vector>

namespace gplib
{

//Xinput関連
class XInput
{
public:
  //XInput互換パッドのボタン定義
  //コメントにデジタルとあるのはOnOffのみ判定でき、
  //アナログとあるのは0.0f~1.0fの間で取得できる
  enum class ButtonsID
  {
    //十字キー
    up = 0,     //デジタル上
    down = 1,   //デジタル下
    left = 2,   //デジタル左
    right = 3,  //デジタル右
    //スタート、バック
    start = 4,  //デジタルスタート
    back = 5,   //デジタルバック
    //アナログLR
    L_thumb = 6,//アナログL
    R_thumb = 7,//アナログR
    //トリガー
    L = 9,      //デジタルL
    R = 10,     //デジタルR
    A = 12,     //デジタルA
    B = 13,     //デジタルB
    X = 14,     //デジタルX
    Y = 15,     //デジタルY
    max
  };
  //XInputでは、最大４つまでのコントローラしか認識できないので、IDを増やしてもダメ
	enum class ID
	{
		controller_1P = 0,  //1PパッドのID
		controller_2P = 1,  //2PパッドのID
		controller_3P = 2,  //3PパッドのID
		controller_4P = 3,  //4PパッドのID
		max_controllers = 4 //パッドは4つまで
	};

private:
  //コントローラ単体処理
	class Controller
	{
	private:
		const float deadzone_ = static_cast<float>(0x7FFF) * 0.24f;//ニュートラル時のあそび値の範囲	microsoftのデフォルト値が24%指定なのでこうなった

    int id_;	//コントローラのID（0から3まで）
		XINPUT_STATE state_;//ボタン情報
		XINPUT_VIBRATION vibration_;//ぶるぶる情報
		bool connected_;//コネクトされているか

		WORD now_,//現フレームでのボタン情報
			prev_;	//前フレームでのボタン情報

    //ボタン情報
    struct ButtonState
    {
      int holdframe;//おされているフレーム数
      int pullframe;//はなされているフレーム数
    } framestate_[(int)ButtonsID::max];

		//アナログパッド情報の取得
		D3DXVECTOR2 getThumb(const short xx, const short yy) const
		{
			if (!connected_) return D3DXVECTOR2(0, 0);
			float x = xx >= 0 ? 32767.0f : 32768.0f;
			float y = yy >= 0 ? 32767.0f : 32768.0f;
			return D3DXVECTOR2(
				static_cast<float>(xx) / x,
				static_cast<float>(yy) / y
				);
		}

	public:
    //デフォルトコンストラクタ消去
    //コンパイルエラー出るときは、=deleteを{}に変更しprivate:へ
		Controller() = delete;
    
    //コントローラ初期化処理
    //　id　初期化対象のコントローラIDを指定
    Controller(ID id)
			:
			state_(),
			vibration_(),
			connected_(false),
			now_(0),
			prev_(0),
			id_(static_cast<int>(id))
		{
      for (auto& framestate : framestate_)
        framestate.holdframe = framestate.pullframe = 0;
		}

		//未使用
		void render()
		{
		}

		//毎フレーム呼び出す
		void update()
		{
			//前回のチェック
			prev_ = now_;

			//パッド抜けさしチェック
			if (XInputGetState(id_, &state_) != ERROR_SUCCESS)
			{
				connected_ = false;
				now_ = 0;
				return;
			}

			//以下、コントローラーが認識されているときの処理
			connected_ = true;

			//現在のボタン情報の取得
			now_ = state_.Gamepad.wButtons;

			//ニュートラル時のあそび値を消去させる（左）
			if ((state_.Gamepad.sThumbLX < deadzone_ &&
				   state_.Gamepad.sThumbLX > -deadzone_) &&
				  (state_.Gamepad.sThumbLY < deadzone_ &&
				   state_.Gamepad.sThumbLY > -deadzone_))
			{
				state_.Gamepad.sThumbLX = 0;
				state_.Gamepad.sThumbLY = 0;
			}

			//ニュートラル時のあそび値を消去させる（右）
			if ((state_.Gamepad.sThumbRX < deadzone_ &&
				   state_.Gamepad.sThumbRX > -deadzone_) &&
				  (state_.Gamepad.sThumbRY < deadzone_ &&
				   state_.Gamepad.sThumbRY > -deadzone_))
			{
				state_.Gamepad.sThumbRX = 0;
				state_.Gamepad.sThumbRY = 0;
			}

      //各ボタンに対し、おされているフレームはなされているフレームをカウント
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        ButtonsID buttonID = static_cast<ButtonsID>(i);
        //このフレームで対象ボタンがおされたかはなされたかでフレームカウンタを0クリア
        if (checkPush(buttonID) || checkPull(buttonID))
        {
          framestate_[i].holdframe = 0;
          framestate_[i].pullframe = 0;
        }
        //おされているかはなされているかでフレームカウンタをインクリメント
        if (checkPress(buttonID)) ++framestate_[i].holdframe;
        if (checkFree(buttonID))  ++framestate_[i].pullframe;
      }
		}

		//押されているかのチェック
    bool checkPress(const ButtonsID btn, int interval = 1) const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == flag && (framestate_[static_cast<int>(btn)].holdframe % interval) == 0);
		}
		//押されていないかのチェック
    bool checkFree(const ButtonsID btn, int interval = 1)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == 0 && (framestate_[static_cast<int>(btn)].pullframe % interval) == 0);
		}
		//押されたかのチェック
    bool checkPush(const ButtonsID btn)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) == 0) && ((now_ & flag) != 0);
		}
		//はなされたかのチェック
    bool checkPull(const ButtonsID btn)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) != 0) && ((now_ & flag) == 0);
		}

		//トリガースティック左の情報を取得する
		//0.0~1.0で返ってくる
		float leftTrigger() const
		{
			if (!connected_) return false;
			return static_cast<float>(state_.Gamepad.bLeftTrigger) / 255.0f;
		}

		//トリガースティック右の情報を取得する
		//0.0~1.0で返ってくる
		float rightTrigger() const
		{
			if (!connected_) return false;
			return static_cast<float>(state_.Gamepad.bRightTrigger) / 255.0f;
		}

		//アナログスティック左の情報を取得する
		//1.0~-1.0で返ってくる
		D3DXVECTOR2 leftThumb() const
		{
			return getThumb(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
		}

		//アナログスティック右の情報を取得する
		//1.0~-1.0で返ってくる
		D3DXVECTOR2 rightThumb() const
		{
			return getThumb(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
		}

		//パッドを振動させる
		//　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
		//　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
		void vibration(float leftpower, float rightpower)
		{
			vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
			vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
			XInputSetState(id_, &vibration_);
		}
		//パッドを振動させる
		//　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
		void vibrationLeft(float leftpower)
		{
			vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
			XInputSetState(id_, &vibration_);
		}
		//パッドを振動させる
		//　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
		void vibrationRight(float rightpower)
		{
			vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
			XInputSetState(id_, &vibration_);
		}
	};

	std::vector<Controller> controllers_;

	//各種コンストラクタ封印
  //関数ヘルパーのためにシングルトンモデルにしたため
	XInput(const XInput&){}
	XInput(const XInput&&){}
	XInput()
	{
		controllers_.clear();
		controllers_.push_back(Controller(ID::controller_1P));
		controllers_.push_back(Controller(ID::controller_2P));
		controllers_.push_back(Controller(ID::controller_3P));
		controllers_.push_back(Controller(ID::controller_4P));
	}

public:
  //インスタンス取得
	static XInput* GetInstance()
	{
		static XInput instance;
		return &instance;
	}
  //毎フレーム呼ぶ
	void update()
	{
		for (auto& controller: controllers_)
			controller.update();
	}
  //描画（なにもしない）
	void render()
	{
		for (auto& controller: controllers_)
			controller.render();
	}

	//押されているかのチェック
  bool checkPress(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].checkPress(btn, interval);
	}
	//押されていないかのチェック
  bool checkFree(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkFree(btn, interval);
	}
	//押されたかのチェック
  bool checkPush(const ButtonsID btn, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkPush(btn);
	}
	//はなされたかのチェック
  bool checkPull(const ButtonsID btn, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkPull(btn);
	}
  //1P~4Pまでのどれかのボタンが押されているかをチェック
  bool checkAnyPress(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPress(btn, 1)) return true;
    return false;
  }
  //1P~4Pまでのどれかのボタンが押されたかをチェック
  bool checkAnyPush(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPush(btn)) return true;
    return false;
  }
  //1P~4Pまでのどれかのボタンがはなされているかをチェック
  bool checkAnyFree(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkFree(btn, 1)) return true;
    return false;
  }
  //1P~4Pまでのどれかのボタンがはなされたかをチェック
  bool checkAnyPull(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPull(btn)) return true;
    return false;
  }
  //トリガースティック左の情報を取得する
	//0.0~1.0で返ってくる
	float leftTrigger(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].leftTrigger();
	}

	//トリガースティック右の情報を取得する
	//0.0~1.0で返ってくる
	float rightTrigger(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].rightTrigger();
	}

	//アナログスティック左の情報を取得する
	//1.0~-1.0で返ってくる
	D3DXVECTOR2 leftThumb(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].leftThumb();
	}

	//アナログスティック右の情報を取得する
	//1.0~-1.0で返ってくる
	D3DXVECTOR2 rightThumb(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].rightThumb();
	}

	//パッドを振動させる
	//　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
	//　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
	void vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibration(leftpower, rightpower);
	}

	//パッドを振動させる
	//　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
	void vibrationLeft(float leftpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibrationLeft(leftpower);
	}
	//パッドを振動させる
	//　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
	void vibrationRight(float rightpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibrationRight(rightpower);
	}


//関数でアクセスできるようラッパー関数を準備
  //毎フレーム呼びましょう
  static void CheckKey()
  {
    GetInstance()->update();
  }
  //押されているかチェック
  static bool CheckPress(const ButtonsID buttonID, int interval = 1, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPress(buttonID, interval, id);
  }
  //はなされているかチェック
  static bool CheckFree(const ButtonsID buttonID, int interval = 1, ID id = ID::controller_1P)
  {
    return GetInstance()->checkFree(buttonID, interval, id);
  }
  //押されたかのチェック
  static bool CheckPush(const ButtonsID btn, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPush(btn);
  }
  //はなされたかのチェック
  static bool CheckPull(const ButtonsID btn, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPull(btn, id);
  }
  //1P~4Pまでのどれかのボタンが押されているかをチェック
  static bool CheckAnyPress(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPress(btn);
  }
  //1P~4Pまでのどれかのボタンが押されているかをチェック
  static bool CheckAnyPush(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPush(btn);
  }
  //1P~4Pまでのどれかのボタンがはなされているかをチェック
  static bool CheckAnyFree(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyFree(btn);
  }
  //1P~4Pまでのどれかのボタンがはなされたかをチェック
  static bool CheckAnyPull(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPull(btn);
  }
  //トリガースティック左の情報を取得する
  //0.0~1.0で返ってくる
  static float LeftTrigger(ID id = ID::controller_1P)
  {
    return GetInstance()->leftTrigger(id);
  }

  //トリガースティック右の情報を取得する
  //0.0~1.0で返ってくる
  static float RightTrigger(ID id = ID::controller_1P)
  {
    return GetInstance()->rightTrigger(id);
  }

  //アナログスティック左の情報を取得する
  //1.0~-1.0で返ってくる
  static D3DXVECTOR2 LeftThumb(ID id = ID::controller_1P)
  {
    return GetInstance()->leftThumb(id);
  }

  //アナログスティック右の情報を取得する
  //1.0~-1.0で返ってくる
  static D3DXVECTOR2 RightThumb(ID id = ID::controller_1P)
  {
    return GetInstance()->rightThumb(id);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void Vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibration(leftpower, rightpower, id);
  }

  //パッドを振動させる
  //　leftpower  左手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationLeft(float leftpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibrationLeft(leftpower, id);
  }
  //パッドを振動させる
  //　rightpower 右手側のモーターを振動させる（0.0fから1.0fまで）
  static void VibrationRight(float rightpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibrationRight(rightpower, id);
  }
};

}