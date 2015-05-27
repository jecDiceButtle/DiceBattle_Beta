/*
*	ファイル名	：	rule.cpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/27
*	内容		：	ゲーム進行中の処理全般。
*					全ての処理の核となる部分。
*					ダイスなどのオブジェクトを所持。
*					ゲーム進行のフェーズを管理。
*/


#include "rule.h"
#include <memory>
#include <string>

namespace game
{
	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//


	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//
	
	//
	ci_ext::Vec3i Rule::getDiceMasu()
	{
		//選択されているダイスの座標を返す
		return player_[(int)turn_].dice_[diceno_].masu;
	}

	void Rule::sendMsg(const std::string& msg, const std::string& process)
	{
		//送信先を決める
		std::weak_ptr<ci_ext::Object> receiver;
		
		//processをメッセージ分割し解読
		auto msgVec = gplib::text::split(msg, ",");
		for (auto ms : msgVec){

			//=====================================
			//	処理ごと追加していく
			//=====================================

			//移動のとき
			if (ms == "movedice")
			{
				//現在選択中のダイス
				receiver = player_[(int)turn_].dice_[diceno_].p_dice;
			}
		}

		//送信
		postMsg(receiver, msg);
	}
	
	int Rule::getExistObj(ci_ext::Vec3i pos)
	{

		//ダイスが居るか確認する
		for (auto p : player_){
			for (auto dPos : p.dice_){
				
				//ダイスが画面にいる場合のみ検索
				if (dPos.show_)
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
		//範囲外をはじく
		if (board_.size() < pos.x() || board_[0].size() < pos.z())
			return -1;

		return board_[pos.x()][pos.z()];
	}

	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//

	Rule::Rule(const std::string& objectName)
		:
		Object(objectName)
	{

	}
	void Rule::init()
	{

	}
	void Rule::render()
	{
		//デバッグ用
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		gplib::graph::Draw_2DRefresh();
#endif

	}
	void Rule::update()
	{

	}
}


