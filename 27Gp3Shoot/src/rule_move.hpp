/*
*	ファイル名	：	rule_move.hpp
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/25
*	内容		：	ダイスの移動を仕切るオブジェクト。
*					とりあえずキー入力も受け付けているが、ダイスに持たせるかは未定。
*					基本的にメインフェイズのみ存在する。
*					ルールオブジェクトにデータをを確認。
*					確認後、命令文を親に送る。
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"
#include "rule.h"
#include "phase_main.hpp"

namespace game
{
	class RuleMove : public ci_ext::Object
	{

		//*************************************************//
		//　定数
		//*************************************************//
	private:


		//*************************************************//
		//　変数
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;				//ルールのポインタ

		//*************************************************//
		//　関数
		//*************************************************//
	private:
		
		/*
			@brief		キー入力確認処理
		*/
		void key()
		{
			if (gplib::input::CheckPush(gplib::input::KEY_LEFT))
			{
				moveDice(ci_ext::Vec3i(-1, 0, 0));
			}
			if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
			{
				moveDice(ci_ext::Vec3i(1, 0, 0));
			}
			if (gplib::input::CheckPush(gplib::input::KEY_UP))
			{
				moveDice(ci_ext::Vec3i(0, 0, 1));
			}
			if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
			{
				moveDice(ci_ext::Vec3i(0, 0, -1));
			}
		}

		void moveDice(const ci_ext::Vec3i& dir)
		{
			//=============================
			// ダイスが移動できるか確認
			//=============================
			auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

			//ダイスのマス座標から調べるマス座標を求める
			ci_ext::Vec3i pos = rule->getDiceMasu();
			pos += dir;
			
			//移動できないボード
			if (rule->getBoardState(pos) == -1)
				return;
			//障害物の確認
			if (rule->getExistObj(pos) != 0)
				return;

			//=============================
			// ダイスの移動
			//=============================

			auto parent = ci_ext::weak_to_shared<PhaseMain>(getParentPtr());
			//移動先の向きを引数にしている（マス座標になる可能性あり）
			parent->moveDiceAnim(dir);

			//アニメーション中はキー入力を受け付けない
			this->pause();
		}

		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] prule		ルールオブジェクトのポインタ

		*/
	public:
		RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
			Object(objectName),
			p_rule(prule)
		{}

		//フレーム処理
		void update() override
		{
			key();
		}


	};
}

