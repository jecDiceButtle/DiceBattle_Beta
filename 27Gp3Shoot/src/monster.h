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
	//�@�萔
	//*************************************************//
private:

	static const float OFFSETY;



	//*************************************************//
	//�@�ϐ�
	//*************************************************//
private:
	int monster_num;			//�����X�^�[�̃i���o�[(�ύX�Ń��f���ύX)
	ci_ext::Vec3f m_pos;		//�����X�^�[��pos(���W)
	ci_ext::Vec3f m_angle;		//�����X�^�[�̌���

	float angle;
	float count;				//�����X�^�[�㉺�A�j���H�̃J�E���g
	bool moveflag;				//�����X�^�[�㉺�A�j���H�̃t���O
	//*************************************************//
	//�@�֐�
	//*************************************************//
private:


public:



	//monsternum=0(KUJIRA)monsternum=1(KINOKO)monsternum=2(TORI)
	Monster(const std::string& objectName, const ci_ext::Vec3f pos, const int monsternum, const ci_ext::Vec3f angle);



	void render() override;


	void update() override;


	void monster_move(ci_ext::Vec3f p,ci_ext::Vec3f a);//�|�C���^�g���č��W�ړ�&�A���O��
	void monster_move(ci_ext::Vec3f p);

	void updownmove();

};

}