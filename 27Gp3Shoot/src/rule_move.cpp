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
			moveDice(ci_ext::Vec3i(-1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
		{
			moveDice(ci_ext::Vec3i(1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_UP))
		{
			moveDice(ci_ext::Vec3i(0, 0, 1));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
		{
			moveDice(ci_ext::Vec3i(0, 0, -1));
		}
	}

	void RuleMove::moveDice(const ci_ext::Vec3i& dir)
	{
		//=============================
		// �_�C�X���ړ��ł��邩�m�F
		//=============================
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//�_�C�X�̃}�X���W���璲�ׂ�}�X���W�����߂�
		ci_ext::Vec3i pos = rule->getDiceMasu();
		pos += dir;

		//�ړ��ł��Ȃ��{�[�h
		if (rule->getBoardState(pos) == -1)
			return;
		//��Q���̊m�F
		if (rule->getExistObj(pos) != 0)
			return;

		//=============================
		// �_�C�X�̈ړ�
		//=============================

		//�ړ���̃}�X�̖��ߕ�"x=X�}�X���W,z=Z�}�X���W"
		std::string masu = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

		//�_�C�X�ړ�
		auto parent = ci_ext::weak_to_shared<game::PhaseMain>(getParentPtr());
		parent->moveDiceAnim(masu);
		//�_�C�X�}�X���W���X�V
		rule->updateMasu(pos);


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

