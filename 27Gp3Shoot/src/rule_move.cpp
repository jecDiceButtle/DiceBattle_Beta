/*
*	�t�@�C����	�F	rule_move.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/25
*	���e		�F	�_�C�X�̈ړ����d�؂�I�u�W�F�N�g�B
*					�Ƃ肠�����L�[���͂��󂯕t���Ă��邪�A�_�C�X�Ɏ������邩�͖���B
*					��{�I�Ƀ��C���t�F�C�Y�̂ݑ��݂���B
*					���[���I�u�W�F�N�g�Ƀf�[�^�����m�F�B
*					�m�F��A���ߕ���e�ɑ���B
*/
#pragma once
#include "rule_move.h"
#include "rule.h"
#include "phase_main.h"


namespace game
{

	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	void RuleMove::key()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_LEFT))
		{
			moveDice("west");
		}
		if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
		{
			moveDice("east");
		}
		if (gplib::input::CheckPush(gplib::input::KEY_UP))
		{
			moveDice("north");
		}
		if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
		{
			moveDice("south");
		}
	}

	void RuleMove::moveDice(const std::string& dir)
	{
		//=============================
		// �_�C�X���ړ��ł��邩�m�F
		//=============================
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//�_�C�X�̃}�X���W���璲�ׂ�}�X���W�����߂�
		ci_ext::Vec3i pos = rule->getDiceMasu();
		pos += rule->getDir(dir);

		//�ړ��ł��Ȃ��{�[�h
		if (rule->getBoardState(pos) == -1)
			return;
		//��Q���̊m�F
		if (rule->getExistObj(pos) != 0)
			return;

		//=============================
		// �_�C�X�̈ړ�
		//=============================

		//�_�C�X�}�X���W���X�V
		rule->updateMasu(pos);

		//�_�C�X�ړ�
		auto parent = ci_ext::weak_to_shared<game::PhaseMain>(getParentPtr());
		parent->moveDiceAnim(dir);

		//�A�j���[�V�������̓L�[���͂��󂯕t���Ȃ�
		this->pause();
	}

	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//

	RuleMove::RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule)
	{}


	//�t���[������
	void RuleMove::update()
	{
		key();
	}


}

