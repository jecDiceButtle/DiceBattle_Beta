/*
*	�t�@�C����	�F	rule.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/27
*	���e		�F	�Q�[���i�s���̏����S�ʁB�S�Ă̏����̊j�ƂȂ镔���B
*					�t�F�[�Y��v���C���[�f�[�^�Ȃǂ̊�{�f�[�^�����B
*					�_�C�X�Ȃǂ̃I�u�W�F�N�g�������B�e�L�X�g�Ŗ��߂𑗂�B
*					�Q�[���i�s�̃t�F�[�Y���Ǘ��B��������Ύ����I�ɏ�������B
*/


#include "rule.h"
#include "phase_main.h"
#include <memory>
#include <string>

#include "camera.h"
#include "back.h"
#include "dice.h"
#include "board.h"

#include <stdexcept>

namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	//**************************************************************************************//
	//�R���X�g���N�^
	//**************************************************************************************//

	Rule::Rule(const std::string& objectName)
		:
		Object(objectName),
		phase_(SUMMON),
		turn_(PLAYER_A),
		dir_({
			{ "west", ci_ext::Vec3i(-1, 0, 0) },
			{ "east", ci_ext::Vec3i(1, 0, 0) },
			{ "north", ci_ext::Vec3i(0, 0, 1) },
			{ "south", ci_ext::Vec3i(0, 0, -1) },
		})
	{
		diceno_ = 0;


		//++++++++++++++++++++++++++++++++++++++++++++
		//	�{�[�h�̏�ԁi���z�u,-1�����j
		board_ = {
			{ 0, 0, 0, 0 },
			{ 0, 0, -1, 0 },
			{ 0, 0, 0, 0 },
			{ -1, 0, 0, 0 }
		};
		//++++++++++++++++++++++++++++++++++++++++++++

	}
	void Rule::init()
	{
		//++++++++++++++++++++++++++++++++++++++++++++
		//	�_�C�X�̊J�n�}�X���W�i�}�W�b�N�i���o�[�j
		const ci_ext::Vec3i STARTMASU[2][2] =
		{
			{ ci_ext::Vec3i(1, 0, 0), ci_ext::Vec3i(3, 0, 0) },
			{ ci_ext::Vec3i(1, 0, 4), ci_ext::Vec3i(3, 0, 4) }
		};
		//++++++++++++++++++++++++++++++++++++++++++++

		//�v���C���[�f�[�^��l��
		for (int i = 0; i < 2; i++){
			for (int j = 0; j < 2; j++){

				DiceData dice;
				dice.masu = STARTMASU[i][j];
				dice.p_dice = insertAsChild(new game::Dice("dice", gplib::math::GetRandom<int>(0, 2), i, dice.masu));
				dice.show_ = true;

				//�f�[�^�ǉ�
				dice_[makeKeyword(i, j)] = dice;
			}
		}

		//�I�u�W�F�N�g�̒ǉ�

		p_board = insertAsChild(new Board("board",board_));		//�{�[�h
		p_camera = insertAsChild(new Camera("camera"));		//�J����

		insertAsChild(new Back("stageback", "TitleBack"));	//�w�i

		//�J�b�g�C���ǉ��\��


		//�t�F�[�Y�I�u�W�F�N�g�ǉ�
		//insertAsChild(new PhaseSummon("phase_summon", this->selfPtr()));
	}

	//**************************************************************************************//
	//�@�֐��L�q
	//**************************************************************************************//

	//�L�[���[�h�쐬
	std::string Rule::makeKeyword(const int player, const int dice)
	{
		//map�z��̃L�[
		std::string key = "";

		//�v���C���[
		if (player == 0){
			key += "PLAYER:A_";
		}
		else if (player == 1){
			key += "PLAYER:B_";
		}
		key += "DICE:" + std::to_string(dice);
		return key;
	}


	ci_ext::Vec3i Rule::getDir(const std::string& dir) const
	{
		return dir_.at(dir);
	}
	ci_ext::Vec3i Rule::getDiceMasu()
	{
		return getDiceMasu(makeKeyword((int)turn_, diceno_));
	}
	ci_ext::Vec3i Rule::getDiceMasu(const std::string& key)
	{
		ci_ext::Vec3i masu;
		try{
			//�I������Ă���_�C�X�̍��W��Ԃ�
			 masu = dice_.at(key).masu;
		}
		catch (const std::out_of_range&){
			//error
			return ci_ext::Vec3i(-1, -1, -1);
		}
		return masu;
	}

	void Rule::updateMasu(const ci_ext::Vec3i& pos)
	{
		//���ݑI������Ă���_�C�X
		updateMasu(pos, makeKeyword((int)turn_, diceno_));
	}
	void Rule::updateMasu(const ci_ext::Vec3i& pos, const std::string& key)
	{
		try{
			//�I������Ă���_�C�X�̍��W��Ԃ�
			dice_.at(key).masu = pos;
		}
		catch (const std::out_of_range&){
			//error
		}
		
	}

	void Rule::sendMsg(const std::string& msg, const std::string& process)
	{
		//=====================================
		//	���M������߂�
		//=====================================
		std::weak_ptr<ci_ext::Object> receiver;
		
		//process�𕪊������
		auto msgVec = gplib::text::split(process, ",");
		for (auto ms : msgVec){

			//�ړ��̂Ƃ�
			if (ms == "movedice")
			{
				//���ݑI�𒆂̃_�C�X�ɑ��M����
				receiver = dice_.at(msgVec[1]).p_dice;
				break;
			}
			//�I���̂Ƃ�
			if (ms == "selectdice")
			{
				//���ݑI�𒆂̃_�C�X�ɑ��M����
				receiver = dice_.at(msgVec[1]).p_dice;
				break;
			}
			//�����ꂽ��
			if(ms == "pushdice")
			{
				receiver = dice_.at(msgVec[1]).p_dice;
				break;
			}

			//��������

		}

		//=====================================
		//	���M
		//=====================================
		postMsg(receiver, msg);
	}
	
	int Rule::getExistObj(const ci_ext::Vec3i pos)
	{

		//�_�C�X�����邩�m�F����
		for (auto d : dice_){

			//�_�C�X����ʂɂ���ꍇ�̂݌���
			if (!d.second.show_)
				continue;
			//�_�C�X�������ꍇ
			if (d.second.masu == pos){
				return 1;
			}
		}

		//�����Ȃ�����
		return 0;
	}

	std::string Rule::getDiceKeyword()
	{
		//�I������Ă���ꏊ�̃_�C�X�̃L�[���[�h��Ԃ�
		return getDiceKeyword(getDiceMasu());
	}

	std::string Rule::getDiceKeyword(const ci_ext::Vec3i pos)
	{
		//�_�C�X�����邩�m�F����
		for (auto d : dice_){

			//�_�C�X����ʂɂ���ꍇ�̂݌���
			if (!d.second.show_)
				continue;

			//�_�C�X�������ꍇ
			if (d.second.masu == pos){
				return d.first;
			}
		}
		return "";
	}

	int Rule::getBoardState(const ci_ext::Vec3i pos)
	{
		try{
			return board_.at(pos.x()).at(pos.z());
		}
		catch(const std::out_of_range&) {
			
			//�͈͊O���͂���
			return -1;

		}
	}

	

	void Rule::NextPhase()
	{
		//++++++++++++++++++++++++++++++++++++++++++++
		//	�J�b�g�C���҂����ԁi�}�W�b�N�i���o�[)
		int wait = 120;
		//++++++++++++++++++++++++++++++++++++++++++++


		//���ݓ��삵�Ă�t�F�[�Y�͍폜
		//�t�F�[�Y�I�u�W�F�N�g���ł��폜�����邪�ꉞ
		auto objs = getObjectsFromChildren({ "phase" });
		for (auto obj : objs){
			obj.lock()->kill();
		}
		
		//�t�F�[�Y�̕ύX�ƁA���̃t�F�[�Y�̓ǂݍ���
		switch (phase_)
		{
		case game::Rule::Phase::SUMMON:
			phase_ = Phase::MAIN;
			insertAsChildSleep(new PhaseMain("phase_main", this->selfPtr()),wait);
			break;

		case game::Rule::Phase::MAIN:
			phase_ = Phase::BATTLE;
			//insertAsChild(new PhaseBattle("phase_battle", this->selfPtr()));
			break;

		case game::Rule::Phase::BATTLE:
			phase_ = Phase::SUMMON;
			//insertAsChild(new PhaseSummon("phase_summon", this->selfPtr()));

			//���̃v���C���[�^�[����
			turn_ = ((turn_ == Turn::PLAYER_A) ? Turn::PLAYER_B : Turn::PLAYER_A);
			break;
		}

	}

	//**************************************************************************************//
	//�@�I�[�o�[���C�h�֐�
	//**************************************************************************************//

	void Rule::render()
	{
		//�f�o�b�O�p
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		//�^�[���\��
		std::string str = "";
		switch (turn_)
		{
		case game::Rule::Turn::PLAYER_A:
			str = "�v���C���[�P";
			break;
		case game::Rule::Turn::PLAYER_B:
			str = "�v���C���[�Q";
			break;
		}
		gplib::font::Draw_FontTextNC(0, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);

		//�t�F�[�Y�\��
		switch (phase_)
		{
		case game::Rule::Phase::SUMMON:
			str = ":�����t�F�[�Y";
			break;
		case game::Rule::Phase::MAIN:
			str = ":���C���t�F�[�Y";
			break;
		case game::Rule::Phase::BATTLE:
			str = ":�o�g���t�F�[�Y";
			break;
		}
		gplib::font::Draw_FontTextNC(120, 20, 0.f, str, ARGB(255, 0, 0, 0), 0);

		gplib::graph::Draw_2DRefresh();
#endif

	}
	void Rule::update()
	{
		//���z�u
#ifdef _DEBUG
		if (gplib::input::CheckPush(gplib::input::KEY_BTN0)){
			NextPhase();
		}
#endif
	}
}


