#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class Rule : public ci_ext::Object
	{

		//*************************************************//
		//　構造体
		//*************************************************//
	private:

		struct DiceData{
			
			std::weak_ptr<ci_ext::Object> p_dice;	// ダイスのポインタ

			//int				no;						// ダイスのナンバー
			ci_ext::Vec3i	masu;					// ダイスのマス座標
			bool			show_;					// 画面に表示されているかどうか

			//状態異常とかはココに追加
			//表示非表示なんかもココでいいかも。
		};

		struct PlayerData
		{
			std::vector<DiceData>	dice_;			// プレイヤーの持つダイス
			int						hp_;			// 体力
		};

		enum Turn{
			PLAYER_A = 0,
			PLAYER_B,
		};

		enum Phase{
			SUMMON,
			MAIN,
			BATTLE,
		};


		//*************************************************//
		//　変数
		//*************************************************//
	private:

		Phase	phase_;								// 現在のフェイズ
		Turn	turn_;								// 現在のターンプレイヤー
		int		diceno_;							// 選択されているダイス番号


		std::vector<PlayerData>			player_;	// プレイヤーデータ
		std::vector<std::vector<int>>	board_;		// ボードの情報


		std::weak_ptr<ci_ext::Object> p_player;		// プレイヤーのポインタ
		std::weak_ptr<ci_ext::Object> p_board;		// ボードのポインタ

		std::weak_ptr<ci_ext::Object> p_camera;		// カメラのポインタ
	

		//*************************************************//
		//　関数
		//*************************************************//


	public:

		/*
			@brief			引数の場所のオブジェクトを確認
			@param[in] pos	確認する場所
			@return			オブジェクトの内容(0:なし、1:ダイス)
		*/
		int getExistObj(ci_ext::Vec3i pos);


		/*
			@brief			現在の選択されているダイス座標を取得
			@return			なし
		*/
		ci_ext::Vec3i getDiceMasu();

		/*
			@brief			引数の場所のボード状態を確認
			@param[in] pos	確認する場所
			@return			ボードの状態(-1:穴 0:通常)
		*/
		int getBoardState(ci_ext::Vec3i pos);


		/*
			@brief				子に送信するメッセージ(命令文)の受付
								msgの内容を子に送信する。
			@ATTEMTION			msgの一番初めは必ず動作命令にすること
			@param[in]	msg		送信する命令文
			@param[in]	process	送信先のオブジェクトなどの内容
			@return				なし
		*/
		void sendMsg(const std::string& msg, const std::string& process);

		/*
			@brief				フェーズの状態変更
			@return				なし
		*/
		void NextPhase();

	
		Rule(const std::string& objectName);
		void init() override;
		void render() override;
		void update() override;
		
	};

}