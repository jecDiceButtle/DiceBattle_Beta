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

	void PhaseMain::moveDiceAnim(const std::string& masu)
	{
		state_ = ANIM;

		//�I���̕ύX��s��
		pauseFromChildren("rule_selelct");

		//�A�j���[�V����������
		insertAsChild(new Animator("animator_movedice", p_rule, Animator::DICEMOVE, masu));
	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseMain::PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT)
	{}

	void PhaseMain::init()
	{
		//�ړ��n�I�u�W�F�N�g
		insertAsChild(new game::RuleMove("rule_move", p_rule));

		//�I���I�u�W�F�N�g
	}
	//�t���[������
	void PhaseMain::update()
	{
		switch (state_)
		{
		case game::PhaseMain::WAIT:
			break;
		case game::PhaseMain::ANIM:

			//�A�j���[�V�����I��������
			auto objs = getObjectsFromChildren({ "animator" });
			for (auto obj : objs){

				if (obj.lock()->isDead()){

					//���͂��󂯕t����
					runFromChildren({ "rule" }, {});
					state_ = WAIT;
				}


				break;
			}
		}
	}
}

