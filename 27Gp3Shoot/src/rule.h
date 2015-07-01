#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class Rule : public ci_ext::Object
	{
		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		const std::map<std::string, ci_ext::Vec3i> dir_;	//�_�C�X�̈ړ������ƃx�N�g��


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

		std::map<std::string, DiceData>	dice_;		// �_�C�X�f�[�^
		std::vector<std::vector<int>>	board_;		// �{�[�h�̏��


		std::weak_ptr<ci_ext::Object> p_player;		// �v���C���[�̃|�C���^
		std::weak_ptr<ci_ext::Object> p_board;		// �{�[�h�̃|�C���^

		std::weak_ptr<ci_ext::Object> p_camera;		// �J�����̃|�C���^


		//*************************************************//
		//�@�֐�
		//*************************************************//


	public:

		Rule(const std::string& objectName);

		/*
		@brief				�G�΂��Ă��邩�ǂ����̊m�F
		@param[in] keyA		�����̃_�C�X�L�[���[�h
		@param[in] keyB		����̃_�C�X�L�[���[�h
		@return				�G�΂��Ă��邩�ǂ���
		@true				�G�΂��Ă���
		@false				�������m�ł���
		*/
		bool checkEnemyDice(const std::string& keyA, const std::string& keyB);


		/*
		@brief				�_�C�X�̃}�X�̍X�V
		@param[in] pos		�X�V��̃}�X
		@param[in] dice		�X�V����_�C�X�̃L�[���[�h
		*/
		void updateMasu(const ci_ext::Vec3i& pos);
		void updateMasu(const ci_ext::Vec3i& pos, const std::string& key);


		/*
		@brief				�_�C�X�f�[�^�̃L�[�쐬
		@param[in] player	�v���C���[No.(0 or 1)
		@param[in] dice		�_�C�XNo.
		@return				�L�[������
		*/

		std::string makeKeyword(const int player, const int dice);


		/*
		@brief			�����̏ꏊ�̃I�u�W�F�N�g���m�F
		@param[in] pos	�m�F����ꏊ
		@return			�I�u�W�F�N�g�̓��e(0:�Ȃ��A1:�_�C�X)
		*/
		int getExistObj(const ci_ext::Vec3i pos);

		/*
		@brief			�����̏ꏊ�̃_�C�X�̃L�[���[�h��Ԃ�
		@param[in] pos	�m�F����ꏊ(�L�[�Ȃ��̏ꍇ�A���ݑI�����ꂽ�_�C�X��Ԃ��j
		@return			�L�[���[�h(�_�C�X���Ȃ��ꍇ�A��f�[�^��Ԃ�)
		*/
		std::string getDiceKeyword();
		std::string getDiceKeyword(const ci_ext::Vec3i pos);


		/*
		@brief			�L�[�̃_�C�X�̃}�X���W���擾
		@param[in] key	�L�[���[�h(���͂Ȃ��̏ꍇ�A���ݑI������Ă���_�C�X�̃}�X�j
		@return			�}�X
		*/
		ci_ext::Vec3i getDiceMasu();
		ci_ext::Vec3i getDiceMasu(const std::string& key);

		/*
		@brief			���p�ɑΉ�����������Ԃ�
		@param			���p( x�� : �����Az���F��k�j
		@return			�����x�N�g��
		*/
		ci_ext::Vec3i getDir(const std::string& dir) const;

		/*
		@brief			�����̏ꏊ�̃{�[�h��Ԃ��m�F
		@param[in] pos	�m�F����ꏊ
		@return			�{�[�h�̏��(-1:�� 0:�ʏ�)
		*/
		int getBoardState(const ci_ext::Vec3i pos);


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



		void init() override;
		void render() override;
		void update() override;


		//(���)�ǉ�
		//
		//�Q�b�^�[�@�_�C�X�X�e�[�g
		bool getDiceShow();
		bool getDiceShow(const std::string& key);
		ci_ext::Vec3i getDiceStartMasu();


		void setDiceShow(bool state, const std::string& key);
		void setDiceShow(bool state);
		//
		/////

	};

}

//
//#pragma once
//#include "../../lib/ci_ext/object.hpp"
//#include "../../lib/ci_ext/vec3.hpp"
//#include "../../lib/gplib.h"
//
//namespace game
//{
//	class Rule : public ci_ext::Object
//	{
//
//		//*************************************************//
//		//�@�\����
//		//*************************************************//
//	private:
//
//		struct DiceData{
//			
//			std::weak_ptr<ci_ext::Object> p_dice;	// �_�C�X�̃|�C���^
//
//			//int				no;					// �_�C�X�̃i���o�[
//			ci_ext::Vec3i	masu;					// �_�C�X�̃}�X���W
//			bool			show_;					// ��ʂɕ\������Ă��邩�ǂ���
//
//			//��Ԉُ�Ƃ��̓R�R�ɒǉ�
//			//�\����\���Ȃ񂩂��R�R�ł��������B
//		};
//
//		struct PlayerData
//		{
//			std::vector<DiceData>	dice_;			// �v���C���[�̎��_�C�X
//			int						hp_;			// �̗�
//		};
//
//		enum Turn{
//			PLAYER_A = 0,
//			PLAYER_B,
//		};
//
//		enum Phase{
//			SUMMON,
//			MAIN,
//			BATTLE,
//		};
//
//
//		//*************************************************//
//		//�@�ϐ�
//		//*************************************************//
//	private:
//
//		Phase	phase_;								// ���݂̃t�F�C�Y
//		Turn	turn_;								// ���݂̃^�[���v���C���[
//		int		diceno_;							// �I������Ă���_�C�X�ԍ�
//
//
//		std::vector<PlayerData>			player_;	// �v���C���[�f�[�^
//		std::vector<std::vector<int>>	board_;		// �{�[�h�̏��
//
//
//		std::weak_ptr<ci_ext::Object> p_player;		// �v���C���[�̃|�C���^
//		std::weak_ptr<ci_ext::Object> p_board;		// �{�[�h�̃|�C���^
//
//		std::weak_ptr<ci_ext::Object> p_camera;		// �J�����̃|�C���^
//	
//
//		//*************************************************//
//		//�@�֐�
//		//*************************************************//
//
//
//	public:
//
//		Rule(const std::string& objectName);
//
//		/*
//			@brief				�_�C�X�̃}�X�̍X�V
//			@param[in] pos		�X�V��̃}�X
//			@param[in] dice		�X�V����_�C�X([0]�v���C���[,[1]�_�C�XNo.)
//		*/
//		void updateMasu(const ci_ext::Vec3i& pos);
//		void updateMasu(const ci_ext::Vec3i& pos, const std::vector<int>& dice);
//		/*
//			@brief			�����̏ꏊ�̃I�u�W�F�N�g���m�F
//			@param[in] pos	�m�F����ꏊ
//			@return			�I�u�W�F�N�g�̓��e(0:�Ȃ��A1:�_�C�X)
//		*/
//		int getExistObj(ci_ext::Vec3i pos);
//
//
//		/*
//			@brief			���݂̑I������Ă���_�C�X���W���擾
//			@return			�Ȃ�
//		*/
//		ci_ext::Vec3i getDiceMasu();
//
//		/*
//			@brief			�����̏ꏊ�̃{�[�h��Ԃ��m�F
//			@param[in] pos	�m�F����ꏊ
//			@return			�{�[�h�̏��(-1:�� 0:�ʏ�)
//		*/
//		int getBoardState(ci_ext::Vec3i pos);
//
//
//		/*
//			@brief				�q�ɑ��M���郁�b�Z�[�W(���ߕ�)�̎�t
//								msg�̓��e���q�ɑ��M����B
//			@ATTEMTION			msg�̈�ԏ��߂͕K�����얽�߂ɂ��邱��
//			@param[in]	msg		���M���閽�ߕ�
//			@param[in]	process	���M��̃I�u�W�F�N�g�Ȃǂ̓��e
//			@return				�Ȃ�
//		*/
//		void sendMsg(const std::string& msg, const std::string& process);
//
//		/*
//			@brief				�t�F�[�Y�̏�ԕύX
//			@return				�Ȃ�
//		*/
//		void NextPhase();
//
//	
//		
//		void init() override;
//		void render() override;
//		void update() override;
//
//
//		//(���)�ǉ�
//		//
//		//�Q�b�^�[�@�_�C�X�X�e�[�g
//		bool Rule::getDiceShow();
//
//		ci_ext::Vec3i getDiceStartMasu();
//
//		//
//		/////
//		
//	};
//
//}