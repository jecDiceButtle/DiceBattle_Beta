
#pragma once
#include "phase_summon.h"
#include "rule.h"
#include "rule_move.h"
#include "animator.hpp"


namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	void PhaseSummon::CalcDiceSummon()
	{
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		ci_ext::Vec3i pos = rule->getDiceStartMasu();
	
		bool setFlag = false;//�Z�b�g�\�ȃ}�X���ǂ����̃t���O

		
		if (!((rule->getBoardState(pos) != -1) && (rule->getExistObj(pos) == 0)))
		{
			for (int z = -1; z <= 1; ++z)
			{
				for (int x = -1; x <= 1; ++x)
				{
					ci_ext::Vec3i setpos = { x, 0, z };
					//�{�[�h�̊m�F
					if ((rule->getBoardState(pos + setpos) != -1) && (rule->getExistObj(pos + setpos) == 0))
						pos += setpos;
					break;

				}
			}
		}

		//=============================
		// �_�C�X�̈ړ�
		//=============================

		//�_�C�X�ړ�
		
		rule->updateMasu(pos);

		frame_ = DICESUMMONSPEED;

		//�ړ���̃}�X�̖��ߕ�"x=X�}�X���W,z=Z�}�X���W"
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//�_�C�X�ɑ��郁�b�Z�[�W
		//"move,x���W,z���W,�ړ��t���[����"
		std::string msg = "summon," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword() + masu + ",frame=" + std::to_string(frame_);

		//���[������ă_�C�X�Ƀ��b�Z�[�W�𑗂�(movedice��Rule�m�F�p)
		rule->sendMsg(msg, "summondice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

		
		state_ = ANIM;
	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseSummon::PhaseSummon(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT)
	{}

	void PhaseSummon::init()
	{
		bool summonF = ci_ext::weak_to_shared<Rule>(p_rule)->getDiceShow();

		if (summonF)
		{
			state_ = END;
		}

		else
		{
			ci_ext::weak_to_shared<Rule>(p_rule)->setDiceShow(true);
			CalcDiceSummon();
		}
	}
	//�t���[������
	void PhaseSummon::update()
	{
		switch (state_)
		{
		case game::PhaseSummon::WAIT:
			break;
		case game::PhaseSummon::ANIM:
			if (--frame_ <= 0)
			{
				state_ = END;
				break;
			}

			if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
			{
				state_ = END;
			}

			break;
		case game::PhaseSummon::END:
			std::string msg = "end," ;
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();

			kill();
			break;
		}
	}

	void PhaseSummon::resume()
	{
		runAll();
	}

}

