#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

namespace game
{

	class Player : public ci_ext::Object
	{
		//*************************************************//
		//　定数
		//*************************************************//
	private:
		static const int DEFAULTCOST;
		//追加要素　コストの描画関連
		//std::string resname_;
		ci_ext::Vec3f pos_1P;
		ci_ext::Vec3f pos_2P;

		const int COST_W = 30;
		const int COST_H = 30;
		//80前後がベスト
		const int COST_Space = 80;


		ci_ext::Vec3f lifepos_1P;
		ci_ext::Vec3f lifepos_2P;
		int life_posx;
		int srcW = 300;
		int LIFE_1P_W;
		int LIFE_2P_W;
		const int LIFE_W = 300;
		const int LIFE_H = 100;




	int turn_;
		//*************************************************//
		//　変数
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object> p_parent;				// 親のポインタ
		std::vector<int>cost;								// コスト


		//*************************************************//
		//　関数
		//*************************************************//
	private:


	public:

		/*
		@brief							コンストラクタ
		@param[in]	objectName			オブジェクト名
		@param[in]	ptr					親のポインタ
		@return							なし
		*/
		Player(const std::string& objectName, const std::weak_ptr<Object> ptr);



		/*
		@brief							描画
		@return							なし
		*/
		void render() override;

		/*
		@brief							フレーム更新
		@return							なし
		*/
		void update() override;

		/*
		@brief							コスト確認
		@param[in] id					プレイヤーID
		@return							コスト残ってるかどうか
		@true							残っている
		@false							残っていない
		*/
		bool isCostRemain(const int id);

		/*
		@brief							コスト減少
		@param[in] id					プレイヤーID
		@param[in] point				減らす量（基本値1）
		@return							なし
		*/
		void CostDecrease(const int id, const int point = 1);

		/*
		@brief							コストを戻す
		@param[in] id					プレイヤーID
		@return							なし
		*/
		void CostReset(const int id);

		/*
		@brief
		@param[in] id
		@return
		*/
		void VictryorDefeat(const int id);

	void receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg)override;



	};

}