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

		const int DICEMOVESPEED = 40;		//ダイスの移動フレーム

		//アニメの種類
		enum STATE
		{
			DICEMOVE,						//ダイスの移動
		};

		//*************************************************//
		//　変数
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//親(ルールオブジェクト)ポインタ
		STATE							state_;			//アニメの種類
		std::string						method_;		//処理の詳細データ、masutorになる場合あり
		int								frame_;			//フレーム数
		bool							stepF_;			//実行中フラグ(true：実行中）


		//*************************************************//
		//　関数
		//*************************************************//

	private:
		/*
			@brief					ダイス移動時の初期化処理
									ダイスを移動させる命令を送る。
			@param[in] dir			方向(methodから入力),現在未使用
		*/
		void initMoveDice(const std::string& dir)
		{
			frame_ = DICEMOVESPEED;


			//移動先のマスの命令文"x=Xマス座標,z=Zマス座標"
			auto pos = ci_ext::weak_to_shared<Rule>(p_rule)->getDiceMasu();
			std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

			//ダイスに送るメッセージ
			//"move,x座標,z座標,移動フレーム数"
			std::string msg = "move," + masu+ ",frame=" + std::to_string(frame_);

			//ルールを介してダイスにメッセージを送る(strはRule判断用)
			auto str = "movedice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword();

			
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, str);

		}

		
	public:
		
		/*
			@brief		アニメーション終了確認
		*/
		bool isfinish()
		{
			return !stepF_;
		}


		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] prule		ルールポインタ
			@param[in] state		アニメーションの種類
			@param[in] method		処理内容
		*/
		Animator(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule, STATE state, const std::string& method = "")
			:
			Object(objectName),
			p_rule(prule),
			state_(state),
			method_(method),
			stepF_(true)
		{
			switch (state_)
			{
			case STATE::DICEMOVE:
				initMoveDice(method);
				break;
			}
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
				//強制終了
				if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
				{
					stepF_ = false;
				}
			}
		}

	};
}

