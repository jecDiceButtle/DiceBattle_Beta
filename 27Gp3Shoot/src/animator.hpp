/*
*	ファイル名	：	animator.hpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/27
*	内容		：	アニメーションを管理する。
*					フレーム管理と、強制終了を担う。
*					指定フレーム数になる、強制終了するとオブジェクトをkillする
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "animator.hpp"

//#include "../../lib/gplib.h"

namespace game
{

	class Animator : public ci_ext::Object
	{

		//*************************************************//
		//　定数
		//*************************************************//
	public:

		//アニメの種類
		enum STATE
		{
			SKIPON,
			SKIPOFF,
		};

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ
		STATE							state_;			//アニメの種類
		int								frame_;			//フレーム数
		bool							stepF_;			//実行中フラグ(true：実行中）


		//*************************************************//
		//　関数
		//*************************************************//

	private:

	public:

		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] prule		ルールポインタ
			@param[in] state		アニメーションの種類
			@param[in] method		処理内容
		*/
		Animator(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule, STATE state, const int frame)
			:
			Object(objectName),
			p_rule(prule),
			state_(state),
			stepF_(true),
			frame_(frame)
		{

		}
		
		/*
			@brief		アニメーション終了確認
		*/
		bool isfinish()
		{
			return !stepF_;
		}


		//フレーム処理
		void update() override
		{
			//実行中なら
			if (stepF_)
			{
				//アニメフレーム数
				if (--frame_ <= 0)
				{
					stepF_ = false;
				}
				
			}


			//状態に合わせた処理
			switch (state_)
			{
			case game::Animator::SKIPON:

				//強制終了
				if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
				{
					stepF_ = false;
				}
				break;

			case game::Animator::SKIPOFF:

				break;
			}
		}

	};
}

