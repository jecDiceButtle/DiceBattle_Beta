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

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ


		//*************************************************//
		//　関数
		//*************************************************//
	private:

		void calc();
		void pushAnim();
		void fallAnim();

		
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

