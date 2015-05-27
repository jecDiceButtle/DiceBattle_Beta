#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/gplib.h"
#include "effect.h"
#include "button.h"
#include "stage.h"

namespace game
{

	class Menu : public ci_ext::Object
	{
		//追加
		std::weak_ptr<ci_ext::Object> p_playbtns;			//buttonのポインタ(play
		std::weak_ptr<ci_ext::Object> p_otherbtns;			//buttonのポインタ(other
		std::weak_ptr<ci_ext::Object> p_optionbtns;			//buttonのポインタ(option
		bool playflag;
		bool otherflag;
		bool optionflag;
		//
	public:
		Menu(const std::string& objectName) :
			Object(objectName),
			playflag(false)
		{
			//effect::Create(0, 0, effect::EFFECTTYPE::FADEINWHITE);
			gplib::graph::Draw_LoadObject("menu_back", "res/gra/menu_back.png",0xFFFFFFFF);
			//追加
			gplib::graph::Draw_LoadObject("menu_play", "res/gra/menu_play.png");
			gplib::graph::Draw_LoadObject("menu_other", "res/gra/menu_other.png");
			gplib::graph::Draw_LoadObject("menu_option", "res/gra/menu_option.png");
		}

		void init() override
		{
			//追加
			p_playbtns = insertAsChild(new Button("button"));
			p_otherbtns = insertAsChild(new Button("button"));
			p_optionbtns = insertAsChild(new Button("button"));
		}

		void render() override
		{
			gplib::graph::Draw_2DClear();

			float w = (float)gplib::graph::Draw_GetImageWidth("menu_back");
				float h = (float)gplib::graph::Draw_GetImageHeight("menu_back");
			gplib::graph::Draw_GraphicsLeftTop(0, 0, 1.f, "menu_back",
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
				insertToParent(new game::CSceneStage("scene_stage"));
				flag = false;
			}
		}

		void update() override
		{
			//追加
			auto btnobj = ci_ext::weak_to_shared<Button>(p_playbtns);
			btnobj->ButtonClick("menu_play", 640, 300, playflag);
			auto btnobj2 = ci_ext::weak_to_shared<Button>(p_otherbtns);
			btnobj2->ButtonClick("menu_other", 640, 400, otherflag);
			auto btnobj3 = ci_ext::weak_to_shared<Button>(p_optionbtns);
			btnobj3->ButtonClick("menu_option", 640, 500, optionflag);

			if (playflag==true)
			{
				insertToParent(new game::CSceneStage("scene_stage"));		//ここにイベントを記述
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

