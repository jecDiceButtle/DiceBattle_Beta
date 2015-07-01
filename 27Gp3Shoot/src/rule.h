#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class Rule : public ci_ext::Object
	{
		//*************************************************//
		//　定数
		//*************************************************//
	private:
		const std::map<std::string, ci_ext::Vec3i> dir_;	//ダイスの移動方向とベクトル


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

		std::map<std::string, DiceData>	dice_;		// ダイスデータ
		std::vector<std::vector<int>>	board_;		// ボードの情報


		std::weak_ptr<ci_ext::Object> p_player;		// プレイヤーのポインタ
		std::weak_ptr<ci_ext::Object> p_board;		// ボードのポインタ

		std::weak_ptr<ci_ext::Object> p_camera;		// カメラのポインタ


		//*************************************************//
		//　関数
		//*************************************************//


	public:

		Rule(const std::string& objectName);

		/*
		@brief				敵対しているかどうかの確認
		@param[in] keyA		自分のダイスキーワード
		@param[in] keyB		相手のダイスキーワード
		@return				敵対しているかどうか
		@true				敵対している
		@false				味方同士である
		*/
		bool checkEnemyDice(const std::string& keyA, const std::string& keyB);


		/*
		@brief				ダイスのマスの更新
		@param[in] pos		更新先のマス
		@param[in] dice		更新するダイスのキーワード
		*/
		void updateMasu(const ci_ext::Vec3i& pos);
		void updateMasu(const ci_ext::Vec3i& pos, const std::string& key);


		/*
		@brief				ダイスデータのキー作成
		@param[in] player	プレイヤーNo.(0 or 1)
		@param[in] dice		ダイスNo.
		@return				キー文字列
		*/

		std::string makeKeyword(const int player, const int dice);


		/*
		@brief			引数の場所のオブジェクトを確認
		@param[in] pos	確認する場所
		@return			オブジェクトの内容(0:なし、1:ダイス)
		*/
		int getExistObj(const ci_ext::Vec3i pos);

		/*
		@brief			引数の場所のダイスのキーワードを返す
		@param[in] pos	確認する場所(キーなしの場合、現在選択されたダイスを返す）
		@return			キーワード(ダイスがない場合、空データを返す)
		*/
		std::string getDiceKeyword();
		std::string getDiceKeyword(const ci_ext::Vec3i pos);


		/*
		@brief			キーのダイスのマス座標を取得
		@param[in] key	キーワード(入力なしの場合、現在選択されているダイスのマス）
		@return			マス
		*/
		ci_ext::Vec3i getDiceMasu();
		ci_ext::Vec3i getDiceMasu(const std::string& key);

		/*
		@brief			方角に対応した向きを返す
		@param			方角( x軸 : 東西、z軸：南北）
		@return			向きベクトル
		*/
		ci_ext::Vec3i getDir(const std::string& dir) const;

		/*
		@brief			引数の場所のボード状態を確認
		@param[in] pos	確認する場所
		@return			ボードの状態(-1:穴 0:通常)
		*/
		int getBoardState(const ci_ext::Vec3i pos);


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



		void init() override;
		void render() override;
		void update() override;


		//(井上)追加
		//
		//ゲッター　ダイスステート
		bool getDiceShow();
		bool getDiceShow(const std::string& key);
		ci_ext::Vec3i getDiceStartMasu();


		void setDiceShow(bool state, const std::string& key);
		void setDiceShow(bool state);
		//
		/////

	};

}

//
//#pragma once
//#include "../../lib/ci_ext/object.hpp"
//#include "../../lib/ci_ext/vec3.hpp"
//#include "../../lib/gplib.h"
//
//namespace game
//{
//	class Rule : public ci_ext::Object
//	{
//
//		//*************************************************//
//		//　構造体
//		//*************************************************//
//	private:
//
//		struct DiceData{
//			
//			std::weak_ptr<ci_ext::Object> p_dice;	// ダイスのポインタ
//
//			//int				no;					// ダイスのナンバー
//			ci_ext::Vec3i	masu;					// ダイスのマス座標
//			bool			show_;					// 画面に表示されているかどうか
//
//			//状態異常とかはココに追加
//			//表示非表示なんかもココでいいかも。
//		};
//
//		struct PlayerData
//		{
//			std::vector<DiceData>	dice_;			// プレイヤーの持つダイス
//			int						hp_;			// 体力
//		};
//
//		enum Turn{
//			PLAYER_A = 0,
//			PLAYER_B,
//		};
//
//		enum Phase{
//			SUMMON,
//			MAIN,
//			BATTLE,
//		};
//
//
//		//*************************************************//
//		//　変数
//		//*************************************************//
//	private:
//
//		Phase	phase_;								// 現在のフェイズ
//		Turn	turn_;								// 現在のターンプレイヤー
//		int		diceno_;							// 選択されているダイス番号
//
//
//		std::vector<PlayerData>			player_;	// プレイヤーデータ
//		std::vector<std::vector<int>>	board_;		// ボードの情報
//
//
//		std::weak_ptr<ci_ext::Object> p_player;		// プレイヤーのポインタ
//		std::weak_ptr<ci_ext::Object> p_board;		// ボードのポインタ
//
//		std::weak_ptr<ci_ext::Object> p_camera;		// カメラのポインタ
//	
//
//		//*************************************************//
//		//　関数
//		//*************************************************//
//
//
//	public:
//
//		Rule(const std::string& objectName);
//
//		/*
//			@brief				ダイスのマスの更新
//			@param[in] pos		更新先のマス
//			@param[in] dice		更新するダイス([0]プレイヤー,[1]ダイスNo.)
//		*/
//		void updateMasu(const ci_ext::Vec3i& pos);
//		void updateMasu(const ci_ext::Vec3i& pos, const std::vector<int>& dice);
//		/*
//			@brief			引数の場所のオブジェクトを確認
//			@param[in] pos	確認する場所
//			@return			オブジェクトの内容(0:なし、1:ダイス)
//		*/
//		int getExistObj(ci_ext::Vec3i pos);
//
//
//		/*
//			@brief			現在の選択されているダイス座標を取得
//			@return			なし
//		*/
//		ci_ext::Vec3i getDiceMasu();
//
//		/*
//			@brief			引数の場所のボード状態を確認
//			@param[in] pos	確認する場所
//			@return			ボードの状態(-1:穴 0:通常)
//		*/
//		int getBoardState(ci_ext::Vec3i pos);
//
//
//		/*
//			@brief				子に送信するメッセージ(命令文)の受付
//								msgの内容を子に送信する。
//			@ATTEMTION			msgの一番初めは必ず動作命令にすること
//			@param[in]	msg		送信する命令文
//			@param[in]	process	送信先のオブジェクトなどの内容
//			@return				なし
//		*/
//		void sendMsg(const std::string& msg, const std::string& process);
//
//		/*
//			@brief				フェーズの状態変更
//			@return				なし
//		*/
//		void NextPhase();
//
//	
//		
//		void init() override;
//		void render() override;
//		void update() override;
//
//
//		//(井上)追加
//		//
//		//ゲッター　ダイスステート
//		bool Rule::getDiceShow();
//
//		ci_ext::Vec3i getDiceStartMasu();
//
//		//
//		/////
//		
//	};
//
//}