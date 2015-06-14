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

		//�A�j���̎��
		enum STATE
		{
			SKIPON,
			SKIPOFF,
		};

		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^
		STATE							state_;			//�A�j���̎��
		int								frame_;			//�t���[����
		bool							stepF_;			//���s���t���O(true�F���s���j


		//*************************************************//
		//�@�֐�
		//*************************************************//

	private:

	public:

		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] prule		���[���|�C���^
			@param[in] state		�A�j���[�V�����̎��
			@param[in] method		�������e
		*/
		Animator(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule, STATE state, const int frame)
			:
			Object(objectName),
			p_rule(prule),
			state_(state),
			stepF_(true),
			frame_(frame)
		{

		}
		
		/*
			@brief		�A�j���[�V�����I���m�F
		*/
		bool isfinish()
		{
			return !stepF_;
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
				
			}


			//��Ԃɍ��킹������
			switch (state_)
			{
			case game::Animator::SKIPON:

				//�����I��
				if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
				{
					stepF_ = false;
				}
				break;

			case game::Animator::SKIPOFF:

				break;
			}
		}

	};
}

