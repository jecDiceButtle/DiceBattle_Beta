#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"




namespace game
{

	class UI : public MovableObject
	{
		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		static const int srcX[];
		static const int cutin_srcY[];


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	public:
		enum UITYPE{ POPUP, CHARA, PHASE, CUTINMONSTER, CUTINPHASE ,CUTINTURN};
	private:
		UITYPE type_;
		float x, y;

		static int turnPlayer_;						// �t�F�C�Y�A�^�[�����Œ艻�B�ꎞ���̂��B
		static int phase_;

		std::string monsnum_;						// �����X�^�[�J�b�g�C���p�A0:�N�W�� 1:�L�m�R 2:�g��


		//cutin
		bool	M_initF;
		bool	P_initF;
		bool	T_initF;
		std::weak_ptr<ci_ext::Object> timer_;
		enum MOVESTATE
		{ M_RIGHTCENTER, M_CENTER, M_CENTERLEFT,	//�����X�^�[
		  P_RIGHTCENTER, P_CENTER, P_CENTERLEFT,	//�t�F�C�Y
		  T_RIGHTCENTER, T_CENTER, T_CENTERLEFT		//�v���C���[�^�[��
		};
		MOVESTATE M_state_, P_state_, T_state_;
		float phaseC_x, phaseC_y;
		float charaC_x, charaC_y;
		float turnC_x, turnC_y;

		//popup

		float sizeX;
		float sizeY;
		bool flag;
		bool Yes;
		bool No;


		//chara
		int turn;
		float player1scale;
		float player2scale;

		//phase
		float scaleX;
		float scaleY;

		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:


	public:

		/*
		@brief							�R���X�g���N�^
		@param[in]	objectName			�I�u�W�F�N�g��
		@return							�Ȃ�
		*/
		UI(const std::string& objectName, UITYPE type, float posx = 0, float posy = 0,int monsnum = -1);
		void flagset();
		void ChangeTurn(const int playerID);
		void Changephase(const int phase);
		/*
		@brief							�`��
		@return							�Ȃ�
		*/

		void init() override;


		void render() override;

		void update() override;


		void receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg) override;
		/*
		@brief							�t���[���X�V
		@return							�Ȃ�
		*/


	};
}