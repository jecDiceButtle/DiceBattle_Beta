/*
*	ファイル名	：	phase_battle.cpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/06/05
*	内容		：	バトルフェイズ実行時中のオブジェクト。
*					【現在動作させているもの】
*					・バトル計算
*					・アニメーション
*/
#pragma once
#include "phase_battle.h"
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

	void PhaseBattle::calc()
	{
		//ルールに現在選択されたダイスの場所を聞く

		//ルールに左右上下のマスに敵のダイスがいるかどうか聞きに行く

		//いなかった場合はバトル終了


	}
	void PhaseBattle::pushAnim()
	{

	}
	void PhaseBattle::fallAnim()
	{

	}

	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//


	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(CALC)
	{}

	void PhaseBattle::init()
	{

	}
	//フレーム処理
	void PhaseBattle::update()
	{
		switch (state_)
		{
		case game::PhaseBattle::CALC:
			
			break;
		case game::PhaseBattle::PUSHANIM:

			break;

		case game::PhaseBattle::FALLANIM:
			break;

		case game::PhaseBattle::END:
			break;

		}
	}

	void PhaseBattle::resume()
	{
		runAll();
	}
	
}

