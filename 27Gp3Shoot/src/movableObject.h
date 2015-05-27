#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{

class MovableObject : public ci_ext::DrawObjf
{
protected:
	ci_ext::Vec3f velocity_;
  //攻撃判定、やられ判定を分ける場合はこの値を変更する
  //利用しない場合は、通常の座標が判定として利用される
  RECT    offenseRect;
  RECT    defenseRect;
public:
  MovableObject(const ci_ext::DrawObjf& f);
  MovableObject(const std::string& objectName);
  MovableObject();

  void render() override;
  void update() override;
  //汎用判定
  RECT getCollisionRect(int sx = 0, int sy = 0, int ex = 0, int ey = 0) const;
  //攻撃判定
  RECT getOffenseRect() ;
  //やられ判定
  RECT getDefenseRect() ;

	bool MovableObject::InScreen(int Border = 100);
	bool MovableObject::InScreen3D(float lx, float ly, float lz, float cx = 0.f, float cy = 0.f, float cz = 0.f);
  bool MovableObject::InScreen3D(ci_ext::Vec3f length, ci_ext::Vec3f center);

};

}