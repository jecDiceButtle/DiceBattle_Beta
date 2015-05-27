#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"

//tuika
#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"
//tuika

namespace game
{

	class Board : public MovableObject
{
	//*************************************************//
	//�@�萔
	//*************************************************//
private:

	static const ci_ext::Vec3i MAPSIZE;					//�}�b�v�̍ő�l


	//*************************************************//
	//�@�ϐ�
	//*************************************************//
private:


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
		@brief							���W�̈ړ��m�F
		@param[in]	pos					�ړ���̍��W
		@return							�ړ��\���ǂ���
		@true							��
		@false							�s��
	*/
	bool isMovablePosition(const ci_ext::Vec3i &pos);



};

}