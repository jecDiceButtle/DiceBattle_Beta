#include "monster.h"
#include "../../lib/gplib.h"

//tuika
#include "movableObject.h"

//3D�J����
extern LPDIRECT3DDEVICE9 pD3DDevice;
//���b�V���S�̊Ǘ��N���X
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;


namespace game
{
	using namespace ci_ext;
	using namespace gplib;


	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	const float Monster::OFFSETY = 5.f;


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	Monster::Monster(const std::string& objectName, const Vec3f pos, const int monsternum, const Vec3f angle)
		:
		MovableObject(
		DrawObjf(objectName)
		),
		m_pos(pos),
		monster_num(monsternum),
		m_angle(angle)
	{
		m_pos.offset(0.f, OFFSETY, 0.f);
		count = 0;
		moveflag = true;
	}

	void Monster::render()
	{
		//m_angle = Vec3f(0.f, 0.f, 0.f);
		Vec3f scale(10.f, 10.f, 10.f);

		

		switch (monster_num){
			//�O�[�A��
		case 0:
			meshManage->drawMesh(m_pos, "tori", m_angle, ARGB(255, 200, 200, 200), scale);
			break;
			//�`���L�A��
		case 1:
			meshManage->drawMesh(m_pos, "kinoko", m_angle, ARGB(255, 200, 200, 200), scale);
			break;
			//�p�[�A��
		case 2:
			meshManage->drawMesh(m_pos, "kuzira", m_angle, ARGB(255, 200, 200, 200), scale);
			break;
		}
	}

	void Monster::update()
	{
		updownmove();

	}

	void Monster::monster_move(Vec3f p, Vec3f a){

			m_pos = p;
			m_angle = a;
	}

	void Monster::monster_move(Vec3f p){

		m_pos.x(p.x());
		m_pos.z(p.z());

	}
	

	void Monster::updownmove(){
		angle += D3DXToRadian(1.f);
		Vec3f f = Vec3f(0.f, ((float)sin(angle))/40.f, 0.f);
		m_pos += f;
	}
}