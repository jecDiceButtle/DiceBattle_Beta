/*
*	ファイル名	：	rule_turn.hpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/25
*	内容		：	フェーズやターンを仕切るオブジェクト。
*					基本的にゲーム進行中は最後まで存在する。
*					ルールオブジェクト（以下：親）からフェーズ変更をもらう。
*					変更後は、親に対しフェーズの状態を送る。
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
//#include "../../lib/gplib.h"

namespace game
{

	class RuleTurn : public ci_ext::Object
	{

		//*************************************************//
		//　定数
		//*************************************************//
	private:

		//プレイヤーターン
		enum TURNSTATE
		{
			null = -1,				//ターン未設定
			PLAYER_A,				//プレイヤー１
			PLAYER_B,				//プレイヤー２
		};

		//ターンフェイズ
		enum PHASESTATE
		{
			null = -1,				//ターン未設定
			SUMMON,					//召喚(アルファ保留）
			MAIN,					//移動
			BATTLE,					//対戦
			END,					//（とりあえず仮配置）
		};


		//*************************************************//
		//　変数
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ
		PHASESTATE						phase_;			//フェーズ
		TURNSTATE						turn_;			//ターン


		//*************************************************//
		//　関数
		//*************************************************//

		/*
			@brief		コンストラクタ
		*/
	public:
		RuleTurn(const std::string& objectName) : Object(objectName){}

		/*
			@param[in]	start 開始プレイヤー(0:1Pスタート ,1:2Pスタート)
		*/
		RuleTurn(const std::string& objectName, const int start)
			:
			Object(objectName),
			phase_(SUMMON)
		{
			if (start == 1) turn_ = PLAYER_A;
			else			turn_ = PLAYER_B;
							
		}

		//フレーム処理
		void update() override
		{

		}

		//メッセージ処理
		void receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg) override
		{
			//メッセージにnextphaseがあるならフェーズを移す
			if (msg.find("nextphase")){
				nextphase();
			}
		}

		//次のフェーズへ移動の受付
		void nextphase()
		{
			std::string msg("");
			switch (phase_)
			{
			case game::RuleTurn::PHASESTATE::SUMMON:
				phase_ = PHASESTATE::MAIN;
				msg = "main";
				break;

			case game::RuleTurn::PHASESTATE::MAIN:
				phase_ = PHASESTATE::BATTLE;
				msg = "battle";
				break;

			case game::RuleTurn::PHASESTATE::BATTLE:
				phase_ = PHASESTATE::END;
				msg = "end";
				break;

			case game::RuleTurn::PHASESTATE::END:

				//ターンの切り替え
				turn_ = ((turn_ == TURNSTATE::PLAYER_A) ? TURNSTATE::PLAYER_B : TURNSTATE::PLAYER_A);
				phase_ = PHASESTATE::SUMMON;
				msg = "summon";
				break;
			}
			
			//ルールオブジェクトに対し切り替え後のフェーズを伝える
			msg = (turn_ == TURNSTATE::PLAYER_A) ? "phase,p1," + msg : "phase,p2," + msg;
			postMsg(p_rule, msg);

		}

	};
}

