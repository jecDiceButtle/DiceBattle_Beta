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
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//


	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//
	void CSceneStage::postTurnAndPhaseMsg()
	{
		//フェーズ送信処理
		std::string s_phase = "phase=" + std::to_string((int)phase_);
		std::string s_turn = "player=" + std::to_string((int)turn_);

		postMsgAllChildren(s_phase + "," + s_turn);
	}
	

	void CSceneStage::NextPhase()
	{
		//カットイン実行中はフェーズ変更をできなくする。
		
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
			//プレイヤーターン交換処理
			turn_ = ((turn_ == TURN::PLAYER1) ? TURN::PLAYER2 : TURN::PLAYER1);
			phase_ = PHASE::SUMMON;
			
			break;
		}

		

	}
	bool CSceneStage::checkMovableDice(const ci_ext::Vec3i &pos)
	{

		auto board = ci_ext::weak_to_shared<Board>(p_board);
		//ボードがOKしてくれたら
		if (board->isMovablePosition(pos))
		{
			auto player = ci_ext::weak_to_shared<Player>(p_player);
			//プレイヤーのコストがあるのならば
			if (player->isCostRemain((int)turn_))
			{
				//プレイヤーのコストを減らす
				player->CostDecrease((int)turn_);
				return true;
			}
		}
		return false;
	}
	void CSceneStage::Standby()
	{
		//ステート切り替え//
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
		//カットインが消えたかどうかの確認
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

		//メインフェイズ処理
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


		//ステート切り替え//
		if (gplib::input::CheckPush(gplib::input::KEY_SPACE))
		{
			//state_ = CSceneStage::STAGESTATE::RESULT; //今はなし
		}
	}
	void CSceneStage::Result()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_SPACE))
		{
			/*kill();*/
			//シーン遷移
			//insertToParent(new Logo("scene_logo")); //今はなし
		}
	}


	//**************************************************************************************//
	//デフォルト関数
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
		//デバッグ用
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		//gplib::font::Draw_FontTextNC(0, 0, 0.f, "ゲーム画面", ARGB(255, 0, 0, 0), 0); //今はなし
		std::string str = " ";

		switch (turn_)
		{
		case game::CSceneStage::TURN::PLAYER1:
			//std::cout << "プレイヤー１" ;
			str = "プレイヤー１";
			
			break;
		case game::CSceneStage::TURN::PLAYER2:
			//std::cout << "プレイヤー２" ;
			str = "プレイヤー２";
			break;
		}
		gplib::font::Draw_FontTextNC(0, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);


		switch (phase_)
		{
		case game::CSceneStage::PHASE::SUMMON:
			//std::cout << ":召喚フェーズ" << std::endl;
			str = ":召喚フェーズ";
			
			break;
		case game::CSceneStage::PHASE::MAIN:
			//std::cout << ":メインフェーズ" << std::endl;
			str = ":メインフェーズ";
			
			break;
		case game::CSceneStage::PHASE::BUTTLE:
			//std::cout << ":バトルフェーズ" << std::endl;
			str = ":バトルフェーズ";
			break;
		case game::CSceneStage::PHASE::END:
			//std::cout << ":エンドフェーズ" << std::endl;
			str = ":エンドフェーズ";
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
			//  先攻後攻決めるフェイズ（未実装）
			//=================================================
		case game::CSceneStage::STAGESTATE::STANDBY:

			Standby();
			break;

			//=================================================
			//  ゲーム中のフェイズ
			//=================================================
		case game::CSceneStage::STAGESTATE::PLAYING:
			Playing();
			
			break;
			//=================================================
			//  リザルト
			//=================================================
			
		case game::CSceneStage::STAGESTATE::RESULT:
			Result();
			
			break;
		}

	}
}


