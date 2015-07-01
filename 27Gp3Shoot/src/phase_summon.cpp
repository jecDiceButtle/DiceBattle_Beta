
#pragma once
#include "phase_summon.h"
#include "rule.h"
#include "rule_move.h"
#include "animator.hpp"


namespace game
{
	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//


	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//

	void PhaseSummon::CalcDiceSummon()
	{
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		ci_ext::Vec3i pos = rule->getDiceStartMasu();
	
		bool setFlag = false;//セット可能なマスかどうかのフラグ

		
		if (!((rule->getBoardState(pos) != -1) && (rule->getExistObj(pos) == 0)))
		{
			for (int z = -1; z <= 1; ++z)
			{
				for (int x = -1; x <= 1; ++x)
				{
					ci_ext::Vec3i setpos = { x, 0, z };
					//ボードの確認
					if ((rule->getBoardState(pos + setpos) != -1) && (rule->getExistObj(pos + setpos) == 0))
						pos += setpos;
					break;

				}
			}
		}

		//=============================
		// ダイスの移動
		//=============================

		//ダイス移動
		
		rule->updateMasu(pos);

		frame_ = DICESUMMONSPEED;

		//移動先のマスの命令文"x=Xマス座標,z=Zマス座標"
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//ダイスに送るメッセージ
		//"move,x座標,z座標,移動フレーム数"
		std::string msg = "summon," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword() + masu + ",frame=" + std::to_string(frame_);

		//ルールを介してダイスにメッセージを送る(movediceはRule確認用)
		rule->sendMsg(msg, "summondice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

		
		state_ = ANIM;
	}


	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//


	PhaseSummon::PhaseSummon(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT)
	{}

	void PhaseSummon::init()
	{
		bool summonF = ci_ext::weak_to_shared<Rule>(p_rule)->getDiceShow();

		if (summonF)
		{
			state_ = END;
		}

		else
		{
			ci_ext::weak_to_shared<Rule>(p_rule)->setDiceShow(true);
			CalcDiceSummon();
		}
	}
	//フレーム処理
	void PhaseSummon::update()
	{
		switch (state_)
		{
		case game::PhaseSummon::WAIT:
			break;
		case game::PhaseSummon::ANIM:
			if (--frame_ <= 0)
			{
				state_ = END;
				break;
			}

			if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
			{
				state_ = END;
			}

			break;
		case game::PhaseSummon::END:
			std::string msg = "end," ;
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();

			kill();
			break;
		}
	}

	void PhaseSummon::resume()
	{
		runAll();
	}

}

