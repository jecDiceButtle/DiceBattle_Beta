#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class Rule : public ci_ext::Object
	{

		//*************************************************//
		//�@�\����
		//*************************************************//
	private:

		struct DiceData{
			
			std::weak_ptr<ci_ext::Object> p_dice;	// �_�C�X�̃|�C���^

			//int				no;						// �_�C�X�̃i���o�[
			ci_ext::Vec3i	masu;					// �_�C�X�̃}�X���W
			bool			show_;					// ��ʂɕ\������Ă��邩�ǂ���

			//��Ԉُ�Ƃ��̓R�R�ɒǉ�
			//�\����\���Ȃ񂩂��R�R�ł��������B
		};

		struct PlayerData
		{
			std::vector<DiceData>	dice_;			// �v���C���[�̎��_�C�X
			int						hp_;			// �̗�
		};

		enum Turn{
			PLAYER_A = 0,
			PLAYER_B,
		};

		enum Phase{
			SUMMON,
			MAIN,
			BATTLE,
		};


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		Phase	phase_;								// ���݂̃t�F�C�Y
		Turn	turn_;								// ���݂̃^�[���v���C���[
		int		diceno_;							// �I������Ă���_�C�X�ԍ�


		std::vector<PlayerData>			player_;	// �v���C���[�f�[�^
		std::vector<std::vector<int>>	board_;		// �{�[�h�̏��


		std::weak_ptr<ci_ext::Object> p_player;		// �v���C���[�̃|�C���^
		std::weak_ptr<ci_ext::Object> p_board;		// �{�[�h�̃|�C���^

		std::weak_ptr<ci_ext::Object> p_camera;		// �J�����̃|�C���^
	

		//*************************************************//
		//�@�֐�
		//*************************************************//


	public:

		/*
			@brief			�����̏ꏊ�̃I�u�W�F�N�g���m�F
			@param[in] pos	�m�F����ꏊ
			@return			�I�u�W�F�N�g�̓��e(0:�Ȃ��A1:�_�C�X)
		*/
		int getExistObj(ci_ext::Vec3i pos);


		/*
			@brief			���݂̑I������Ă���_�C�X���W���擾
			@return			�Ȃ�
		*/
		ci_ext::Vec3i getDiceMasu();

		/*
			@brief			�����̏ꏊ�̃{�[�h��Ԃ��m�F
			@param[in] pos	�m�F����ꏊ
			@return			�{�[�h�̏��(-1:�� 0:�ʏ�)
		*/
		int getBoardState(ci_ext::Vec3i pos);


		/*
			@brief				�q�ɑ��M���郁�b�Z�[�W(���ߕ�)�̎�t
								msg�̓��e���q�ɑ��M����B
			@ATTEMTION			msg�̈�ԏ��߂͕K�����얽�߂ɂ��邱��
			@param[in]	msg		���M���閽�ߕ�
			@param[in]	process	���M��̃I�u�W�F�N�g�Ȃǂ̓��e
			@return				�Ȃ�
		*/
		void sendMsg(const std::string& msg, const std::string& process);

		/*
			@brief				�t�F�[�Y�̏�ԕύX
			@return				�Ȃ�
		*/
		void NextPhase();

	
		Rule(const std::string& objectName);
		void init() override;
		void render() override;
		void update() override;
		
	};

}