
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"



#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"



namespace game
{

	class Dice : public MovableObject
	{
		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		enum DIRECTION{ NORTH, SOUTH, EAST, WEST, CENTER };
		enum TYPE{ GU, CH, PA, DEF };

		//�s���X�e�[�^�X
		enum STATE
		{
			DEAD,			// ���ɏ�ԁi�`��Ȃ�����Ȃ��j
			IDOL,			// �ҋ@���
			MOVE,			// �ړ���
			ATTACK,			// �U����
		};

		//�`��X�e�[�^�X
		enum DISPSTATE
		{
			DICE,			// �T�C�R���̂�
			DICE_CHARA,		// �T�C�R���ƃL�����N�^�[
			CHARA,			// �L�����̂�
		};

		//�A�j���[�V�����֘A�̒萔
		const float OFFSET;
		const int ANIMFRAMES;
		const float VELOCITY = OFFSET / ANIMFRAMES;


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:
		ci_ext::Vec3f angle;
		ci_ext::Vec3f scale;
		int alpha;
		int rgb; //����3�F�܂Ƃ߂Ă���


		TYPE face[6];
		TYPE defType_;
		STATE state_;										// �s�����
		DISPSTATE dispstate_;								// �`����
		bool selected_;										// �I�����

		int playerID_;
		int animcnt_;										// �A�j���J�E���^

		bool RGBFLAG;


		ci_ext::Vec3f prepos_;		//�֐���_
		//���g���Ă��Ȃ�
		//float nowpos_y;
		//float nowpos_x;
		//D3DXVECTOR3 nowpos_;
		ci_ext::Vec3f nextpos_;		//�֐���_


		//Qauternion
		D3DXQUATERNION middleQ;
		D3DXQUATERNION startQ;
		D3DXQUATERNION endQ;
		float rollAnim;
		D3DXMATRIX matRot;

		DIRECTION hougaku_; //�֐���_


		//�ǉ�
		std::weak_ptr<ci_ext::Object> p_mons;			//�����X�^�[�̃|�C���^

		static const TYPE ATKTYPE[6];						// �U�������̏�����	�i���j			

		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:

		/*
		@brief							�ҋ@���̏���
		@return							�Ȃ�
		*/
		void DoIdol();

		/*
		@brief							�ړ����̏���
		@return							�Ȃ�
		*/
		void DoMove();

		/*
		@brief							�ړ����̏���
		@return							�Ȃ�
		*/
		void Attack();


		/*
		@brief							�ړ����̏���
		@return							�Ȃ�
		*/
		void DoDead();


		/*
		@brief							�R���X�g���N�^�Ŗڂ̒l���Z�b�g
		@return							�Ȃ�
		*/
		void setFace(TYPE ONE_SIX, TYPE TWO_FIVE, TYPE THREE_FOUR);


		void swap(DIRECTION dir);


		//***************//
		//�@�������p�֐��@//
		//***************//
		//-------------DoMove------------//
		/*
		@brief							�ړ��v�Z
		@return							�Ȃ�
		*/
		void MoveDice(STATE &pOut);
		void ClacMove(ci_ext::Vec3f &pOut_nowpos, const DIRECTION &hougaku);
		void setStartQuaternion(D3DXQUATERNION &out_end, const D3DXQUATERNION &in_middle, const DIRECTION &hougaku);
		//-------------------------------//
		//-------------DoIdol------------//
		/*
		@brief							�ړ��v�Z
		@return							�Ȃ�
		*/
		void change_rgb();
		//-------------------------------//
		//----------prepareMove----------//
		/*
		@brief							�ړ������t���O���Z�b�g
		@return							DIRECT
		*/
		DIRECTION setHougaku(const ci_ext::Vec3f &pos);
		/*
		@brief							�ړ������t���O���Z�b�g
		@return							DIRECT
		*/
		ci_ext::Vec3f MasuToPos(const ci_ext::Vec3i &masu);
		//-------------------------------//


		void setDicePosX(const ci_ext::Vec3i &masu);
		void setDicePosY(const ci_ext::Vec3i &masu);




		

		//***************/
		//�@ �֐����ϐ� �@//
		//***************//
		//----------DIRECTION�֐�----------//
		//@brief							�f�t�H���gDIRECTON�֐�
		DIRECTION hougaku();
		//@brief							�ړ������t���O���Z�b�g
		DIRECTION hougaku(const DIRECTION &in_dir);
		//---------------------------------//
		//-----------nextpos�֐�-----------//
		//@brief							�f�t�H���gnextpos�֐�
		ci_ext::Vec3f nextpos();
		//@brief							nextpos���Z�b�g
		ci_ext::Vec3f nextpos(const ci_ext::Vec3f &in_pos);
		//---------------------------------//




	public:
		/*
		@brief							�R���X�g���N�^
		@param[in] objectName			�I�u�W�F�N�g��
		@param[in] type					�h�䑤�̎푰
		@param[in] playerID				�v���C���[
		@param[in] pos					�������W
		@return							�Ȃ�
		*/
		Dice(const std::string& objectName, const int type, const int playerID, const ci_ext::Vec3i& pos = ci_ext::Vec3i::zero());

		void init()	override;
		void render() override;
		void update() override;

		void receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg) override;


		int getTopType();	//�g���Ă�̂���H

		/*
		@brief							�A�N�Z�T�A�ҋ@�����ǂ���
		@return							�ҋ@�����ǂ���
		@true							�ҋ@��
		*/
		bool isIdoling();


		/*
		@brief							�A�N�Z�T�A��Ԃ̊m�F
		@return							�ړ������ǂ���
		@true							�ړ���
		*/
		bool isMoving();

		/*
		@brief							�A�N�Z�T�A��Ԃ̊m�F
		@return							���S�����ǂ���
		@true							���S��
		*/
		bool isDying();


		/*
		@brief							�A�N�Z�T�A�h��
		@return							�Ȃ�
		*/

		void Spawn();


		/*
		@brief							�A�N�Z�T�A�_�C�X�����S��Ԃ�
		@return							�Ȃ�
		*/
		void destroy();


		/*
		@brief							�A�N�Z�T�A�U�������̃`�F�b�N
		@return							�����l
		*/
		int getAtkSpecies();

		/*
		@brief							�A�N�Z�T�A�h�䑮���̃`�F�b�N
		@return							�����l
		*/
		int getDefSpecies();

		/*
		@brief							�ړ��������i�ړ��ɕK�v�Ȓl�����Z�b�g�j
		@return							�Ȃ�
		*/
		void prepareMove(ci_ext::Vec3i &masu);

		/*
		@brief							selectflag�̐؂�ւ�
		@return							�Ȃ�
		*/
		void OnSelectFlag();
		void OffSelectFlag();

	};

}