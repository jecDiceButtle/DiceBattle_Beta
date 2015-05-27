#include <iostream>

#include "movableObject.h"
#include "../../lib/ci_ext/math.hpp"

namespace game
{
  using namespace ci_ext;

MovableObject::MovableObject(const ci_ext::DrawObjf& f)
  :
  ci_ext::DrawObjf(f)
{
    //矩形の初期化
    SetRect(&offenseRect, -1, -1, -1, -1);
    SetRect(&defenseRect, -1, -1, -1, -1);
}

MovableObject::MovableObject()
:
ci_ext::DrawObjf("",
ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), "", ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), ci_ext::Color(0xFFFFFFFF), 0)
{
  SetRect(&offenseRect, -1, -1, -1, -1);
  SetRect(&defenseRect, -1, -1, -1, -1);
}

MovableObject::MovableObject(const std::string& objectName)
:
ci_ext::DrawObjf(objectName,
ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), "", ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), ci_ext::Color(0xFFFFFFFF), 0)
{
  SetRect(&offenseRect ,- 1, -1, -1, -1);
  SetRect(&defenseRect, -1, -1, -1, -1);
}

void MovableObject::render()
{
  DrawObjf::render();
}

void MovableObject::update()
{
  offsetPos(velocity_);
  Vec3f hSize = halfsize<float>();
  if (!gplib::camera::InScreenCameraCenter(
    x(), y(),
    hSize.x(),
    hSize.y()))
  {
    kill();
  }
}

//オフセット値考慮の矩形生成
RECT MovableObject::getCollisionRect(int sx,int sy,int ex,int ey) const
{
  return makeDrawRect(sx,sy,ex,ey);
}

//攻撃矩形生成
RECT MovableObject::getOffenseRect() 
{
  return offenseRect;
}
//やられ矩形生成
RECT MovableObject::getDefenseRect() 
{
  return defenseRect;
}

//指定の座標が画面内ならば(処理可能状態ならば)真を返す　
bool MovableObject::InScreen(int Border){
	RECT rt = gplib::camera::GetScreenRect();
	if (x() < rt.left - Border || y() < rt.top - Border ||
			x() > rt.right + Border || y() > rt.bottom + Border){
		return false;
	}
	return true;
}

//指定の座標が画面内ならば(処理可能状態ならば)真を返す
//コリジョンは傾いていない前提
//中心点から箱のサイズ縦横奥
bool MovableObject::InScreen3D(float lx, float ly, float lz,float cx, float cy, float cz ){
	if (cx - lx / 2 < x() && cx + lx / 2 > x() &&
			cy - ly / 2 < y() && cy + ly / 2 > y() &&
			cz - lz / 2 < z() && cz + lz / 2 > z()){
		return true;
	}
	return false;
}

//指定の座標が画面内ならば(処理可能状態ならば)真を返す
//コリジョンは傾いていない前提
bool MovableObject::InScreen3D(ci_ext::Vec3f length, ci_ext::Vec3f center){
  if (center.x() - length.x() / 2 < x() && center.x() + length.x() / 2 > x() &&
    center.y() - length.y() / 2 < y() && center.y() + length.y() / 2 > y() &&
    center.z() - length.z() / 2 < z() && center.z() + length.z() / 2 > z()){
		return true;
	}
	return false;
}


}