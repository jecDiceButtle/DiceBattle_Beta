#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"


namespace game
{

	class Board : public MovableObject
	{
		//*************************************************//
		//�@�萔
		//*************************************************//
	private:

		static const ci_ext::Vec3i MAPSIZE;			//�}�b�v�̍ő�l
		ci_ext::Vec3f scale;						//�\���̔{��
		const float panelLength;					//�p�l����ӂ̒���


		//*************************************************//
		//�@�ϐ�
		//*************************************************//
	private:
		vector<vector<int>> panel_;				//�p�l��


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
		Board(const std::string& objectName);
		Board(const std::string& objectName, const std::vector<std::vector<int>>& mapData_);


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


		///*
		//	@brief							���W�̈ړ��m�F
		//	@param[in]	pos					�ړ���̍��W
		//	@return							�ړ��\���ǂ���
		//	@true							��
		//	@false							�s��
		//*/
		//bool isMovablePosition(const ci_ext::Vec3i &pos);

		void colorChange(int i,int j);			//������

		bool HolePos(const ci_ext::Vec3i &pos);			//���̏ꏊ�@�������킩��Ȃ�


	};
}

