#include "stage.h"
#include <memory>
#include <string>
#include <iostream>

#include "logo.hpp"
#include "player.h"
#include "dicemanager.h"
#include "board.h"

//tuika
#include "camera.h"
#include "back.h"
#include "ui.h"

#include <stdio.h>


namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//
	void CSceneStage::postTurnAndPhaseMsg()
	{
		//�t�F�[�Y���M����
		std::string s_phase = "phase=" + std::to_string((int)phase_);
		std::string s_turn = "player=" + std::to_string((int)turn_);

		postMsgAllChildren(s_phase + "," + s_turn);
	}
	

	void CSceneStage::NextPhase()
	{
		//�J�b�g�C�����s���̓t�F�[�Y�ύX���ł��Ȃ�����B
		
		if (cutinF_ || phaseinit_)
			return;

		phaseinit_ = true;

		switch (phase_)
		{
		case game::CSceneStage::PHASE::SUMMON:
			phase_ = PHASE::MAIN;

			break;

		case game::CSceneStage::PHASE::MAIN:
			phase_ = PHASE::BUTTLE;
			break;

		case game::CSceneStage::PHASE::BUTTLE:

			phase_ = PHASE::END;
			break;

		case game::CSceneStage::PHASE::END:
			insertAsChild(new UI("turn", game::UI::UITYPE::CUTINTURN));		//??
			//�v���C���[�^�[����������
			turn_ = ((turn_ == TURN::PLAYER1) ? TURN::PLAYER2 : TURN::PLAYER1);
			phase_ = PHASE::SUMMON;
			
			break;
		}

		

	}
	bool CSceneStage::checkMovableDice(const ci_ext::Vec3i &pos)
	{

		auto board = ci_ext::weak_to_shared<Board>(p_board);
		//�{�[�h��OK���Ă��ꂽ��
		if (board->isMovablePosition(pos))
		{
			auto player = ci_ext::weak_to_shared<Player>(p_player);
			//�v���C���[�̃R�X�g������̂Ȃ��
			if (player->isCostRemain((int)turn_))
			{
				//�v���C���[�̃R�X�g�����炷
				player->CostDecrease((int)turn_);
				return true;
			}
		}
		return false;
	}
	void CSceneStage::Standby()
	{
		//�X�e�[�g�؂�ւ�//
		if (true)
		{
			state_ = CSceneStage::STAGESTATE::PLAYING;
			phase_ = PHASE::SUMMON;
			turn_ = TURN::PLAYER1;
			phaseinit_ = true;
		}
	}
	void CSceneStage::Playing()
	{
		//�J�b�g�C�������������ǂ����̊m�F
		auto objects = getObjectsFromChildren({ "cutin" });
		if (!objects.empty())
		{
			for (auto obj : objects)
			{
				auto ui = ci_ext::weak_to_shared<game::UI>(obj);
				if(ui->isDestroy())
					cutinF_ = false;
			}
		}

		//���C���t�F�C�Y����
		if (phase_ == PHASE::MAIN || phase_ == PHASE::END)
		{
			if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
			{
				NextPhase();
			}
		}
		
		if (phaseinit_)
		{
			postTurnAndPhaseMsg();
			insertAsChild(new UI("cutin", game::UI::UITYPE::CUTINPHASE));
			cutinF_ = true;
			phaseinit_ = false;
		}


		//�X�e�[�g�؂�ւ�//
		if (gplib::input::CheckPush(gplib::input::KEY_SPACE))
		{
			//state_ = CSceneStage::STAGESTATE::RESULT; //���͂Ȃ�
		}
	}
	void CSceneStage::Result()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_SPACE))
		{
			/*kill();*/
			//�V�[���J��
			//insertToParent(new Logo("scene_logo")); //���͂Ȃ�
		}
	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//

	CSceneStage::CSceneStage(const std::string& objectName)
		:
		Object(objectName),
		state_(STAGESTATE::STANDBY),
		turn_(TURN::null),
		phase_(PHASE::null),
		cutinF_(false)
	{

	}
	void CSceneStage::init()
	{

		p_board = insertAsChild(new Board("board"));
		p_player = insertAsChild(new Player("player", this->selfPtr()));
		p_dm = insertAsChild(new DiceManager("dicemanager",this->selfPtr()));
		p_camera = insertAsChild(new Camera("camera"));
		
		insertAsChild(new UI("phase", game::UI::UITYPE::PHASE, gplib::system::WINW / 2.f, 100.f));
		insertAsChild(new UI("chara", game::UI::UITYPE::CHARA, 100, 120));
	
		insertAsChild(new Back("stageback","TitleBack"));

	}
	void CSceneStage::render()
	{
		//�f�o�b�O�p
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		//gplib::font::Draw_FontTextNC(0, 0, 0.f, "�Q�[�����", ARGB(255, 0, 0, 0), 0); //���͂Ȃ�
		std::string str = " ";

		switch (turn_)
		{
		case game::CSceneStage::TURN::PLAYER1:
			//std::cout << "�v���C���[�P" ;
			str = "�v���C���[�P";
			
			break;
		case game::CSceneStage::TURN::PLAYER2:
			//std::cout << "�v���C���[�Q" ;
			str = "�v���C���[�Q";
			break;
		}
		gplib::font::Draw_FontTextNC(0, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);


		switch (phase_)
		{
		case game::CSceneStage::PHASE::SUMMON:
			//std::cout << ":�����t�F�[�Y" << std::endl;
			str = ":�����t�F�[�Y";
			
			break;
		case game::CSceneStage::PHASE::MAIN:
			//std::cout << ":���C���t�F�[�Y" << std::endl;
			str = ":���C���t�F�[�Y";
			
			break;
		case game::CSceneStage::PHASE::BUTTLE:
			//std::cout << ":�o�g���t�F�[�Y" << std::endl;
			str = ":�o�g���t�F�[�Y";
			break;
		case game::CSceneStage::PHASE::END:
			//std::cout << ":�G���h�t�F�[�Y" << std::endl;
			str = ":�G���h�t�F�[�Y";
			break;
		}
		gplib::font::Draw_FontTextNC(120, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);

		gplib::graph::Draw_2DRefresh();
#endif

	}
	void CSceneStage::update()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_SPACE))
		{
			insertAsChild(new UI("cutin", game::UI::UITYPE::CUTINTURN));
		}

		switch (state_)
		{
			//=================================================
			//  ��U��U���߂�t�F�C�Y�i�������j
			//=================================================
		case game::CSceneStage::STAGESTATE::STANDBY:

			Standby();
			break;

			//=================================================
			//  �Q�[�����̃t�F�C�Y
			//=================================================
		case game::CSceneStage::STAGESTATE::PLAYING:
			Playing();
			
			break;
			//=================================================
			//  ���U���g
			//=================================================
			
		case game::CSceneStage::STAGESTATE::RESULT:
			Result();
			
			break;
		}

	}
}


