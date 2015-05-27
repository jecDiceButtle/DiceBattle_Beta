
#include "../../lib/gplib.h"
#include "movableObject.h"
#include "button.h"

namespace game
{


	

	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//
	

	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//
	void Button::ButtonClick(std::string resname, float posx, float posy, bool& flag)
	{
		//�}�E�Xpos
		pt_ = gplib::input::GetMousePosition();
		setPos(pt_.x,pt_.y,0.f);

		//�{�^��
		rc_.left = (posx - (gplib::graph::Draw_GetImageWidth(resname) / 2));
		rc_.top = (posy - (gplib::graph::Draw_GetImageHeight(resname)/2));
		rc_.right = (posx + (gplib::graph::Draw_GetImageWidth(resname) / 2));
		rc_.bottom = (posy + (gplib::graph::Draw_GetImageHeight(resname) / 2));

		gplib::graph::Draw_2DClear();

		gplib::graph::Draw_Graphics(
			posx, posy, 0.f, resname, 0, 0, 
			gplib::graph::Draw_GetImageWidth(resname), gplib::graph::Draw_GetImageHeight(resname), 
			0.f, scaleX, scaleY, a, r, g, b);

		gplib::graph::Draw_2DRefresh();

		//����
		if (rc_.left<pt_.x&&rc_.right>pt_.x&&
			rc_.top<pt_.y&&rc_.bottom>pt_.y)
		{
			r = 200;
			g = 200;
			b = 200;

			if (gplib::input::CheckPush(gplib::input::KEY_MOUSE_LBTN))
			{
				scaleX = 1.2f;
				scaleY = 1.2f;

			}
			else if (gplib::input::CheckPull(gplib::input::KEY_MOUSE_LBTN))
			{
				flag = true;
				scaleX = 1.0f;
				scaleY = 1.0f;
			}
		}
		else
		{

			r = 255;
			g = 255;
			b = 255;

			scaleX = 1.0f;
			scaleY = 1.0f;


		}


	}


	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//

	Button::Button(const std::string& objectName)
		:
		MovableObject(objectName),
		scaleX(1.0f), scaleY(1.0f), a(255), r(255), g(255), b(255)
	{

	}

	void Button::render()
	{

	}

	void Button::update()
	{
		
	}

}