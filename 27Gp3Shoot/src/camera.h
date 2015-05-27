#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/dx_media_/dx3dmeshmanager.h"


namespace game
{

class Camera : public ci_ext::Object
{
	//*************************************************//
	//�@�萔
	//*************************************************//
private:
	enum Cameraflag{UP,DOWN,RIGHT,LEFT,FREE};


	//*************************************************//
	//�@�ϐ�
	//*************************************************//
private:

	DxVec3 v;	//�J�����̈ʒu
	DxVec3 v2;	//�J�����̑��x
	DxVec3 lookpos;// ���_���W
	Cameraflag Cflag;

	float kakudo;

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
	Camera(const std::string& objectName);

	void kaiten();
	void input();
	void camera(DxVec3 v2);

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

};

}