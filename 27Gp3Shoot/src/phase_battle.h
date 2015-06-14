/*
*	ファイル名	：	phase_battle.h
*	製作者		：	丸山洋一郎
*	制作日		：	2015/06/05
*	内容		：	バトルフェイズ実行時中のオブジェクト。
*					【現在動作させているもの】
*					・バトル計算
*					・アニメーション
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{
	class PhaseBattle : public ci_ext::Object
	{

		//*************************************************//
		//　定数
		//*************************************************//
	private:
		
		enum STATE{
			CALC,		//バトル計算中
			PUSHANIM,	//押し出しアニメーション中
			FALLANIM,	//落ちアニメーション中
			END,		//終了
		};

		const int PUSHSPEED;

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ

		struct Data
		{
			std::string dir_;							//移動先の方角
			std::string key_;							//ダイスのキーワード

			Data(){}
			Data(const std::string dir, const std::string key):
				dir_(dir), key_(key)
			{}

		};

		std::vector<Data>	movedice_;					//移動ダイスのデータ


		//*************************************************//
		//　関数
		//*************************************************//
	private:
		
		
		/*
			@brief					計算時処理
		*/
		void calc();
		/*
			@brief					移動処理の初期化
		*/
		void initPushAnim();
		/*
			@brief					落下時処理
		*/
		void fallAnim();


		/*
			@brief					バトルの計算（再帰処理）
									移動するダイスを見つけたら、movedice_へデータを追加していく。
			@param[in]	dir			移動先の方角
			@param[in]	masu		確認前の座標（現在の座標）
			@param[in]	no			再帰処理を繰り返した回数
		*/
		void battleCalc(const std::string& dir, const ci_ext::Vec3i masu, const int no);

		
	public:

		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] pParent		親のポインタ
		*/
		PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);
		/*
			@brief					子の初期化
		*/
		void init() override;
		
		/*
			@brief					フレーム処理
		*/
		void update() override;

		/*
			@brief					待機からの戻り処理
		*/
		void resume() override;


	};
}

