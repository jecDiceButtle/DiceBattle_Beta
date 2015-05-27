/*
*	�t�@�C����	�F	rule_move.hpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/25
*	���e		�F	�_�C�X�̈ړ����d�؂�I�u�W�F�N�g�B
*					�Ƃ肠�����L�[���͂��󂯕t���Ă��邪�A�_�C�X�Ɏ������邩�͖���B
*					��{�I�Ƀ��C���t�F�C�Y�̂ݑ��݂���B
*					���[���I�u�W�F�N�g�Ƀf�[�^�����m�F�B
*					�m�F��A���ߕ���e�ɑ���B
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"
#include "rule.h"
#include "phase_main.hpp"

namespace game
{
	class RuleMove : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	private:


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;				//���[���̃|�C���^

		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:
		
		/*
			@brief		�L�[���͊m�F����
		*/
		void key()
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

		void moveDice(const ci_ext::Vec3i& dir)
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

			auto parent = ci_ext::weak_to_shared<PhaseMain>(getParentPtr());
			//�ړ���̌����������ɂ��Ă���i�}�X���W�ɂȂ�\������j
			parent->moveDiceAnim(dir);

			//�A�j���[�V�������̓L�[���͂��󂯕t���Ȃ�
			this->pause();
		}

		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] prule		���[���I�u�W�F�N�g�̃|�C���^

		*/
	public:
		RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
			Object(objectName),
			p_rule(prule)
		{}

		//�t���[������
		void update() override
		{
			key();
		}


	};
}

