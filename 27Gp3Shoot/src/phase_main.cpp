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

	void PhaseMain::moveDiceAnim(const std::string& masu)
	{
		state_ = ANIM;

		//選択の変更を不可に
		pauseFromChildren("rule_selelct");

		//アニメーションさせる
		insertAsChild(new Animator("animator_movedice", p_rule, Animator::DICEMOVE, masu));
	}


	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//


	PhaseMain::PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT)
	{}

	void PhaseMain::init()
	{
		//移動系オブジェクト
		insertAsChild(new game::RuleMove("rule_move", p_rule));

		//選択オブジェクト
	}
	//フレーム処理
	void PhaseMain::update()
	{
		switch (state_)
		{
		case game::PhaseMain::WAIT:
			break;
		case game::PhaseMain::ANIM:

			//アニメーション終了したら
			auto objs = getObjectsFromChildren({ "animator" });
			for (auto obj : objs){

				if (obj.lock()->isDead()){

					//入力を受け付ける
					runFromChildren({ "rule" }, {});
					state_ = WAIT;
				}


				break;
			}
		}
	}
}

