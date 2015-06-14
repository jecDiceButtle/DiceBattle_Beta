/*
*	�t�@�C����	�F	phase_battle.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/06/05
*	���e		�F	�o�g���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*					�E�o�g���v�Z
*					�E�A�j���[�V����
*/
#pragma once
#include "phase_battle.h"
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

	void PhaseBattle::calc()
	{
		//���[���Ɍ��ݑI�����ꂽ�_�C�X�̏ꏊ�𕷂�

		//���[���ɍ��E�㉺�̃}�X�ɓG�̃_�C�X�����邩�ǂ��������ɍs��

		//���Ȃ������ꍇ�̓o�g���I��


	}
	void PhaseBattle::pushAnim()
	{

	}
	void PhaseBattle::fallAnim()
	{

	}

	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(CALC)
	{}

	void PhaseBattle::init()
	{

	}
	//�t���[������
	void PhaseBattle::update()
	{
		switch (state_)
		{
		case game::PhaseBattle::CALC:
			
			break;
		case game::PhaseBattle::PUSHANIM:

			break;

		case game::PhaseBattle::FALLANIM:
			break;

		case game::PhaseBattle::END:
			break;

		}
	}

	void PhaseBattle::resume()
	{
		runAll();
	}
	
}

