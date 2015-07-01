/*
*	�t�@�C����	�F	phase_summon.h
*	�����		�F	����
*	�����		�F	2015/06/09
*	���e		�F	���C���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{
	class PhaseSummon : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	private:

		enum STATE{
			WAIT,				//�ҋ@
			ANIM,				//�A�j���[�V������
			END,				//�t�F�[�Y�I��

		};

		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^

		const int DICESUMMONSPEED = 90;		//�_�C�X�̈ړ��t���[��
		int								frame_;			//�t���[����


		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:

		/*
		@brief					�A�j�����̏���
		*/
		/*void anim();*/

		/*
		@brief					�_�C�X�̓_�ŏ���
		@param[in]	clear		True:�_�� False:����
		*/
		//void selectDice(const bool clear);

		/*
		@brief					1frame�v�Z
		*/
		void CalcDiceSummon();

	public:
		/*
		@brief					�_�C�X�̈ړ��A�j���[�V����
		@param[in] masu			�ړ���̃}�X���W
		@param[in] pParent		�e�̃|�C���^
		*/
		/*void moveDiceAnim(const std::string& masu);*/

		/*
		@brief					�R���X�g���N�^
		@param[in] objectName	�I�u�W�F�N�g��
		@param[in] pParent		�e�̃|�C���^
		*/
		PhaseSummon(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);
		/*
		@brief					�q�̏�����
		*/
		void init() override;

		/*
		@brief					�t���[������
		*/
		void update() override;

		/*
		@brief					�ҋ@����̖߂菈��
		*/
		void resume() override;

	};
}

