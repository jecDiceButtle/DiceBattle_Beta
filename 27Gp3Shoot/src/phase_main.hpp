/*
*	�t�@�C����	�F	phase_main.hpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/27
*	���e		�F	���C���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*					�E�ړ�
*					�E�I��
*					�E�A�j���[�V����
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"
#include "rule.h"
#include "rule_move.hpp"

namespace game
{
	class PhaseMain : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		
		enum STATE{
			WAIT,				//�ҋ@
			ANIM,				//�A�j���[�V������

		};

		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_parent;			//�e(���[���I�u�W�F�N�g)�|�C���^


		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:

		
	public:
		/*
			@brief					�_�C�X�̈ړ��A�j���[�V����
			@param[in] dir			�ړ�����
			@param[in] pParent		�e�̃|�C���^
		*/
		void moveDiceAnim(ci_ext::Vec3i dir)
		{
			state_ = ANIM;

			//�I���̕ύX��s��
			pauseFromChildren("rule_selelct");

			//�A�j���[�V����������
			//insertAsChild(new Animation("anim_dice", dir));
		}



		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] pParent		�e�̃|�C���^
		*/
		PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& pParent) :
			Object(objectName),
			p_parent(pParent),
			state_(WAIT)
		{}
		/*
			@brief					�q�̏�����
		*/
		void init() override
		{
			//�ړ��n�I�u�W�F�N�g
			insertAsChild(new RuleMove("rule_move", p_parent));

			//�I���I�u�W�F�N�g
		}
		//�t���[������
		void update() override
		{

		}


	};
}

