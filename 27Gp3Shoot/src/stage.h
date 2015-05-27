#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class CSceneStage : public ci_ext::Object
	{
	private:

		//*************************************************//
		//�@�萔
		//*************************************************//

		//��ԑJ��
		enum class STAGESTATE
		{
			STANDBY,				//�Q�[���O
			PLAYING,				//�Q�[����
			RESULT,					//�Q�[���I�[�o�[
		};

		//�v���C���[�^�[��
		enum TURN
		{
			null = -1,				//�^�[�����ݒ�
			PLAYER1,				//�v���C���[�P
			PLAYER2,				//�v���C���[�Q
		};

		//�^�[���t�F�C�Y
		enum class PHASE
		{
			null = -1,				//�^�[�����ݒ�
			SUMMON,					//����(�A���t�@�ۗ��j
			MAIN,					//�ړ�
			BUTTLE,					//�ΐ�
			END,					//�i�Ƃ肠�������z�u�j
		};


		//*************************************************//
		//�@�ϐ�
		//*************************************************//

	private:
		

		bool		phaseinit_;						// �t�F�C�Y�̍X�V�t���O
		bool		cutinF_;						// �J�b�g�C�����s���t���O,true:���s��

		std::weak_ptr<ci_ext::Object> p_player;		// �v���C���[�̃|�C���^
		std::weak_ptr<ci_ext::Object> p_board;		// �{�[�h�̃|�C���^
		std::weak_ptr<ci_ext::Object> p_dm;			// �{�[�h�̃|�C���^


		//tuika
		std::weak_ptr<ci_ext::Object> p_camera;		// �J�����̃|�C���^
	
		
		TURN		turn_;							// �v���C���[�^�[��
		STAGESTATE	state_;							// ��ԑJ��
		PHASE		phase_;							// �^�[���t�F�C�Y


		//*************************************************//
		//�@�֐�
		//*************************************************//

		/*
			@brief			�X�^���o�C���
			@return			�Ȃ�
		*/
		void Standby();

		/*
			@brief			���C�����
			@return			�Ȃ�
		*/
		void Playing();

		/*
			@brief			�I�����
			@return			�Ȃ�
		*/
		void Result();


		/*
			@brief			���b�Z�[�W���M
			@return			�Ȃ�
		*/
		void postTurnAndPhaseMsg();


	public:

		CSceneStage(const std::string& objectName);

		
		

		/*
			@brief			�t�F�C�Y�̍X�V
			@return			�Ȃ�
		*/
		void NextPhase();

		/*
			@brief					�_�C�X�ړ�����
			@param[in]	pos			�ړ���̍��W
			@return					�����������ǂ���
			@true					�������ꂽ
			@false					��������Ă��Ȃ�
		*/
		bool checkMovableDice(const ci_ext::Vec3i &pos);


		void init() override;
		void render() override;
		void update() override;
	};

}