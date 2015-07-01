/*
*	�t�@�C����	�F	phase_main.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/27
*	���e		�F	���C���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*					�E�ړ�
*					�E�I��
*					�E�A�j���[�V����
*/
#pragma once
#include "phase_main.h"
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

	void PhaseMain::moveDiceAnim(const std::string& dir)
	{
		//==========================
		//	�ړ�����
		//==========================


		//�ړ���̃}�X�̖��ߕ�"x=X�}�X���W,z=Z�}�X���W"
		auto pos = ci_ext::weak_to_shared<Rule>(p_rule)->getDiceMasu();
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//�_�C�X�ɑ��郁�b�Z�[�W
		//"move,x���W,z���W,�ړ��t���[����"
		std::string msg = "move," + masu + ",frame=" + std::to_string(DICEMOVESPEED);


		//���[������ă_�C�X�Ƀ��b�Z�[�W�𑗂�(str��Rule���f�p)
		auto str = "movedice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword();
		ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, str);


		//�A�j���[�V����������
		insertAsChild(new Animator("animator_movedice", p_rule, Animator::SKIPON, DICEMOVESPEED));
		state_ = ANIM;

	}
	void PhaseMain::anim()
	{
		//�A�j���[�V�����I��������
		auto obj = ci_ext::weak_to_shared<Animator>(getObjectFromChildren("animator_movedice"));

		if (obj->isfinish()){

			//�A�j���[�V�����I�u�W�F�N�g��~
			obj->kill();

			//���͂��󂯕t����
			runFromChildrens({ "rule" });
			state_ = END;

			std::string msg;

			msg = "end," ;

			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			/*ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();*/
		}
	}
	void PhaseMain::selectDice(const bool clear)
	{
		std::string msg,str;

		//�I���I�t����
		msg = clear ? "select,on" : "select,off";

		str = "selectdice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword();

		//���b�Z�[�W���M
		ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg,str);
	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseMain::PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT),
		DICEMOVESPEED(12)	//�}�W�b�N�i���o�[
	{}

	void PhaseMain::init()
	{
		//�ړ��n�I�u�W�F�N�g
		insertAsChildPause(new game::RuleMove("rule_move", p_rule));

		//�I���I�u�W�F�N�g
		selectDice(true);
	}
	//�t���[������
	void PhaseMain::update()
	{
		switch (state_)
		{
		case game::PhaseMain::WAIT:
			break;
		case game::PhaseMain::ANIM:

			anim();
			break;
		case game::PhaseMain::END:
			
			selectDice(false);
			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();
			this->kill();
			break;
		}
	}

	PhaseMain::~PhaseMain()
	{
		////�f�o�b�O�p�B
		//selectDice(false);
	}

	void PhaseMain::resume()
	{
		runAll();
	}
}

