/*
*	�t�@�C����	�F	phase_battle.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/06/05
*	���e		�F	�o�g���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*					�E�o�g���v�Z
*					�E�A�j���[�V����
*/
#pragma once
#include "phase_battle.h"
#include "rule.h"
#include "rule_move.h"
#include "animator.hpp"


namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	void PhaseBattle::battleCalc(const std::string& dir, const ci_ext::Vec3i masu, const int no)
	{
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//�w�肳��Ă�����p�̃x�N�g�����擾
		auto vec = rule->getDir(dir);

		//�m�F����}�X�̌v�Z
		auto checkmasu = masu + vec;

		//�m�F����}�X�̈ʒu�̃_�C�X�L�[���[�h���擾
		auto key = rule->getDiceKeyword(checkmasu);

		//�_�C�X���Ȃ������ꍇ
		if (key.empty()){

			return;
		}
		//�_�C�X���������ꍇ
		else{

			//���񎞂����A�G�_�C�X�̂Ƃ��݂̂Ƃ���B
			if (no == 0){
				if (!rule->checkEnemyDice(rule->getDiceKeyword(masu), key))
					return;
			}

			//�ړ�����_�C�X�̃f�[�^��ۑ�
			movedice_.push_back(Data(dir, key));

			//�ċA�����ł������̃}�X���m�F
			battleCalc(dir, checkmasu, no + 1);

		}
	}

	void PhaseBattle::calc()
	{

		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//���ݑI������Ă���_�C�X�̃}�X���W
		ci_ext::Vec3i pos = rule->getDiceMasu();

		//���������ی�
		movedice_.clear();

		//���[���ɍ��E�㉺�̃}�X�ɓG�̃_�C�X�����邩�ǂ��������ɍs���ċA�������s���B
		//�㉺���E�̌����ƁA�_�C�X�̃L�[���[�h��ۑ�����B
		battleCalc("west", pos, 0);
		battleCalc("east", pos, 0);
		battleCalc("north", pos, 0);
		battleCalc("south", pos, 0);

		//�o�g���������Ȃ������ꍇ�͏I��
		if (movedice_.empty()){
			state_ = END;
		}
		//���������ꍇ
		else
		{
			initPushAnim();
		}

	}
	void PhaseBattle::initPushAnim()
	{
		//==============================
		//	�ړ��̏�����
		//==============================

		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		//�ړ�����_�C�X�S��
		for (auto dice : movedice_){

			//�ړ���̃}�X�̎Z�o
			auto vec = rule->getDir(dice.dir_);				//�ړ���
			auto masu = rule->getDiceMasu(dice.key_);		//���݂̍��W
			auto pos = masu + vec;							//�ړ���

			//���W�̍X�V
			rule->updateMasu(pos, dice.key_);

			if (rule->getBoardState(pos) < 0)
			{
			
				rule->setDiceShow(false, dice.key_);
			}

			//-----------------------
			//	�ړ�����
			//-----------------------

			//�ړ���̃}�X�̖��ߕ�"x=X�}�X���W,z=Z�}�X���W"
			std::string str = "x=" + std::to_string(pos.x()) + ",z=" + std::to_string(pos.z());

			//�_�C�X�ɑ��郁�b�Z�[�W
			//"push,x���W,z���W,�ړ��t���[����"
			std::string msg = "push," + str + ",frame=" + std::to_string(PUSHSPEED);

			//���[������ă_�C�X�Ƀ��b�Z�[�W�𑗂�(str��Rule���f�p)
			std::string process = "movedice," + dice.key_;
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, process);

		}

		//�A�j���[�V�����̎��s
		p_anim = insertAsChild(new Animator("animator_pushdice", p_rule, Animator::SKIPON, PUSHSPEED));
		//��Ԃ�ύX
		state_ = PUSHANIM;

	}
	void PhaseBattle::fallAnim()
	{
		//�_�C�X�̃L�[���[�h�ƃ}�b�v���m�F���āA�����Ă���悤�Ȃ珈���B
		auto rule = ci_ext::weak_to_shared<Rule>(p_rule);

		for (auto dice : movedice_){

			////�ړ���̃}�X�̎Z�o
			//auto vec = rule->getDir(dice.dir_);				//�ړ���
			//auto masu = rule->getDiceMasu(dice.key_);		//���݂̍��W
			//auto pos = masu + vec;							//�ړ���

			//���W�̍X�V
			/*rule->updateMasu(pos, dice.key_);*/

			if (!(rule->getDiceShow(dice.key_)))
			{
				//�_�C�X�ɑ��郁�b�Z�[�W
				//"push,x���W,z���W,�ړ��t���[����"
				std::string msg = "fall,frame=" + std::to_string(FALLSPEED);;

				//���[������ă_�C�X�Ƀ��b�Z�[�W�𑗂�(str��Rule���f�p)
				std::string process = "falldice," + dice.key_;
				ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, process);
			}


		}
		//�A�j���[�V�����̎��s
		p_anim = insertAsChild(new Animator("animator_pushdice", p_rule, Animator::SKIPON, FALLSPEED));
		//��Ԃ�ύX
		
	}

	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
		Object(objectName),
		p_rule(prule),
		state_(CALC),
		PUSHSPEED(12)
	{
		calc();
	}

	void PhaseBattle::init()
	{

	}
	//�t���[������
	void PhaseBattle::update()
	{
		//tuika
		std::string msg;

		switch (state_)
		{
		case game::PhaseBattle::CALC:

			break;
		case game::PhaseBattle::PUSHANIM:
			
			//tuika
			if (ci_ext::weak_to_shared<Animator>(p_anim)->isfinish()){

				//�A�j���[�V�����I�u�W�F�N�g��~
				ci_ext::weak_to_shared<Animator>(p_anim)->kill();

				state_ = FALLANIM;
				fallAnim();

			}
				
			

			break;
		case game::PhaseBattle::FALLANIM:
			if (ci_ext::weak_to_shared<Animator>(p_anim)->isfinish()){

				//�A�j���[�V�����I�u�W�F�N�g��~
				ci_ext::weak_to_shared<Animator>(p_anim)->kill();

				state_ = END;

			}

			break;
		case game::PhaseBattle::END:
			std::string msg = "end,";
			ci_ext::weak_to_shared<Rule>(p_rule)->sendMsg(msg, "enddice," + ci_ext::weak_to_shared<Rule>(p_rule)->getDiceKeyword());

			ci_ext::weak_to_shared<Rule>(p_rule)->NextPhase();
			break;

		}
	}

	void PhaseBattle::resume()
	{
		runAll();
	}

}




///*
//*	�t�@�C����	�F	phase_battle.cpp
//*	�����		�F	�ێR�m��Y
//*	�����		�F	2015/06/05
//*	���e		�F	�o�g���t�F�C�Y���s�����̃I�u�W�F�N�g�B
//*					�y���ݓ��삳���Ă�����́z
//*					�E�o�g���v�Z
//*					�E�A�j���[�V����
//*/
//#pragma once
//#include "phase_battle.h"
//#include "rule.h"
//#include "rule_move.h"
//#include "animator.hpp"
//
//
//namespace game
//{
//	//**************************************************************************************//
//	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
//	//**************************************************************************************//
//
//
//	//**************************************************************************************//
//	//�֐��L�q
//	//**************************************************************************************//
//
//
//	//**************************************************************************************//
//	//�f�t�H���g�֐�
//	//**************************************************************************************//
//
//
//	PhaseBattle::PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule) :
//		Object(objectName),
//		p_rule(prule),
//		state_(CALC)
//	{}
//
//	void PhaseBattle::init()
//	{
//		//�ړ��n�I�u�W�F�N�g
//		insertAsChildPause(new game::RuleMove("rule_move", p_rule));
//
//		//�I���I�u�W�F�N�g
//	}
//	//�t���[������
//	void PhaseBattle::update()
//	{
//		switch (state_)
//		{
//		case game::PhaseBattle::CALC:
//
//			break;
//
//		case game::PhaseBattle::PUSHANIM:
//
//			break;
//
//		case game::PhaseBattle::FALLANIM:
//			break;
//
//		case game::PhaseBattle::END:
//			break;
//
//		}
//	}
//
//	void PhaseBattle::resume()
//	{
//		runAll();
//	}
//	
//}
//
