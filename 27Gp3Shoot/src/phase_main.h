/*
*	�t�@�C����	�F	phase_main.h
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
		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^


		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:

		
	public:
		/*
			@brief					�_�C�X�̈ړ��A�j���[�V����
			@param[in] masu			�ړ���̃}�X���W
			@param[in] pParent		�e�̃|�C���^
		*/
		void moveDiceAnim(const std::string& masu);

		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] pParent		�e�̃|�C���^
		*/
		PhaseMain(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);
		/*
			@brief					�q�̏�����
		*/
		void init() override;
		
		/*
			@brief					�t���[������
		*/
		void update() override;


	};
}

