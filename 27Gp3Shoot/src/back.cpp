#include "back.h"
#include "../../lib/gplib.h"



namespace game
{




	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//
	

	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//


	
	Back::Back(const std::string& objectName, const std::string back)
		:
		Object(objectName),
		s_back(back)
	{
		gplib::graph::Draw_LoadObject("TitleBack", "res/gra/TitleBack.png");
		gplib::graph::Draw_LoadObject("StageBack", "res/gra/StageBack.png");
	}

	void Back::render()
	{
		gplib::graph::Draw_2DClear();

		if (s_back == "StageBack"){
			gplib::graph::Draw_GraphicsLeftTop(0.f, 0.f, 1.f, s_back, 0, 0, 1980, 1080, 0.f, nullptr, (float)gplib::system::WINW / 1980.f, (float)gplib::system::WINH / 1080.f);
		}
		else if (s_back == "TitleBack"){
			gplib::graph::Draw_GraphicsLeftTop(0.f, 0.f, 1.f, s_back, 0, 0, 1920, 1200, 0.f, nullptr, (float)gplib::system::WINW / 1920.f, (float)gplib::system::WINH / 1200.f);

		}
		
		gplib::graph::Draw_2DRefresh();
	}

	void Back::update()
	{

	}

}