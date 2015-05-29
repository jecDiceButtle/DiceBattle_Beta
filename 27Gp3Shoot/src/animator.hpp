/*
*	�t�@�C����	�F	animator.hpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/27
*	���e		�F	�A�j���[�V�������Ǘ�����B
*					�t���[���Ǘ��ƁA�����I����S���B
*					�w��t���[�����ɂȂ�A�����I������ƃI�u�W�F�N�g��kill����
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "animator.hpp"

//#include "../../lib/gplib.h"

namespace game
{

	class Animator : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	public:

		const int DICEMOVESPEED = 40;		//�_�C�X�̈ړ��t���[��

		//�A�j���̎��
		enum STATE
		{
			DICEMOVE,						//�_�C�X�̈ړ�
		};

		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^
		STATE							state_;			//�A�j���̎��
		std::string						method_;		//�����̏ڍ׃f�[�^�AVector�ɂȂ�ꍇ����
		int								frame_;			//�t���[����
		bool							stepF_;			//���s���t���O(true�F���s���j


		//*************************************************//
		//�@�֐�
		//*************************************************//

	private:
		/*
			@brief					�_�C�X�ړ����̏���������
									�_�C�X���ړ������閽�߂𑗂�B
			@param[in] dir			����(method�������)
		*/
		void initMoveDice(const std::string& masu)
		{
			frame_ = DICEMOVESPEED;

			//�_�C�X�ɑ��郁�b�Z�[�W
			//"move,x���W,z���W,�ړ��t���[����"
			std::string msg = "move," + masu + ",frame=" + std::to_string(frame_);

			//���[������ă_�C�X�Ƀ��b�Z�[�W�𑗂�(movedice��Rule�m�F�p)
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "movedice");

		}

		
	public:
		
		/*
			@brief		�A�j���[�V�����I���m�F
		*/
		bool isfinish()
		{
			return !stepF_;
		}


		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] prule		���[���|�C���^
			@param[in] state		�A�j���[�V�����̎��
			@param[in] method		�������e
		*/
		Animator(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule, STATE state, const std::string& method = "")
			:
			Object(objectName),
			p_rule(prule),
			state_(state),
			method_(method),
			stepF_(true)
		{
			switch (state_)
			{
			case STATE::DICEMOVE:
				initMoveDice(method);
				break;
			}
		}

		//�t���[������
		void update() override
		{
			//���s���Ȃ�
			if (stepF_)
			{
				//�A�j���t���[����
				if (--frame_ <= 0)
				{
					stepF_ = false;
				}
				//�����I��
				if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
				{
					stepF_ = false;
				}
			}
		}

	};
}

