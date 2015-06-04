/*
*	ファイル名	：	rule.cpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/27
*	内容		：	ゲーム進行中の処理全般。全ての処理の核となる部分。
*					フェーズやプレイヤーデータなどの基本データを持つ。
*					ダイスなどのオブジェクトを所持。テキストで命令を送る。
*					ゲーム進行のフェーズを管理。生成すれば自動的に処理する。
*/


#include "rule.h"
#include "phase_main.h"
#include <memory>
#include <string>

#include "camera.h"
#include "back.h"
#include "dice.h"
#include "board.h"

#include <stdexcept>

namespace game
{
	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//


	//**************************************************************************************//
	//コンストラクタ
	//**************************************************************************************//

	Rule::Rule(const std::string& objectName)
		:
		Object(objectName),
		phase_(SUMMON),
		turn_(PLAYER_A)
	{
		diceno_ = 0;


		//++++++++++++++++++++++++++++++++++++++++++++
		//	ボードの状態（仮配置,-1が穴）
		board_ = {
			{ 0, 0, 0, 0 },
			{ 0, 0, -1, 0 },
			{ 0, 0, 0, 0 },
			{ -1, 0, 0, 0 }
		};
		//++++++++++++++++++++++++++++++++++++++++++++

	}
	void Rule::init()
	{
		//++++++++++++++++++++++++++++++++++++++++++++
		//	ダイスの開始マス座標（マジックナンバー）
		const ci_ext::Vec3i STARTMASU[2][2] =
		{
			{ ci_ext::Vec3i(1, 0, 0), ci_ext::Vec3i(3, 0, 0) },
			{ ci_ext::Vec3i(1, 0, 4), ci_ext::Vec3i(3, 0, 4) }
		};
		//++++++++++++++++++++++++++++++++++++++++++++

		//プレイヤーデータ二人分
		for (int i = 0; i < 2; i++){

			PlayerData	player;

			//ダイス分
			for (int j = 0; j < 2; j++){
				DiceData dice;
				dice.masu = STARTMASU[i][j];
				dice.p_dice = insertAsChild(new game::Dice("dice", gplib::math::GetRandom<int>(0, 2), i, dice.masu));
				dice.show_ = true;

				player.dice_.push_back(dice);
			}

			player_.push_back(player);
		}

		//オブジェクトの追加

		p_board = insertAsChild(new Board("board",board_));		//ボード
		p_camera = insertAsChild(new Camera("camera"));		//カメラ

		insertAsChild(new Back("stageback", "TitleBack"));	//背景

		//カットイン追加予定


		//フェーズオブジェクト追加
		//insertAsChild(new PhaseSummon("phase_summon", this->selfPtr()));
	}

	//**************************************************************************************//
	//　関数記述
	//**************************************************************************************//


	ci_ext::Vec3i Rule::getDiceMasu()
	{
		//選択されているダイスの座標を返す
		return player_[(int)turn_].dice_[diceno_].masu;
	}

	void Rule::updateMasu(const ci_ext::Vec3i& pos)
	{
		//現在選択されているダイス
		updateMasu(pos, { turn_, diceno_ });
	}
	void Rule::updateMasu(const ci_ext::Vec3i& pos, const std::vector<int>& dice)
	{
		player_[(int)turn_].dice_[diceno_].masu = pos;
	}

	void Rule::sendMsg(const std::string& msg, const std::string& process)
	{
		//=====================================
		//	送信先を決める
		//=====================================
		std::weak_ptr<ci_ext::Object> receiver;
		
		//processを分割し解読
		auto msgVec = gplib::text::split(process, ",");
		for (auto ms : msgVec){

			//移動のとき
			if (ms == "movedice")
			{
				//現在選択中のダイスに送信する
				receiver = player_[(int)turn_].dice_[diceno_].p_dice;
				break;
			}
			//選択のとき
			if (ms == "selectdice")
			{
				//現在選択中のダイスに送信する
				receiver = player_[(int)turn_].dice_[diceno_].p_dice;
				break;
			}
		}

		//=====================================
		//	送信
		//=====================================
		postMsg(receiver, msg);
	}
	
	int Rule::getExistObj(ci_ext::Vec3i pos)
	{

		//ダイスが居るか確認する
		for (auto p : player_){
			for (auto dPos : p.dice_){
				
				//ダイスが画面にいる場合のみ検索
				if (!dPos.show_)
					continue;
				//ダイスがいた場合
				if (dPos.masu == pos){
					return 1;
				}
			}
		}

		//何もなかった
		return 0;
	}

	int Rule::getBoardState(ci_ext::Vec3i pos)
	{
		try{
			return board_.at(pos.x()).at(pos.z());
		}
		catch(const std::out_of_range&) {
			
			//範囲外をはじく
			return -1;

		}
	}

	void Rule::NextPhase()
	{
		//++++++++++++++++++++++++++++++++++++++++++++
		//	カットイン待ち時間（マジックナンバー)
		int wait = 120;
		//++++++++++++++++++++++++++++++++++++++++++++


		//現在動作してるフェーズは削除
		//フェーズオブジェクト内でも削除させるが一応
		auto objs = getObjectsFromChildren({ "phase" });
		for (auto obj : objs){
			obj.lock()->kill();
		}
		
		//フェーズの変更と、次のフェーズの読み込み
		switch (phase_)
		{
		case game::Rule::Phase::SUMMON:
			phase_ = Phase::MAIN;
			insertAsChildSleep(new PhaseMain("phase_main", this->selfPtr()),wait);
			break;

		case game::Rule::Phase::MAIN:
			phase_ = Phase::BATTLE;
			//insertAsChild(new PhaseBattle("phase_battle", this->selfPtr()));
			break;

		case game::Rule::Phase::BATTLE:
			phase_ = Phase::SUMMON;
			//insertAsChild(new PhaseSummon("phase_summon", this->selfPtr()));

			//次のプレイヤーターンへ
			turn_ = ((turn_ == Turn::PLAYER_A) ? Turn::PLAYER_B : Turn::PLAYER_A);
			break;
		}

	}

	//**************************************************************************************//
	//　オーバーライド関数
	//**************************************************************************************//

	void Rule::render()
	{
		//デバッグ用
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		//ターン表示
		std::string str = "";
		switch (turn_)
		{
		case game::Rule::Turn::PLAYER_A:
			str = "プレイヤー１";
			break;
		case game::Rule::Turn::PLAYER_B:
			str = "プレイヤー２";
			break;
		}
		gplib::font::Draw_FontTextNC(0, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);

		//フェーズ表示
		switch (phase_)
		{
		case game::Rule::Phase::SUMMON:
			str = ":召喚フェーズ";
			break;
		case game::Rule::Phase::MAIN:
			str = ":メインフェーズ";
			break;
		case game::Rule::Phase::BATTLE:
			str = ":バトルフェーズ";
			break;
		}
		gplib::font::Draw_FontTextNC(120, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);

		gplib::graph::Draw_2DRefresh();
#endif

	}
	void Rule::update()
	{
		//仮配置
#ifdef _DEBUG
		if (gplib::input::CheckPush(gplib::input::KEY_BTN0)){
			NextPhase();
		}
#endif
	}
}


