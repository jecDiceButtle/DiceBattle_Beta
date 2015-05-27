#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"

#include "../../lib/ci_ext/vec3.hpp"


namespace game
{

	class Button : public MovableObject
{
	//*************************************************//
	//�@�萔
	//*************************************************//
private:
	

	//*************************************************//
	//�@�ϐ�
	//*************************************************//
private:
	POINT	pt_;
	RECT	rc_;
	float	scaleX, scaleY;
	u_char	a, r, g, b;


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
	Button(const std::string& objectName);

	
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
		@brief							�{�^���쐬�ƃ{�^���ƃ}�E�X�̂����蔻��
		@param[in] resname				�摜�̖��O
		@param[in] posx					�{�^���̈ʒuX
		@param[in] posy					�{�^���̈ʒuY
		@param[in] flag					true�ŃC�x���g����(������}�E�X���N���b�N��true)
		@return							flag
	*/
	void ButtonClick(std::string resname,float posx, float posy,bool& flag);
	



};

}