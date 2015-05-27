/*
*	�t�@�C����	�F	rule.cpp
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/05/27
*	���e		�F	�Q�[���i�s���̏����S�ʁB
*					�S�Ă̏����̊j�ƂȂ镔���B
*					�_�C�X�Ȃǂ̃I�u�W�F�N�g�������B
*					�Q�[���i�s�̃t�F�[�Y���Ǘ��B
*/


#include "rule.h"
#include <memory>
#include <string>

namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//
	
	//
	ci_ext::Vec3i Rule::getDiceMasu()
	{
		//�I������Ă���_�C�X�̍��W��Ԃ�
		return player_[(int)turn_].dice_[diceno_].masu;
	}

	void Rule::sendMsg(const std::string& msg, const std::string& process)
	{
		//���M������߂�
		std::weak_ptr<ci_ext::Object> receiver;
		
		//process�����b�Z�[�W���������
		auto msgVec = gplib::text::split(msg, ",");
		for (auto ms : msgVec){

			//=====================================
			//	�������ƒǉ����Ă���
			//=====================================

			//�ړ��̂Ƃ�
			if (ms == "movedice")
			{
				//���ݑI�𒆂̃_�C�X
				receiver = player_[(int)turn_].dice_[diceno_].p_dice;
			}
		}

		//���M
		postMsg(receiver, msg);
	}
	
	int Rule::getExistObj(ci_ext::Vec3i pos)
	{

		//�_�C�X�����邩�m�F����
		for (auto p : player_){
			for (auto dPos : p.dice_){
				
				//�_�C�X����ʂɂ���ꍇ�̂݌���
				if (dPos.show_)
					continue;
				//�_�C�X�������ꍇ
				if (dPos.masu == pos){
					return 1;
				}
			}
		}

		//�����Ȃ�����
		return 0;
	}

	int Rule::getBoardState(ci_ext::Vec3i pos)
	{
		//�͈͊O���͂���
		if (board_.size() < pos.x() || board_[0].size() < pos.z())
			return -1;

		return board_[pos.x()][pos.z()];
	}

	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//

	Rule::Rule(const std::string& objectName)
		:
		Object(objectName)
	{

	}
	void Rule::init()
	{

	}
	void Rule::render()
	{
		//�f�o�b�O�p
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();

		gplib::graph::Draw_2DRefresh();
#endif

	}
	void Rule::update()
	{

	}
}


