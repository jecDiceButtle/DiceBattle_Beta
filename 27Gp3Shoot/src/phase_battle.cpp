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

	void PhaseBattle::battleCalc(const std::string& dir, const ci_ext::Vec3i masu, const int no)
	{
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//指定されている方角のベクトルを取得
		auto vec = rule->getDir(dir);

		//確認するマスの計算
		auto checkmasu = masu + vec;

		//確認するマスの位置のダイスキーワードを取得
		auto key = rule->getDiceKeyword(checkmasu);

		//ダイスがなかった場合
		if (key.empty()){

			return;
		}
		//ダイスがあった場合
		else{

			//初回時だけ、敵ダイスのときのみとする。
			if (no == 0){
				if (!rule->checkEnemyDice(rule->getDiceKeyword(masu), key))
					return;
			}

			//移動するダイスのデータを保存
			movedice_.push_back(Data(dir, key));

			//再帰処理でもう一つ先のマスも確認
			battleCalc(dir, checkmasu, no + 1);

		}
	}

	void PhaseBattle::calc()
	{

		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//現在選択されているダイスのマス座標
		ci_ext::Vec3i pos = rule->getDiceMasu();

		//いちおう保険
		movedice_.clear();

		//ルールに左右上下のマスに敵のダイスがいるかどうか聞きに行く再帰処理を行う。
		//上下左右の向きと、ダイスのキーワードを保存する。
		battleCalc("west", pos, 0);
		battleCalc("east", pos, 0);
		battleCalc("north", pos, 0);
		battleCalc("south", pos, 0);

		//バトル発生しなかった場合は終了
		if (movedice_.empty()){
			state_ = END;
		}
		//発生した場合
		else
		{
			initPushAnim();
		}

	}
	void PhaseBattle::initPushAnim()
	{
		//==============================
		//	移動の初期化
		//==============================

		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//移動するダイス全て
		for (auto dice : movedice_){

			//移動先のマスの算出
			auto vec = rule->getDir(dice.dir_);				//移動量
			auto masu = rule->getDiceMasu(dice.key_);		//現在の座標
			auto pos = masu + vec;							//移動先

			//座標の更新
			rule->updateMasu(pos, dice.key_);

			if (rule->getBoardState(pos) < 0)
			{
			
				rule->setDiceShow(false, dice.key_);
			}

			//-----------------------
			//	移動命令
			//-----------------------

			//移動先のマスの命令文"x=Xマス座標,z=Zマス座標"
			std::string str = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

			//ダイスに送るメッセージ
			//"push,x座標,z座標,移動フレーム数"
			std::string msg = "push," + str + ",frame=" + std::to_string(PUSHSPEED);

			//ルールを介してダイスにメッセージを送る(strはRule判断用)
			std::string process = "movedice," + dice.key_;
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, process);

		}

		//アニメーションの実行
		p_anim = insertAsChild(new Animator("animator_pushdice", p_rule, Animator::SKIPON, PUSHSPEED));
		//状態を変更
		state_ = PUSHANIM;

	}
	void PhaseBattle::fallAnim()
	{
		//ダイスのキーワードとマップを確認して、落ちているようなら処理。
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		for (auto dice : movedice_){

			////移動先のマスの算出
			//auto vec = rule->getDir(dice.dir_);				//移動量
			//auto masu = rule->getDiceMasu(dice.key_);		//現在の座標
			//auto pos = masu + vec;							//移動先

			//座標の更新
			/*rule->updateMasu(pos, dice.key_);*/

			if (!(rule->getDiceShow(dice.key_)))
			{
				//ダイスに送るメッセージ
				//"push,x座標,z座標,移動フレーム数"
				std::string msg = "fall,frame=" + std::to_string(FALLSPEED);;

				//ルールを介してダイスにメッセージを送る(strはRule判断用)
				std::string process = "falldice," + dice.key_;
				ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, process);
			}


		}
		//アニメーションの実行
		p_anim = insertAsChild(new Animator("animator_pushdice", p_rule, Animator::SKIPON, FALLSPEED));
		//状態を変更
		
	}

	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//


	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(CALC),
		PUSHSPEED(12)
	{
		calc();
	}

	void PhaseBattle::init()
	{

	}
	//フレーム処理
	void PhaseBattle::update()
	{
		//tuika
		std::string msg;

		switch (state_)
		{
		case game::PhaseBattle::CALC:

			break;
		case game::PhaseBattle::PUSHANIM:
			
			//tuika
			if (ci_ext::weak_to_shared<Animator>(p_anim)->isfinish()){

				//アニメーションオブジェクト停止
				ci_ext::weak_to_shared<Animator>(p_anim)->kill();

				state_ = FALLANIM;
				fallAnim();

			}
				
			

			break;
		case game::PhaseBattle::FALLANIM:
			if (ci_ext::weak_to_shared<Animator>(p_anim)->isfinish()){

				//アニメーションオブジェクト停止
				ci_ext::weak_to_shared<Animator>(p_anim)->kill();

				state_ = END;

			}

			break;
		case game::PhaseBattle::END:
			std::string msg = "end,";
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();
			break;

		}
	}

	void PhaseBattle::resume()
	{
		runAll();
	}

}




///*
//*	ファイル名	：	phase_battle.cpp
//*	製作者		：	丸山洋一郎
//*	制作日		：	2015/06/05
//*	内容		：	バトルフェイズ実行時中のオブジェクト。
//*					【現在動作させているもの】
//*					・バトル計算
//*					・アニメーション
//*/
//#pragma once
//#include "phase_battle.h"
//#include "rule.h"
//#include "rule_move.h"
//#include "animator.hpp"
//
//
//namespace game
//{
//	//**************************************************************************************//
//	//作成するプログラムで必要となる変数、定数定義
//	//**************************************************************************************//
//
//
//	//**************************************************************************************//
//	//関数記述
//	//**************************************************************************************//
//
//
//	//**************************************************************************************//
//	//デフォルト関数
//	//**************************************************************************************//
//
//
//	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
//		Object(objectName),
//		p_rule(prule),
//		state_(CALC)
//	{}
//
//	void PhaseBattle::init()
//	{
//		//移動系オブジェクト
//		insertAsChildPause(new game::RuleMove("rule_move", p_rule));
//
//		//選択オブジェクト
//	}
//	//フレーム処理
//	void PhaseBattle::update()
//	{
//		switch (state_)
//		{
//		case game::PhaseBattle::CALC:
//
//			break;
//
//		case game::PhaseBattle::PUSHANIM:
//
//			break;
//
//		case game::PhaseBattle::FALLANIM:
//			break;
//
//		case game::PhaseBattle::END:
//			break;
//
//		}
//	}
//
//	void PhaseBattle::resume()
//	{
//		runAll();
//	}
//	
//}
//
