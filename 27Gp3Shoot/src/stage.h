#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class CSceneStage : public ci_ext::Object
	{
	private:

		//*************************************************//
		//　定数
		//*************************************************//

		//状態遷移
		enum class STAGESTATE
		{
			STANDBY,				//ゲーム前
			PLAYING,				//ゲーム中
			RESULT,					//ゲームオーバー
		};

		//プレイヤーターン
		enum TURN
		{
			null = -1,				//ターン未設定
			PLAYER1,				//プレイヤー１
			PLAYER2,				//プレイヤー２
		};

		//ターンフェイズ
		enum class PHASE
		{
			null = -1,				//ターン未設定
			SUMMON,					//召喚(アルファ保留）
			MAIN,					//移動
			BUTTLE,					//対戦
			END,					//（とりあえず仮配置）
		};


		//*************************************************//
		//　変数
		//*************************************************//

	private:
		

		bool		phaseinit_;						// フェイズの更新フラグ
		bool		cutinF_;						// カットイン実行中フラグ,true:実行中

		std::weak_ptr<ci_ext::Object> p_player;		// プレイヤーのポインタ
		std::weak_ptr<ci_ext::Object> p_board;		// ボードのポインタ
		std::weak_ptr<ci_ext::Object> p_dm;			// ボードのポインタ


		//tuika
		std::weak_ptr<ci_ext::Object> p_camera;		// カメラのポインタ
	
		
		TURN		turn_;							// プレイヤーターン
		STAGESTATE	state_;							// 状態遷移
		PHASE		phase_;							// ターンフェイズ


		//*************************************************//
		//　関数
		//*************************************************//

		/*
			@brief			スタンバイ状態
			@return			なし
		*/
		void Standby();

		/*
			@brief			メイン状態
			@return			なし
		*/
		void Playing();

		/*
			@brief			終了状態
			@return			なし
		*/
		void Result();


		/*
			@brief			メッセージ送信
			@return			なし
		*/
		void postTurnAndPhaseMsg();


	public:

		CSceneStage(const std::string& objectName);

		
		

		/*
			@brief			フェイズの更新
			@return			なし
		*/
		void NextPhase();

		/*
			@brief					ダイス移動処理
			@param[in]	pos			移動先の座標
			@return					処理したかどうか
			@true					処理された
			@false					処理されていない
		*/
		bool checkMovableDice(const ci_ext::Vec3i &pos);


		void init() override;
		void render() override;
		void update() override;
	};

}