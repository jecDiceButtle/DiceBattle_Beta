#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "effect.h"
#include "button.h"
#include "Menu.hpp"
#include "stage.h"

namespace game
{

	class Title : public ci_ext::Object
	{
		//追加
		std::weak_ptr<ci_ext::Object> p_btns;			//buttonのポインタ
		bool btnflag;
		//
	public:
		Title(const std::string& objectName) :
			Object(objectName),
			btnflag(false)
		{
			//effect::Create(0, 0, effect::EFFECTTYPE::FADEINWHITE);
			gplib::graph::Draw_LoadObject("titleback", "res/gra/title_back.png",0xFFFFFFFF);
			//追加
			gplib::graph::Draw_LoadObject("start", "res/gra/title_start.png");
		}

		void init() override
		{
			//追加
			p_btns = insertAsChild(new Button("button"));
		}

		void render() override
		{
			gplib::graph::Draw_2DClear();

			float w = (float)gplib::graph::Draw_GetImageWidth("titleback");
				float h = (float)gplib::graph::Draw_GetImageHeight("titleback");
			gplib::graph::Draw_GraphicsLeftTop(0, 0, 1.f, "titleback",
				0, 0, (int)w, (int)h, 0.f, nullptr,
				gplib::system::WINW / w,
				gplib::system::WINH / h);

			gplib::graph::Draw_2DRefresh();

			//w = (float)gplib::graph::Draw_GetImageWidth("titlelogo");
			//h = (float)gplib::graph::Draw_GetImageHeight("titlelogo");
			//gplib::graph::Draw_Graphics(gplib::system::WINW / 2, 200, 1.f, "titlelogo",
			//	0, 0, (int)w, (int)h);
		}

		void resume() override
		{
			static bool flag = true;
			if (flag){
				insertToParent(new game::Menu("scene_stage"));
				flag = false;
			}
		}

		void update() override
		{
			//追加
			auto btnobj = ci_ext::weak_to_shared<Button>(p_btns);
			btnobj->ButtonClick("start", 640, 300, btnflag);
			if (btnflag==true)
			{
				insertToParent(new game::Menu("scene_stage"));		//ここにイベントを記述
				kill();
			}
			//

			if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
			{
				//effect::Create(0, 0, effect::EFFECTTYPE::FADEOUTWHITE);
				sleep(5);	
			}
		}
	};
}

