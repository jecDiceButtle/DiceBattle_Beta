#pragma once

#pragma comment(lib, "xinput.lib")
#include <XInput.h> 
#include <d3dx9.h> 
#include <vector>

namespace gplib
{

//Xinput�֘A
class XInput
{
public:
  //XInput�݊��p�b�h�̃{�^����`
  //�R�����g�Ƀf�W�^���Ƃ���̂�OnOff�̂ݔ���ł��A
  //�A�i���O�Ƃ���̂�0.0f~1.0f�̊ԂŎ擾�ł���
  enum class ButtonsID
  {
    //�\���L�[
    up = 0,     //�f�W�^����
    down = 1,   //�f�W�^����
    left = 2,   //�f�W�^����
    right = 3,  //�f�W�^���E
    //�X�^�[�g�A�o�b�N
    start = 4,  //�f�W�^���X�^�[�g
    back = 5,   //�f�W�^���o�b�N
    //�A�i���OLR
    L_thumb = 6,//�A�i���OL
    R_thumb = 7,//�A�i���OR
    //�g���K�[
    L = 9,      //�f�W�^��L
    R = 10,     //�f�W�^��R
    A = 12,     //�f�W�^��A
    B = 13,     //�f�W�^��B
    X = 14,     //�f�W�^��X
    Y = 15,     //�f�W�^��Y
    max
  };
  //XInput�ł́A�ő�S�܂ł̃R���g���[�������F���ł��Ȃ��̂ŁAID�𑝂₵�Ă��_��
	enum class ID
	{
		controller_1P = 0,  //1P�p�b�h��ID
		controller_2P = 1,  //2P�p�b�h��ID
		controller_3P = 2,  //3P�p�b�h��ID
		controller_4P = 3,  //4P�p�b�h��ID
		max_controllers = 4 //�p�b�h��4�܂�
	};

private:
  //�R���g���[���P�̏���
	class Controller
	{
	private:
		const float deadzone_ = static_cast<float>(0x7FFF) * 0.24f;//�j���[�g�������̂����ђl�͈̔�	microsoft�̃f�t�H���g�l��24%�w��Ȃ̂ł����Ȃ���

    int id_;	//�R���g���[����ID�i0����3�܂Łj
		XINPUT_STATE state_;//�{�^�����
		XINPUT_VIBRATION vibration_;//�Ԃ�Ԃ���
		bool connected_;//�R�l�N�g����Ă��邩

		WORD now_,//���t���[���ł̃{�^�����
			prev_;	//�O�t���[���ł̃{�^�����

    //�{�^�����
    struct ButtonState
    {
      int holdframe;//������Ă���t���[����
      int pullframe;//�͂Ȃ���Ă���t���[����
    } framestate_[(int)ButtonsID::max];

		//�A�i���O�p�b�h���̎擾
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
    //�f�t�H���g�R���X�g���N�^����
    //�R���p�C���G���[�o��Ƃ��́A=delete��{}�ɕύX��private:��
		Controller() = delete;
    
    //�R���g���[������������
    //�@id�@�������Ώۂ̃R���g���[��ID���w��
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

		//���g�p
		void render()
		{
		}

		//���t���[���Ăяo��
		void update()
		{
			//�O��̃`�F�b�N
			prev_ = now_;

			//�p�b�h���������`�F�b�N
			if (XInputGetState(id_, &state_) != ERROR_SUCCESS)
			{
				connected_ = false;
				now_ = 0;
				return;
			}

			//�ȉ��A�R���g���[���[���F������Ă���Ƃ��̏���
			connected_ = true;

			//���݂̃{�^�����̎擾
			now_ = state_.Gamepad.wButtons;

			//�j���[�g�������̂����ђl������������i���j
			if ((state_.Gamepad.sThumbLX < deadzone_ &&
				   state_.Gamepad.sThumbLX > -deadzone_) &&
				  (state_.Gamepad.sThumbLY < deadzone_ &&
				   state_.Gamepad.sThumbLY > -deadzone_))
			{
				state_.Gamepad.sThumbLX = 0;
				state_.Gamepad.sThumbLY = 0;
			}

			//�j���[�g�������̂����ђl������������i�E�j
			if ((state_.Gamepad.sThumbRX < deadzone_ &&
				   state_.Gamepad.sThumbRX > -deadzone_) &&
				  (state_.Gamepad.sThumbRY < deadzone_ &&
				   state_.Gamepad.sThumbRY > -deadzone_))
			{
				state_.Gamepad.sThumbRX = 0;
				state_.Gamepad.sThumbRY = 0;
			}

      //�e�{�^���ɑ΂��A������Ă���t���[���͂Ȃ���Ă���t���[�����J�E���g
      for (int i = 0; i < static_cast<int>(ButtonsID::max); ++i)
      {
        ButtonsID buttonID = static_cast<ButtonsID>(i);
        //���̃t���[���őΏۃ{�^���������ꂽ���͂Ȃ��ꂽ���Ńt���[���J�E���^��0�N���A
        if (checkPush(buttonID) || checkPull(buttonID))
        {
          framestate_[i].holdframe = 0;
          framestate_[i].pullframe = 0;
        }
        //������Ă��邩�͂Ȃ���Ă��邩�Ńt���[���J�E���^���C���N�������g
        if (checkPress(buttonID)) ++framestate_[i].holdframe;
        if (checkFree(buttonID))  ++framestate_[i].pullframe;
      }
		}

		//������Ă��邩�̃`�F�b�N
    bool checkPress(const ButtonsID btn, int interval = 1) const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == flag && (framestate_[static_cast<int>(btn)].holdframe % interval) == 0);
		}
		//������Ă��Ȃ����̃`�F�b�N
    bool checkFree(const ButtonsID btn, int interval = 1)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((now_ & flag) == 0 && (framestate_[static_cast<int>(btn)].pullframe % interval) == 0);
		}
		//�����ꂽ���̃`�F�b�N
    bool checkPush(const ButtonsID btn)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) == 0) && ((now_ & flag) != 0);
		}
		//�͂Ȃ��ꂽ���̃`�F�b�N
    bool checkPull(const ButtonsID btn)  const
		{
			if (!connected_) return false;
      WORD flag = 1 << static_cast<int>(btn);
      return ((prev_ & flag) != 0) && ((now_ & flag) == 0);
		}

		//�g���K�[�X�e�B�b�N���̏����擾����
		//0.0~1.0�ŕԂ��Ă���
		float leftTrigger() const
		{
			if (!connected_) return false;
			return static_cast<float>(state_.Gamepad.bLeftTrigger) / 255.0f;
		}

		//�g���K�[�X�e�B�b�N�E�̏����擾����
		//0.0~1.0�ŕԂ��Ă���
		float rightTrigger() const
		{
			if (!connected_) return false;
			return static_cast<float>(state_.Gamepad.bRightTrigger) / 255.0f;
		}

		//�A�i���O�X�e�B�b�N���̏����擾����
		//1.0~-1.0�ŕԂ��Ă���
		D3DXVECTOR2 leftThumb() const
		{
			return getThumb(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
		}

		//�A�i���O�X�e�B�b�N�E�̏����擾����
		//1.0~-1.0�ŕԂ��Ă���
		D3DXVECTOR2 rightThumb() const
		{
			return getThumb(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
		}

		//�p�b�h��U��������
		//�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
		//�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
		void vibration(float leftpower, float rightpower)
		{
			vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
			vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
			XInputSetState(id_, &vibration_);
		}
		//�p�b�h��U��������
		//�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
		void vibrationLeft(float leftpower)
		{
			vibration_.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftpower);
			XInputSetState(id_, &vibration_);
		}
		//�p�b�h��U��������
		//�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
		void vibrationRight(float rightpower)
		{
			vibration_.wRightMotorSpeed = static_cast<WORD>(65535.0f * rightpower);
			XInputSetState(id_, &vibration_);
		}
	};

	std::vector<Controller> controllers_;

	//�e��R���X�g���N�^����
  //�֐��w���p�[�̂��߂ɃV���O���g�����f���ɂ�������
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
  //�C���X�^���X�擾
	static XInput* GetInstance()
	{
		static XInput instance;
		return &instance;
	}
  //���t���[���Ă�
	void update()
	{
		for (auto& controller: controllers_)
			controller.update();
	}
  //�`��i�Ȃɂ����Ȃ��j
	void render()
	{
		for (auto& controller: controllers_)
			controller.render();
	}

	//������Ă��邩�̃`�F�b�N
  bool checkPress(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].checkPress(btn, interval);
	}
	//������Ă��Ȃ����̃`�F�b�N
  bool checkFree(const ButtonsID btn, int interval = 1, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkFree(btn, interval);
	}
	//�����ꂽ���̃`�F�b�N
  bool checkPush(const ButtonsID btn, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkPush(btn);
	}
	//�͂Ȃ��ꂽ���̃`�F�b�N
  bool checkPull(const ButtonsID btn, ID id = ID::controller_1P)  const
	{
    return controllers_[static_cast<int>(id)].checkPull(btn);
	}
  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  bool checkAnyPress(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPress(btn, 1)) return true;
    return false;
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^���������ꂽ�����`�F�b�N
  bool checkAnyPush(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPush(btn)) return true;
    return false;
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ���Ă��邩���`�F�b�N
  bool checkAnyFree(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkFree(btn, 1)) return true;
    return false;
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ��ꂽ�����`�F�b�N
  bool checkAnyPull(const ButtonsID btn) const
  {
    for (const auto& controller : controllers_)
      if (controller.checkPull(btn)) return true;
    return false;
  }
  //�g���K�[�X�e�B�b�N���̏����擾����
	//0.0~1.0�ŕԂ��Ă���
	float leftTrigger(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].leftTrigger();
	}

	//�g���K�[�X�e�B�b�N�E�̏����擾����
	//0.0~1.0�ŕԂ��Ă���
	float rightTrigger(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].rightTrigger();
	}

	//�A�i���O�X�e�B�b�N���̏����擾����
	//1.0~-1.0�ŕԂ��Ă���
	D3DXVECTOR2 leftThumb(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].leftThumb();
	}

	//�A�i���O�X�e�B�b�N�E�̏����擾����
	//1.0~-1.0�ŕԂ��Ă���
	D3DXVECTOR2 rightThumb(ID id = ID::controller_1P) const
	{
    return controllers_[static_cast<int>(id)].rightThumb();
	}

	//�p�b�h��U��������
	//�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
	//�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
	void vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibration(leftpower, rightpower);
	}

	//�p�b�h��U��������
	//�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
	void vibrationLeft(float leftpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibrationLeft(leftpower);
	}
	//�p�b�h��U��������
	//�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
	void vibrationRight(float rightpower, ID id = ID::controller_1P)
	{
    controllers_[static_cast<int>(id)].vibrationRight(rightpower);
	}


//�֐��ŃA�N�Z�X�ł���悤���b�p�[�֐�������
  //���t���[���Ăт܂��傤
  static void CheckKey()
  {
    GetInstance()->update();
  }
  //������Ă��邩�`�F�b�N
  static bool CheckPress(const ButtonsID buttonID, int interval = 1, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPress(buttonID, interval, id);
  }
  //�͂Ȃ���Ă��邩�`�F�b�N
  static bool CheckFree(const ButtonsID buttonID, int interval = 1, ID id = ID::controller_1P)
  {
    return GetInstance()->checkFree(buttonID, interval, id);
  }
  //�����ꂽ���̃`�F�b�N
  static bool CheckPush(const ButtonsID btn, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPush(btn);
  }
  //�͂Ȃ��ꂽ���̃`�F�b�N
  static bool CheckPull(const ButtonsID btn, ID id = ID::controller_1P)
  {
    return GetInstance()->checkPull(btn, id);
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  static bool CheckAnyPress(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPress(btn);
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^����������Ă��邩���`�F�b�N
  static bool CheckAnyPush(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPush(btn);
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ���Ă��邩���`�F�b�N
  static bool CheckAnyFree(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyFree(btn);
  }
  //1P~4P�܂ł̂ǂꂩ�̃{�^�����͂Ȃ��ꂽ�����`�F�b�N
  static bool CheckAnyPull(const ButtonsID btn)
  {
    return GetInstance()->CheckAnyPull(btn);
  }
  //�g���K�[�X�e�B�b�N���̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  static float LeftTrigger(ID id = ID::controller_1P)
  {
    return GetInstance()->leftTrigger(id);
  }

  //�g���K�[�X�e�B�b�N�E�̏����擾����
  //0.0~1.0�ŕԂ��Ă���
  static float RightTrigger(ID id = ID::controller_1P)
  {
    return GetInstance()->rightTrigger(id);
  }

  //�A�i���O�X�e�B�b�N���̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  static D3DXVECTOR2 LeftThumb(ID id = ID::controller_1P)
  {
    return GetInstance()->leftThumb(id);
  }

  //�A�i���O�X�e�B�b�N�E�̏����擾����
  //1.0~-1.0�ŕԂ��Ă���
  static D3DXVECTOR2 RightThumb(ID id = ID::controller_1P)
  {
    return GetInstance()->rightThumb(id);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void Vibration(float leftpower, float rightpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibration(leftpower, rightpower, id);
  }

  //�p�b�h��U��������
  //�@leftpower  ���葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationLeft(float leftpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibrationLeft(leftpower, id);
  }
  //�p�b�h��U��������
  //�@rightpower �E�葤�̃��[�^�[��U��������i0.0f����1.0f�܂Łj
  static void VibrationRight(float rightpower, ID id = ID::controller_1P)
  {
    GetInstance()->vibrationRight(rightpower, id);
  }
};

}