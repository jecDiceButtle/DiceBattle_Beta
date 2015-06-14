/*
*	�t�@�C����	�F	phase_battle.h
*	�����		�F	�ێR�m��Y
*	�����		�F	2015/06/05
*	���e		�F	�o�g���t�F�C�Y���s�����̃I�u�W�F�N�g�B
*					�y���ݓ��삳���Ă�����́z
*					�E�o�g���v�Z
*					�E�A�j���[�V����
*/
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "../../lib/ci_ext/vec3.hpp"


namespace game
{
	class PhaseBattle : public ci_ext::Object
	{

		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		
		enum STATE{
			CALC,		//�o�g���v�Z��
			PUSHANIM,	//�����o���A�j���[�V������
			FALLANIM,	//�����A�j���[�V������
			END,		//�I��
		};

		const int PUSHSPEED;

		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		STATE	state_;
		std::weak_ptr<ci_ext::Object>	p_rule;			//�e(���[���I�u�W�F�N�g)�|�C���^

		struct Data
		{
			std::string dir_;							//�ړ���̕��p
			std::string key_;							//�_�C�X�̃L�[���[�h

			Data(){}
			Data(const std::string dir, const std::string key):
				dir_(dir), key_(key)
			{}

		};

		std::vector<Data>	movedice_;					//�ړ��_�C�X�̃f�[�^


		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:
		
		
		/*
			@brief					�v�Z������
		*/
		void calc();
		/*
			@brief					�ړ������̏�����
		*/
		void initPushAnim();
		/*
			@brief					����������
		*/
		void fallAnim();


		/*
			@brief					�o�g���̌v�Z�i�ċA�����j
									�ړ�����_�C�X����������Amovedice_�փf�[�^��ǉ����Ă����B
			@param[in]	dir			�ړ���̕��p
			@param[in]	masu		�m�F�O�̍��W�i���݂̍��W�j
			@param[in]	no			�ċA�������J��Ԃ�����
		*/
		void battleCalc(const std::string& dir, const ci_ext::Vec3i masu, const int no);

		
	public:

		/*
			@brief					�R���X�g���N�^
			@param[in] objectName	�I�u�W�F�N�g��
			@param[in] pParent		�e�̃|�C���^
		*/
		PhaseBattle(const std::string& objectName, const std::weak_ptr<ci_ext::Object>& prule);
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

