/*
*	ファイル名	：	phase_main.cpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/27
*	内容		：	メインフェイズ実行時中のオブジェクト。
*					【現在動作させているもの】
*					・移動
*					・選択
*					・アニメーション
*/
#pragma once
#include "phase_main.h"
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

	void PhaseMain::moveDiceAnim(const std::string& dir)
	{
		//==========================
		//	移動処理
		//==========================


		//移動先のマスの命令文"x=Xマス座標,z=Zマス座標"
		auto pos = ci_ext::weak_to_shared<Rule>(p_rule)->getDiceMasu();
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//ダイスに送るメッセージ
		//"move,x座標,z座標,移動フレーム数"
		std::string msg = "move," + masu + ",frame=" + std::to_string(DICEMOVESPEED);


		//ルールを介してダイスにメッセージを送る(strはRule判断用)
		auto str = "movedice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword();
		ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, str);


		//アニメーションさせる
		insertAsChild(new Animator("animator_movedice", p_rule, Animator::SKIPON, DICEMOVESPEED));
		state_ = ANIM;

	}
	void PhaseMain::anim()
	{
		//アニメーション終了したら
		auto obj = ci_ext::weak_to_shared<Animator>(getObjectFromChildren("animator_movedice"));

		if (obj->isfinish()){

			//アニメーションオブジェクト停止
			obj->kill();

			//入力を受け付ける
			runFromChildrens({ "rule" });
			state_ = END;

			std::string msg;

			msg = "end," ;

			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			/*ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();*/
		}
	}
	void PhaseMain::selectDice(const bool clear)
	{
		std::string msg,str;

		//オンオフ判別
		msg = clear ? "select,on" : "select,off";

		str = "selectdice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword();

		//メッセージ送信
		ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg,str);
	}


	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//


	PhaseMain::PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT),
		DICEMOVESPEED(12)	//マジックナンバー
	{}

	void PhaseMain::init()
	{
		//移動系オブジェクト
		insertAsChildPause(new game::RuleMove("rule_move", p_rule));

		//選択オブジェクト
		selectDice(true);
	}
	//フレーム処理
	void PhaseMain::update()
	{
		switch (state_)
		{
		case game::PhaseMain::WAIT:
			break;
		case game::PhaseMain::ANIM:

			anim();
			break;
		case game::PhaseMain::END:
			
			selectDice(false);
			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();
			this->kill();
			break;
		}
	}

	PhaseMain::~PhaseMain()
	{
		////デバッグ用。
		//selectDice(false);
	}

	void PhaseMain::resume()
	{
		runAll();
	}
}

