/*
*	ファイル名	：	rule_move.h
*	製作者		：	丸山洋一郎
*	制作日		：	2015/05/28
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
		void key();

		/*
			@brief					ダイスの移動
			@param[in] dir			移動する向き

		*/
		void moveDice(const std::string& dir);

		/*
			@brief					コンストラクタ
			@param[in] objectName	オブジェクト名
			@param[in] prule		ルールオブジェクトのポインタ
		*/
	public:
		RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);


		//フレーム処理
		void update() override;

		

	};
}

