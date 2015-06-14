/*
*	�t�@�C����	�F	rule_move.h
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/28
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
		void key();

		/*
			@brief					�_�C�X�̈ړ�
			@param[in] dir			�ړ��������

		*/
		void moveDice(const std::string& dir);

		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] prule		���[���I�u�W�F�N�g�̃|�C���^
		*/
	public:
		RuleMove(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);


		//�t���[������
		void update() override;

		

	};
}

