#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"

#include "../../lib/ci_ext/vec3.hpp"


namespace game
{

	class Button : public MovableObject
{
	//*************************************************//
	//　定数
	//*************************************************//
private:
	

	//*************************************************//
	//　変数
	//*************************************************//
private:
	POINT	pt_;
	RECT	rc_;
	float	scaleX, scaleY;
	u_char	a, r, g, b;


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
	Button(const std::string& objectName);

	
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
		@brief							ボタン作成とボタンとマウスのあたり判定
		@param[in] resname				画像の名前
		@param[in] posx					ボタンの位置X
		@param[in] posy					ボタンの位置Y
		@param[in] flag					trueでイベント発生(判定内マウス左クリックでtrue)
		@return							flag
	*/
	void ButtonClick(std::string resname,float posx, float posy,bool& flag);
	



};

}