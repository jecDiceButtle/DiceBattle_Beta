#pragma once

namespace effect3D{

	enum EFFECTTYPE3D{
    BOMB,
    HOUKI,
    HIT,

    EFFECT_MAX,
	}; 
  class Effect3D : public game::MovableObject
  {
    effect3D::EFFECTTYPE3D type_;
    int	count_;		//�o���t���[����
    int renderMode_;	//�`����@
    int repertDraw_;	//�d�˂��
    float	animCnt_;
    float	animFrm_;			//�A�j�����x �؂�ւ��t���[��
    int		animMax_;			//�A�j���ő吔
    float	addAngle_;			//�p�x���Z���x
    float alpha_;


    bool count();
    bool angle();
    bool anime();
    bool alpha();
    bool move();
    bool scaling();
    void createBomb(float x,float y,float z);
    bool updateBomb();
    void createHouki(float x, float y, float z);
    bool updateHouki();

public:   
  typedef void(Effect3D::*create)(float x, float y, float z);
  typedef bool(Effect3D::*step)();

  struct EffectFunc{
      create	create;
      step	  step;
    }eFunc;

  	void LoadReses();
	  void Init();
    
    Effect3D(float x, float y, float z, effect3D::EFFECTTYPE3D type);
    Effect3D();

    void update() override;
    void render() override;
  };
}


