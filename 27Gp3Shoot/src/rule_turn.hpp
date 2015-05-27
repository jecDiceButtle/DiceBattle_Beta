/*
*	�t�@�C����	�F	rule_turn.hpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/25
*	���e		�F	�t�F�[�Y��^�[�����d�؂�I�u�W�F�N�g�B
*					��{�I�ɃQ�[���i�s���͍Ō�܂ő��݂���B
*					���[���I�u�W�F�N�g�i�ȉ��F�e�j����t�F�[�Y�ύX�����炤�B
*					�ύX��́A�e�ɑ΂��t�F�[�Y�̏�Ԃ𑗂�B
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
//#include "../../lib/gplib.h"

namespace game
{

	class RuleTurn : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	private:

		//�v���C���[�^�[��
		enum TURNSTATE
		{
			null = -1,				//�^�[�����ݒ�
			PLAYER_A,				//�v���C���[�P
			PLAYER_B,				//�v���C���[�Q
		};

		//�^�[���t�F�C�Y
		enum PHASESTATE
		{
			null = -1,				//�^�[�����ݒ�
			SUMMON,					//����(�A���t�@�ۗ��j
			MAIN,					//�ړ�
			BATTLE,					//�ΐ�
			END,					//�i�Ƃ肠�������z�u�j
		};


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^
		PHASESTATE						phase_;			//�t�F�[�Y
		TURNSTATE						turn_;			//�^�[��


		//*************************************************//
		//�@�֐�
		//*************************************************//

		/*
			@brief		�R���X�g���N�^
		*/
	public:
		RuleTurn(const std::string& objectName) : Object(objectName){}

		/*
			@param[in]	start �J�n�v���C���[(0:1P�X�^�[�g ,1:2P�X�^�[�g)
		*/
		RuleTurn(const std::string& objectName, const int start)
			:
			Object(objectName),
			phase_(SUMMON)
		{
			if (start == 1) turn_ = PLAYER_A;
			else			turn_ = PLAYER_B;
							
		}

		//�t���[������
		void update() override
		{

		}

		//���b�Z�[�W����
		void receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg) override
		{
			//���b�Z�[�W��nextphase������Ȃ�t�F�[�Y���ڂ�
			if (msg.find("nextphase")){
				nextphase();
			}
		}

		//���̃t�F�[�Y�ֈړ��̎�t
		void nextphase()
		{
			std::string msg("");
			switch (phase_)
			{
			case game::RuleTurn::PHASESTATE::SUMMON:
				phase_ = PHASESTATE::MAIN;
				msg = "main";
				break;

			case game::RuleTurn::PHASESTATE::MAIN:
				phase_ = PHASESTATE::BATTLE;
				msg = "battle";
				break;

			case game::RuleTurn::PHASESTATE::BATTLE:
				phase_ = PHASESTATE::END;
				msg = "end";
				break;

			case game::RuleTurn::PHASESTATE::END:

				//�^�[���̐؂�ւ�
				turn_ = ((turn_ == TURNSTATE::PLAYER_A) ? TURNSTATE::PLAYER_B : TURNSTATE::PLAYER_A);
				phase_ = PHASESTATE::SUMMON;
				msg = "summon";
				break;
			}
			
			//���[���I�u�W�F�N�g�ɑ΂��؂�ւ���̃t�F�[�Y��`����
			msg = (turn_ == TURNSTATE::PLAYER_A) ? "phase,p1," + msg : "phase,p2," + msg;
			postMsg(p_rule, msg);

		}

	};
}

