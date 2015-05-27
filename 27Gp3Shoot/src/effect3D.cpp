#include	"../../lib/GpLib.h"
using namespace gplib;

#include "movableObject.h"
#include "effect3D.h"
#include <vector>
#include <algorithm>


// �r���{�[�h��]�s��擾�֐�
//   BillPos    : �r���{�[�h�̐���_�̈ʒu
//   TargetPos  : �r���{�[�h�����������_
//   Rpt        : ��]�s��
void GetBillBoardRotation(D3DXVECTOR3 *BillPos, D3DXVECTOR3 *TargetPos, D3DXMATRIX *Rot)
{
	D3DXMatrixIdentity(Rot);
	D3DXMatrixLookAtLH(Rot, TargetPos, BillPos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(Rot, NULL, Rot);
	Rot->_41 = 0.0f;   // �I�t�Z�b�g��؂�i��]�s�񂾂��ɂ��Ă��܂��j
	Rot->_42 = 0.0f;
	Rot->_43 = 0.0f;
}

namespace effect3D
{

  //---------------------------
  //�G�t�F�N�g�̃J�E���g����
  //�J�E���g�I����false
  //---------------------------
  bool Effect3D::count()
  {
    count_--;
    if (count_ <= 0){
      return false;
    }
    return true;
  }

  //---------------------------
  //�G�t�F�N�g�̉�]����
  //��]�I����false
  //---------------------------
  bool Effect3D::angle()
  {
    degree_ += addAngle_;
    if (degree_ > 360){
      return false;
    }
    return true;
  }

  //---------------------------
  //�G�t�F�N�g�̃A�j������
  //�A�j���I����false
  //---------------------------
  bool Effect3D::anime()
  {
    //�A�j����0.1�t���[���X�V
    animCnt_++;
    if (animCnt_ >= animFrm_){
      animCnt_ = 0;
      src_.x(src_.x()+1);
      if (src_.x() >= animMax_){
        src_.x(0);
        return false;
      }
    }
    return true;
  }

  //---------------------------
  //�G�t�F�N�g�̃A���t�@����
  //�A���t�@�I����false
  //---------------------------
  bool Effect3D::alpha()
  {
    //�����悻0.5sec�ŏ���
    alpha_ -= system::ONEFRAME_TIME * 255.f * 2.f;
	color_.a(static_cast<unsigned int>(alpha_));
    if (alpha_ < 0){
      return false;
    }
    return true;
  }

  //---------------------------
  //�G�t�F�N�g�̈ړ�����
  //��ʊO��false
  //---------------------------
  bool Effect3D::move()
  {
    pos_.offset(velocity_);
    if (InScreen3D(1000.f,1000.f,1000.f)){
      return false;
    }
    return true;
  }

  bool Effect3D::scaling()
  {
    offsetScale(ci_ext::Vec3f(1.f, 1.f, 0.f));
    if (scale_.x() > 50.f){
      return false;
    }
    return true;
  }

  //����
  void Effect3D::createBomb(float x, float y, float z ){
    pos_.x(x);
    pos_.y(y);
    pos_.z(z);
    //�g�嗦
    scale_.x(30.f);
    scale_.y(30.f);
    scale_.z(0.f);

    color_.set(100, 255, 0, 0);
    alpha_ = 255.f;

    resname_ = "explode3D";
    graph::Draw_LoadObject("explode3D", "res/gra/effect/bomb.png");

  }

  bool Effect3D::updateBomb(){
    return scaling();
  }
  //�ق������疂�@�̕�
  void Effect3D::createHouki(float x, float y, float z){
    pos_.x(x);
    pos_.y(y);
    pos_.z(z);
    //�g�嗦
    scale_.x(1.1f);
    scale_.y(1.1f);
    scale_.z(0.f);
    velocity_.set(
      math::GetRandom(-1.f, 1.f),
      math::GetRandom(1.f, 2.f),
      0.f
      );
    //10�t���[���ŏ���
    count_ = 10;
    color_.set(255, 
      math::GetRandom(200, 255), 
      math::GetRandom(200, 255), 
      math::GetRandom(200, 255));
    alpha_ = 255.f;

    resname_ = "Houki3D";
    graph::Draw_LoadObject("Houki3D", "res/gra/effect/effect_houki.png");

  }

  bool Effect3D::updateHouki(){
    move();
    return count();
  }

  Effect3D::Effect3D(float x, float y, float z, effect3D::EFFECTTYPE3D type)
    :
    MovableObject(ci_ext::DrawObjf("effect",
    ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), "", ci_ext::Vec3f::one(), ci_ext::Vec3f::one(), ci_ext::Color(0xFFFFFFFF), 0)),
    type_(type)
  {
    EffectFunc temp[] =
    {
      { &effect3D::Effect3D::createBomb, &effect3D::Effect3D::updateBomb },
      { &effect3D::Effect3D::createHouki, &effect3D::Effect3D::updateHouki },
    };
    eFunc = temp[type];
    (this->*eFunc.create)(x, y, z);
  }

  Effect3D::Effect3D(){}

  void Effect3D::render()
  {
    //for (const auto& effect3D : effect3Ds){
    ci_ext::Vec3f v = ci_ext::Vec3f(pos().x(), pos().y(), pos().z()-10);
//    graph::Draw_SetRenderMode(ADD, false);
    graph::Draw_Polygon(v, resname_, color_, ci_ext::Vec3f(scale_.x(), scale_.y(), (float)1.0f));

    //  for (int i = 0; i < effect3D.repertDraw; i++){
    //    D3DXVECTOR3 v = D3DXVECTOR3(effect3D.pos.pos.x, effect3D.pos.pos.y, effect3D.pos.pos.z);
    //    graph::Draw_Polygon(v, effect3D.pos.resname, effect3D.color, D3DXVECTOR3((float)effect3D.pos.scale.x, (float)effect3D.pos.scale.y, (float)1.0f));
    //  }
//    graph::Draw_SetRenderMode(NORMAL, false);
    //}
  }


  void Effect3D::update()
  {
    if ((this->*eFunc.step)() == false){
      kill();
    }
  }

  void Effect3D::LoadReses()
  {
    graph::Draw_LoadObject("shot", "res/gra/effect/shot2.png");
    graph::Draw_LoadObject("circle", "res/gra/effect/circle.png");
    graph::Draw_LoadObject("myst", "res/gra/effect/myst.png");
    
    graph::Draw_LoadObject("houki", "res/gra/effect/effect3D_houki.png");


  }




}
