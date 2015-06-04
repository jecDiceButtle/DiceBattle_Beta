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

	void PhaseBattle::moveDiceAnim(const std::string& masu)
	{
		state_ = ANIM;

		//�I���̕ύX��s��
		//pauseFromChildren("rule_selelct");

		//�A�j���[�V����������
		insertAsChild(new Animator("animator_movedice", p_rule, Animator::DICEMOVE, masu));
	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(WAIT)
	{}

	void PhaseBattle::init()
	{
		//�ړ��n�I�u�W�F�N�g
		insertAsChildPause(new game::RuleMove("rule_move", p_rule));

		//�I���I�u�W�F�N�g
	}
	//�t���[������
	void PhaseBattle::update()
	{
		switch (state_)
		{
		case game::PhaseBattle::WAIT:
			break;
		case game::PhaseBattle::ANIM:

			//�A�j���[�V�����I��������
			auto obj = ci_ext::weak_to_shared<Animator>(getObjectFromChildren("animator_movedice"));

			if (obj->isfinish()){

				//�A�j���[�V�����I�u�W�F�N�g��~
				obj->kill();

				//���͂��󂯕t����
				runFromChildrens({ "rule" });
				state_ = WAIT;
			}


			break;
		}
	}

	void PhaseBattle::resume()
	{
		runAll();
	}
	
}

