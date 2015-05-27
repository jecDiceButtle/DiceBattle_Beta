#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{

	class Back : public ci_ext::Object
{
	//*************************************************//
	//　定数
	//*************************************************//
private:
	

	//*************************************************//
	//　変数
	//*************************************************//
private:
	int BackW, BackH;
	std::string s_back;

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

	//back=画像の名前を入れて！ StageBack,TitleBack
	Back(const std::string& objectName, const std::string back);


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
		@brief							座標の移動確認
		@param[in]	pos					移動先の座標
		@return							移動可能かどうか
		@true							可
		@false							不可
	*/




};

}