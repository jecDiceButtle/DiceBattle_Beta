/*
*	ファイル名	：	rule_move.cpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/25
*	内容		：	ダイスの移動を仕切るオブジェクト。
*					とりあえずキー入力も受け付けているが、ダイスに持たせるかは未定。
*					基本的にメインフェイズのみ存在する。
*					ルールオブジェクトにデータをを確認。
*					確認後、命令文を親に送る。
*/
#pragma once
#include "rule_move.h"
#include "rule.h"
#include "phase_main.h"


namespace game
{

	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//


	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//

	void RuleMove::key()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_LEFT))
		{
			moveDice(ci_ext::Vec3i(-1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
		{
			moveDice(ci_ext::Vec3i(1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_UP))
		{
			moveDice(ci_ext::Vec3i(0, 0, 1));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
		{
			moveDice(ci_ext::Vec3i(0, 0, -1));
		}
	}

	void RuleMove::moveDice(const ci_ext::Vec3i& dir)
	{
		//=============================
		// ダイスが移動できるか確認
		//=============================
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//ダイスのマス座標から調べるマス座標を求める
		ci_ext::Vec3i pos = rule->getDiceMasu();
		pos += dir;

		//移動できないボード
		if (rule->getBoardState(pos) == -1)
			return;
		//障害物の確認
		if (rule->getExistObj(pos) != 0)
			return;

		//=============================
		// ダイスの移動
		//=============================

		//移動先のマスの命令文"x=Xマス座標,z=Zマス座標"
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//ダイス移動
		auto parent = ci_ext::weak_to_shared<game::PhaseMain>(getParentPtr());
		parent->moveDiceAnim(masu);
		//ダイスマス座標を更新
		rule->updateMasu(pos);


		//アニメーション中はキー入力を受け付けない
		this->pause();
	}

	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//

	RuleMove::RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule)
	{}

	//フレーム処理
	void RuleMove::update()
	{
		key();
	}


}

