#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

namespace game
{

	class Player : public ci_ext::Object
	{
		//*************************************************//
		//�@�萔
		//*************************************************//
	private:
		static const int DEFAULTCOST;
		//�ǉ��v�f�@�R�X�g�̕`��֘A
		//std::string resname_;
		ci_ext::Vec3f pos_1P;
		ci_ext::Vec3f pos_2P;

		const int COST_W = 30;
		const int COST_H = 30;
		//80�O�オ�x�X�g
		const int COST_Space = 80;


		ci_ext::Vec3f lifepos_1P;
		ci_ext::Vec3f lifepos_2P;
		int life_posx;
		int srcW = 300;
		int LIFE_1P_W;
		int LIFE_2P_W;
		const int LIFE_W = 300;
		const int LIFE_H = 100;




	int turn_;
		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:

		std::weak_ptr<ci_ext::Object> p_parent;				// �e�̃|�C���^
		std::vector<int>cost;								// �R�X�g


		//*************************************************//
		//�@�֐�
		//*************************************************//
	private:


	public:

		/*
		@brief							�R���X�g���N�^
		@param[in]	objectName			�I�u�W�F�N�g��
		@param[in]	ptr					�e�̃|�C���^
		@return							�Ȃ�
		*/
		Player(const std::string& objectName, const std::weak_ptr<Object> ptr);



		/*
		@brief							�`��
		@return							�Ȃ�
		*/
		void render() override;

		/*
		@brief							�t���[���X�V
		@return							�Ȃ�
		*/
		void update() override;

		/*
		@brief							�R�X�g�m�F
		@param[in] id					�v���C���[ID
		@return							�R�X�g�c���Ă邩�ǂ���
		@true							�c���Ă���
		@false							�c���Ă��Ȃ�
		*/
		bool isCostRemain(const int id);

		/*
		@brief							�R�X�g����
		@param[in] id					�v���C���[ID
		@param[in] point				���炷�ʁi��{�l1�j
		@return							�Ȃ�
		*/
		void CostDecrease(const int id, const int point = 1);

		/*
		@brief							�R�X�g��߂�
		@param[in] id					�v���C���[ID
		@return							�Ȃ�
		*/
		void CostReset(const int id);

		/*
		@brief
		@param[in] id
		@return
		*/
		void VictryorDefeat(const int id);

	void receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg)override;



	};

}