/*
*	ファイル名	：	phase_summon.h
*	製作者		：	井上暁
*	制作日		：	2015/06/09
*	内容		：	メインフェイズ実行時中のオブジェクト。
*					【現在動作させているもの】
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{
	class PhaseSummon : public ci_ext::Object
	{

		//*************************************************//
		//　定数
		//*************************************************//
	private:

		enum STATE{
			WAIT,				//待機
			ANIM,				//アニメーション中
			END,				//フェーズ終了

		};

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ

		const int DICESUMMONSPEED = 90;		//ダイスの移動フレーム
		int								frame_;			//フレーム数


		//*************************************************//
		//　関数
		//*************************************************//
	private:

		/*
		@brief					アニメ中の処理
		*/
		/*void anim();*/

		/*
		@brief					ダイスの点滅処理
		@param[in]	clear		True:点滅 False:消灯
		*/
		//void selectDice(const bool clear);

		/*
		@brief					1frame計算
		*/
		void CalcDiceSummon();

	public:
		/*
		@brief					ダイスの移動アニメーション
		@param[in] masu			移動先のマス座標
		@param[in] pParent		親のポインタ
		*/
		/*void moveDiceAnim(const std::string& masu);*/

		/*
		@brief					コンストラクタ
		@param[in] objectName	オブジェクト名
		@param[in] pParent		親のポインタ
		*/
		PhaseSummon(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);
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

