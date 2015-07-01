#include "player.h"
#include "../../lib/gplib.h"
#include "stage.h"

namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	const int Player::DEFAULTCOST = 3;

	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	bool Player::isCostRemain(const int id)
	{
		return cost[id] > 0;
	}

	void Player::CostDecrease(const int id, const int point)
	{
		cost[id] -= point;
	}

	void Player::CostReset(const int id)
	{
		cost[id] = DEFAULTCOST;
	}

	//void Player::VictryorDefeat(const int WinLose1P)		//1P�̏������������炤
	//{
	//	if (WinLose1P == 0)		//1P�������̎� 5��U��
	//	{
	//		LIFE_2P_W -= 60;
	//	}
	//	if (WinLose1P == 1)		//1P�������̎� 5��U��
	//	{
	//		LIFE_1P_W += 60;
	//		posx += 72;
	//		srcW -= 60;
	//	}
	//}


	Player::Player(const std::string& objectName, const std::weak_ptr<Object> ptr)
		:
		Object(objectName),
		p_parent(ptr)
	{
		for (int i = 0; i < 2; i++)
		{
			cost.push_back(DEFAULTCOST);
		}
		//cost�摜�̓ǂݍ���
		gplib::graph::Draw_LoadObject("cost", "res/gra/cost.png");
		gplib::graph::Draw_LoadObject("cost_back", "res/gra/cost_back.png");
		//cost�̈ʒu��������
		pos_1P.x(300);
		pos_1P.y(200);

		pos_2P.x(1000);
		pos_2P.y(200);


		gplib::graph::Draw_LoadObject("lifebar_frame", "res/gra/lifebar_frame.png");
		gplib::graph::Draw_LoadObject("lifebar_HP", "res/gra/lifebar_HP.png");
		gplib::graph::Draw_LoadObject("1PWIN", "res/gra/win_1P.png");
		gplib::graph::Draw_LoadObject("2PWIN", "res/gra/win_2P.png");

		lifepos_1P.x(200);
		lifepos_1P.y(75);
		lifepos_2P.x(725);
		lifepos_2P.y(75);

		life_posx = 200;
		LIFE_1P_W = 0;
		LIFE_2P_W = 300;

	}

	void Player::render()
	{

		gplib::graph::Draw_2DClear();//tuika
		std::string str = "P1_COST:" + std::to_string(cost[0]) + "  P2_COST:" + std::to_string(cost[1]);

		////�ǉ� cost�`��//////////////////////////////////////////////////////////
		////player1
		//for (int i = 0; i < DEFAULTCOST; i++)
		//{
		//	gplib::graph::Draw_Graphics(
		//		pos_1P.ix() + COST_Space * i, pos_1P.iy(), pos_1P.z(),
		//		"cost_back",
		//		0, 0, COST_W, COST_H, 0, 2.f, 2.f);
		//}
		//for (int i = 0; i < cost[0]; i++)
		//{
		//	gplib::graph::Draw_Graphics(
		//		pos_1P.ix() + COST_Space * i, pos_1P.iy(), pos_1P.z(),
		//		"cost",
		//		0, 0, COST_W, COST_H, 0, 2.f, 2.f);
		//}
		////////////////////////////////////////////////////////////////////////////
		////player2
		//for (int i = 0; i < DEFAULTCOST; i++)
		//{
		//	gplib::graph::Draw_Graphics(
		//		pos_2P.ix() + COST_Space * -i, pos_2P.iy(), pos_2P.z(),
		//		"cost_back",
		//		0, 0, COST_W, COST_H, 0, 2.f, 2.f);
		//}
		//for (int i = 0; i < cost[1]; i++)
		//{
		//	gplib::graph::Draw_Graphics(
		//		pos_2P.ix() + COST_Space * -i, pos_2P.iy(), pos_2P.z(),
		//		"cost",
		//		0, 0, COST_W, COST_H, 0, 2.f, 2.f);
		//}
		/////////////////////////////////////////////////////////////////////////////





		//LIFEBAR�`��////////////////////////////////////////////////////////////////////////
		//1P frame
		gplib::graph::Draw_GraphicsLeftTop(
			lifepos_1P.ix(), lifepos_1P.iy(), lifepos_1P.z(),
			"lifebar_frame",
			0, 0, LIFE_W, LIFE_H, 0, 0, 1.2f, 1.2f);
		//1P HP
		if (LIFE_1P_W <= 300){
			gplib::graph::Draw_GraphicsLeftTop(
				life_posx, lifepos_1P.iy(), lifepos_1P.z(),
				"lifebar_HP",
				LIFE_1P_W, 0, srcW, LIFE_H, 0, 0, 1.2f, 1.2f);
		}
		//if (LIFE_1P_W >= 300)			2P�����̃e���b�v ����͕K�v�Ȃ��������鏈���͏����ĂȂ�
		//{
		//	gplib::graph::Draw_Graphics(
		//		gplib::system::WINW / 2, gplib::system::WINH / 2, 0,
		//		"2PWIN",
		//		0, 0, 500, 200, 0, 1.f, 1.f);

		//}
		//////////////////////////////////////////////////////////////////////////
		//2P frame
		gplib::graph::Draw_GraphicsLeftTop(
			lifepos_2P.ix(), lifepos_2P.iy(), lifepos_2P.z(),
			"lifebar_frame",
			300, 0, LIFE_W, LIFE_H, 0, 0, 1.2f, 1.2f);
		//2P HP
		if (LIFE_2P_W >= 0)
		{
			gplib::graph::Draw_GraphicsLeftTop(
				lifepos_2P.ix(), lifepos_2P.iy(), lifepos_2P.z(),
				"lifebar_HP",
				300, 0, LIFE_2P_W, LIFE_H, 0, 0, 1.2f, 1.2f);
		}
		//if (LIFE_2P_W <= 0)			1P�����̃e���b�v ����͕K�v�Ȃ��������鏈���͏����ĂȂ�
		//{
		//	gplib::graph::Draw_Graphics(
		//		gplib::system::WINW / 2, gplib::system::WINH / 2, 0,
		//		"1PWIN",
		//		0, 0, 500, 200, 0, 1.f, 1.f);

		//}


		gplib::graph::Draw_2DRefresh();//tuika
#if _DEBUG
		gplib::font::Draw_FontTextNC(100, 150, 0.f, str, ARGB(255, 0, 0, 0), 0);
#endif

	}

	void Player::update()
	{

#if _DEBUG
		if (gplib::input::CheckPush(gplib::input::KEY_F1))
		{
			LIFE_1P_W += 60;
			life_posx += 72;
			srcW -= 60;

		}
		if (gplib::input::CheckPush(gplib::input::KEY_F2))
		{
			LIFE_2P_W -= 60;
		}
#endif

	}

	void Player::receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg)
	{

		//���b�Z�[�W����
		auto msgVec = gplib::text::split(msg, ",");

		for (auto ms : msgVec){
			//����ɕ���
			

			if (ms == "player")
			{
				if (turn_ != std::stoi(ms))
				{
					turn_ = std::stoi(ms);
					CostReset(turn_);
					
				}
				
			}
			//�I������

			if (ms == "player1")
				{
					LIFE_1P_W += 60;
					life_posx += 72;
					srcW -= 60;

				}
			else if (ms == "player2")
				{
					LIFE_2P_W -= 60;
				}

			
		}
		//if (msg == "�@")				//�ǉ��@�ߊ} 1P�̏��������̃��b�Z�[�W���~�����ł�
		//{
		//	VictryorDefeat(num);
		//}

	}

}