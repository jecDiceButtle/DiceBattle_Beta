#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"


namespace game
{

	class Board : public MovableObject
	{
		//*************************************************//
		//　定数
		//*************************************************//
	private:

		static const ci_ext::Vec3i MAPSIZE;			//マップの最大値
		ci_ext::Vec3f scale;						//表示の倍率
		const float panelLength;					//パネル一辺の長さ


		//*************************************************//
		//　変数
		//*************************************************//
	private:
		vector<vector<int>> panel_;				//パネル


		//*************************************************//
		//　関数
		//*************************************************//
	private:


	public:

		/*
			@brief							コンストラクタ
			@param[in]	objectName			オブジェクト名
			@return							なし
			*/
		Board(const std::string& objectName);
		Board(const std::string& objectName, const std::vector<std::vector<int>>& mapData_);


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


		///*
		//	@brief							座標の移動確認
		//	@param[in]	pos					移動先の座標
		//	@return							移動可能かどうか
		//	@true							可
		//	@false							不可
		//*/
		//bool isMovablePosition(const ci_ext::Vec3i &pos);

		void colorChange(int i,int j);			//未実装

		bool HolePos(const ci_ext::Vec3i &pos);			//穴の場所　動くかわからない


	};
}

