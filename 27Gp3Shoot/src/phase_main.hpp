/*
*	ファイル名	：	phase_main.hpp
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
#include "rule.h"
#include "rule_move.hpp"

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

		};

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_parent;			//親(ルールオブジェクト)ポインタ


		//*************************************************//
		//　関数
		//*************************************************//
	private:

		
	public:
		/*
			@brief					ダイスの移動アニメーション
			@param[in] dir			移動方向
			@param[in] pParent		親のポインタ
		*/
		void moveDiceAnim(ci_ext::Vec3i dir)
		{
			state_ = ANIM;

			//選択の変更を不可に
			pauseFromChildren("rule_selelct");

			//アニメーションさせる
			//insertAsChild(new Animation("anim_dice", dir));
		}



		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] pParent		親のポインタ
		*/
		PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& pParent) :
			Object(objectName),
			p_parent(pParent),
			state_(WAIT)
		{}
		/*
			@brief					子の初期化
		*/
		void init() override
		{
			//移動系オブジェクト
			insertAsChild(new RuleMove("rule_move", p_parent));

			//選択オブジェクト
		}
		//フレーム処理
		void update() override
		{

		}


	};
}

