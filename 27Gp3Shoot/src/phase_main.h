/*
*	ファイル名	：	phase_main.h
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/27
*	内容		：	メインフェイズ実行時中のオブジェクト。
*					【現在動作させているもの】
*					・移動
*					・選択
*					・アニメーション
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{
	class PhaseMain : public ci_ext::Object
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

		const int DICEMOVESPEED;

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

		/*
			@brief					アニメ中の処理
		*/
		void anim();

		/*
			@brief					ダイスの点滅処理
			@param[in]	clear		True:点滅 False:消灯
		*/
		void selectDice(const bool clear);

		
	public:
		/*
			@brief					ダイスの移動アニメーション
			@param[in] masu			移動先のマス座標
			@param[in] pParent		親のポインタ
		*/
		void moveDiceAnim(const std::string& masu);

		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] pParent		親のポインタ
		*/
		PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);


		~PhaseMain();

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

