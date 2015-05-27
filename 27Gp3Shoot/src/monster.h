#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

//tuika
#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"
//tuika

namespace game
{

	class Monster : public MovableObject
{
	//*************************************************//
	//　定数
	//*************************************************//
private:

	static const float OFFSETY;



	//*************************************************//
	//　変数
	//*************************************************//
private:
	int monster_num;			//モンスターのナンバー(変更でモデル変更)
	ci_ext::Vec3f m_pos;		//モンスターのpos(座標)
	ci_ext::Vec3f m_angle;		//モンスターの向き

	float angle;
	float count;				//モンスター上下アニメ？のカウント
	bool moveflag;				//モンスター上下アニメ？のフラグ
	//*************************************************//
	//　関数
	//*************************************************//
private:


public:



	//monsternum=0(KUJIRA)monsternum=1(KINOKO)monsternum=2(TORI)
	Monster(const std::string& objectName, const ci_ext::Vec3f pos, const int monsternum, const ci_ext::Vec3f angle);



	void render() override;


	void update() override;


	void monster_move(ci_ext::Vec3f p,ci_ext::Vec3f a);//ポインタ使って座標移動&アングル
	void monster_move(ci_ext::Vec3f p);

	void updownmove();

};

}