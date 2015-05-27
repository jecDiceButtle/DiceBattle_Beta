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
    //��`�̏�����
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

//�I�t�Z�b�g�l�l���̋�`����
RECT MovableObject::getCollisionRect(int sx,int sy,int ex,int ey) const
{
  return makeDrawRect(sx,sy,ex,ey);
}

//�U����`����
RECT MovableObject::getOffenseRect() 
{
  return offenseRect;
}
//�����`����
RECT MovableObject::getDefenseRect() 
{
  return defenseRect;
}

//�w��̍��W����ʓ��Ȃ��(�����\��ԂȂ��)�^��Ԃ��@
bool MovableObject::InScreen(int Border){
	RECT rt = gplib::camera::GetScreenRect();
	if (x() < rt.left - Border || y() < rt.top - Border ||
			x() > rt.right + Border || y() > rt.bottom + Border){
		return false;
	}
	return true;
}

//�w��̍��W����ʓ��Ȃ��(�����\��ԂȂ��)�^��Ԃ�
//�R���W�����͌X���Ă��Ȃ��O��
//���S�_���甠�̃T�C�Y�c����
bool MovableObject::InScreen3D(float lx, float ly, float lz,float cx, float cy, float cz ){
	if (cx - lx / 2 < x() && cx + lx / 2 > x() &&
			cy - ly / 2 < y() && cy + ly / 2 > y() &&
			cz - lz / 2 < z() && cz + lz / 2 > z()){
		return true;
	}
	return false;
}

//�w��̍��W����ʓ��Ȃ��(�����\��ԂȂ��)�^��Ԃ�
//�R���W�����͌X���Ă��Ȃ��O��
bool MovableObject::InScreen3D(ci_ext::Vec3f length, ci_ext::Vec3f center){
  if (center.x() - length.x() / 2 < x() && center.x() + length.x() / 2 > x() &&
    center.y() - length.y() / 2 < y() && center.y() + length.y() / 2 > y() &&
    center.z() - length.z() / 2 < z() && center.z() + length.z() / 2 > z()){
		return true;
	}
	return false;
}


}