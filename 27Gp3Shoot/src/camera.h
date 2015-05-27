#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/dx_media_/dx3dmeshmanager.h"


namespace game
{

class Camera : public ci_ext::Object
{
	//*************************************************//
	//　定数
	//*************************************************//
private:
	enum Cameraflag{UP,DOWN,RIGHT,LEFT,FREE};


	//*************************************************//
	//　変数
	//*************************************************//
private:

	DxVec3 v;	//カメラの位置
	DxVec3 v2;	//カメラの速度
	DxVec3 lookpos;// 視点座標
	Cameraflag Cflag;

	float kakudo;

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
	Camera(const std::string& objectName);

	void kaiten();
	void input();
	void camera(DxVec3 v2);

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

};

}