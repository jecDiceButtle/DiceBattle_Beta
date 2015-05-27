#include "ui.h"
#include "../../lib/gplib.h"


#include "movableObject.h"
#include "easing.h"
#include "timer.h"


namespace game
{
	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//
	const int UI::srcX[] = { 0, 150, 300, 450 };
	const int UI::cutin_srcY[] = { 0, 74, 143, 222 };
	
	int UI::turnPlayer_ = 0;
	int UI::phase_ = 0;

	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//
	void UI::flagset(){
		if (flag == false){
			flag = true;
		}
		else if (flag == true){
			flag = false;
		}
	}

	void UI::ChangeTurn(const int playerID)
	{
		turnPlayer_ = playerID;

	}

	void UI::Changephase(const int phase)
	{
		phase_ = phase;
	}



	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//
	UI::UI(const std::string& objectName, UITYPE type, float posx, float posy,int monsnum)
		:
		MovableObject(objectName),
		type_(type),
		M_state_(MOVESTATE::M_RIGHTCENTER),
		P_state_(MOVESTATE::P_RIGHTCENTER),
		T_state_(MOVESTATE::T_RIGHTCENTER),
		M_initF(true),
		P_initF(true),
		T_initF(true),
		flag(false),
		x(posx),
		y(posy)
	{
		//モンスター用resname
		std::string str = "";

		if (monsnum == 0) str = "cutin_tori";
		if (monsnum == 1) str = "cutin_kinoko";
		if (monsnum == 2) str = "cutin_kuzira";

		monsnum_ = str;

		switch (type_)
		{

		case UITYPE::POPUP:

			sizeX = 1.f;
			sizeY = 1.f;
			Yes = false;
			No = false;

			gplib::graph::Draw_LoadObject("question", "res/gra/question.png");
			gplib::graph::Draw_LoadObject("Yes", "res/gra/anser_yes.png");
			gplib::graph::Draw_LoadObject("No", "res/gra/anser_no.png");

			break;

		case UITYPE::CHARA:
			player1scale = 1.f;
			player2scale = 1.f;


			gplib::graph::Draw_LoadObject("player1", "res/gra/player1.png", 0xFFFFaaFF);
			gplib::graph::Draw_LoadObject("player2", "res/gra/player2.png", 0xFFFFaaFF);
			break;

		case UITYPE::PHASE:
			scaleX = 1.f;
			scaleY = 1.f;
			gplib::graph::Draw_LoadObject("phase", "res/gra/phase.png");
			break;

		case UITYPE::CUTINMONSTER:
			gplib::graph::Draw_LoadObject("cutin_kuzira", "res/gra/cutin_kuzira.png", 0xFFFFaaFF);
			//キノコ絵がないので仮にクジラをおいてます。。
			gplib::graph::Draw_LoadObject("cutin_kinoko", "res/gra/cutin_kinoko.png", 0xFFFFaaFF);
			gplib::graph::Draw_LoadObject("cutin_tori", "res/gra/cutin_tori.png", 0xFFFFaaFF);
			easing::Create("cutin_chara1", easing::EASINGTYPE::CIRCIN, gplib::system::WINW + 600.f, 640, 60);
			easing::Create("cutin_chara2", easing::EASINGTYPE::CIRCIN, 640, -600, 30);
			charaC_x = gplib::system::WINW + 600.f;
			charaC_y = gplib::system::WINH / 2.f;
			break;

		case UITYPE::CUTINPHASE:
			gplib::graph::Draw_LoadObject("cutin_phase", "res/gra/cutin_phase.png", 0xFFFFaaFF);
			easing::Create("cutin_phase1", easing::EASINGTYPE::CIRCIN, gplib::system::WINW + 600.f, 640, 60);
			easing::Create("cutin_phase2", easing::EASINGTYPE::CIRCIN, 640, -600, 60);
			phaseC_x = gplib::system::WINW + 600.f;
			phaseC_y = gplib::system::WINH / 2.f;
			break;
		case UITYPE::CUTINTURN:
			gplib::graph::Draw_LoadObject("cutin_turn", "res/gra/cutin_turn.png", 0xFFFFaaFF);
			easing::Create("cutin_turn1", easing::EASINGTYPE::CIRCIN, gplib::system::WINW + 500.f, 640, 60);
			easing::Create("cutin_turn2", easing::EASINGTYPE::CIRCIN, 640, -500, 60);
			turnC_x = gplib::system::WINW + 500.f;
			turnC_y = 250;
			break;


		}
	}


	void UI::init()
	{
		timer_ = insertAsChildSleep(new game::Timer("LimitTimer"), 120);
		
	}

	void UI::render()
	{

		gplib::graph::Draw_2DClear();
		switch (type_)
		{

		case UITYPE::POPUP:
			if (flag){
				std::string t = "行動を決定しますか？";
				gplib::graph::Draw_Graphics(x, y, 0.f, "question", 0, 0, gplib::graph::Draw_GetImageWidth("question"), gplib::graph::Draw_GetImageHeight("question"), 0.f, 1.5f, 1.5f);
				gplib::graph::Draw_Graphics(x - 200, y + 200, 0.f, "Yes", 0, 0, gplib::graph::Draw_GetImageWidth("Yes"), gplib::graph::Draw_GetImageHeight("Yes"), 0.f, sizeY, sizeY);
				gplib::graph::Draw_Graphics(x + 200, y + 200, 0.f, "No", 0, 0, gplib::graph::Draw_GetImageWidth("No"), gplib::graph::Draw_GetImageHeight("No"), 0.f, sizeX, sizeX);
				gplib::font::Draw_FontText(static_cast<int>(x), static_cast<int>(y), 0.f, t, ARGB(255, 255, 255, 255), 0.f);
			}
			break;
		case UITYPE::CHARA:

			gplib::graph::Draw_Graphics(x, y, 0.f, "player1", 0, 0, gplib::graph::Draw_GetImageWidth("player1"), gplib::graph::Draw_GetImageHeight("player1"), 0, player1scale, player1scale);
			gplib::graph::Draw_Graphics(gplib::system::WINW - x, y, 0.f, "player2", 0, 0, gplib::graph::Draw_GetImageWidth("player1"), gplib::graph::Draw_GetImageHeight("player1"), 0, player2scale, player2scale);
			break;
		case UITYPE::PHASE:

			gplib::graph::Draw_Graphics(x, y, 0.f, "phase", srcX[phase_], 0, (gplib::graph::Draw_GetImageWidth("phase") / 4), gplib::graph::Draw_GetImageHeight("phase"), 0.f);
			break;

		case UITYPE::CUTINMONSTER:

			gplib::graph::Draw_Graphics(charaC_x, charaC_y, 0.f, monsnum_, 0, 0, 700, 250, 0.f, 1.5f, 1.5f);

			break;

		case UITYPE::CUTINPHASE:
			gplib::graph::Draw_Graphics(phaseC_x, phaseC_y, 0.f, "cutin_phase", 0, cutin_srcY[phase_], 600, 74, 0.f, 1.5f, 1.5f);
			break;

		case UITYPE::CUTINTURN:
			gplib::graph::Draw_Graphics(turnC_x, turnC_y, 0.f, "cutin_turn", 0, 150 / 2 * turnPlayer_, 500, 75, 0.f, 1.5f, 1.5f);
			break;
		}
		gplib::graph::Draw_2DRefresh();

	}

	void UI::update()
	{

		auto P_timer = ci_ext::weak_to_shared<game::Timer>(timer_);
		auto M_timer = ci_ext::weak_to_shared<game::Timer>(timer_);
		auto T_timer = ci_ext::weak_to_shared<game::Timer>(timer_);

		switch (type_)
		{

		case UITYPE::POPUP:
			if (gplib::input::CheckPush(gplib::input::KEY_UP))
			{
				flagset();
			}
			if (flag){
				if (gplib::input::CheckPush(gplib::input::KEY_LEFT)){
					sizeY = 1.2f;
					sizeX = 1.0f;
					Yes = true;
					No = false;
				}
				else if (gplib::input::CheckPush(gplib::input::KEY_RIGHT)){
					sizeX = 1.2f;
					sizeY = 1.0f;
					Yes = false;
					No = true;
				}
				else if (gplib::input::CheckPush(gplib::input::KEY_DOWN) && (Yes == true || No == true)){
					//決定みたいなやつ
					kill();
				}
			}
			break;

		case UITYPE::CHARA:
			if (turnPlayer_ == 0)		//プレイヤー1ターンの時
			{
				player1scale = 1.2f;
				player2scale = 1.0f;

			}
			if (turnPlayer_ == 1)		//プレイヤー2ターンの時
			{
				player2scale = 1.2f;
				player1scale = 1.0f;

			}
			break;
		case UITYPE::PHASE:
			break;

		case UITYPE::CUTINMONSTER:
			switch (M_state_)
			{
			case MOVESTATE::M_RIGHTCENTER:
				if (M_initF == true)
				{
					easing::Start("cutin_chara1");
					M_initF = false;
				}
				easing::GetPos("cutin_chara1", charaC_x);
				if (charaC_x <= 640)
				{
					M_state_ = MOVESTATE::M_CENTER;
					M_initF = true;
				}
				break;
			case MOVESTATE::M_CENTER:
				if (M_timer->get() > 3.0f)
				{
					M_state_ = MOVESTATE::M_CENTERLEFT;
				}
				break;
			case MOVESTATE::M_CENTERLEFT:
				if (M_initF == true)
				{
					easing::Start("cutin_chara2");
					M_initF = false;
				}
				easing::GetPos("cutin_chara2", charaC_x);
				if (charaC_x <= -700)
				{
					kill();
				}
				break;
			}
		case UITYPE::CUTINPHASE:

			switch (P_state_)
			{
			case MOVESTATE::P_RIGHTCENTER:
				if (P_initF == true)
				{
					easing::Start("cutin_phase1");
					P_initF = false;
				}
				easing::GetPos("cutin_phase1", phaseC_x);
				if (phaseC_x <= 640)
				{
					P_state_ = MOVESTATE::P_CENTER;
					P_initF = true;
				}
				break;
			case MOVESTATE::P_CENTER:
				if (P_timer->get() > 3.0f)
				{
					P_state_ = MOVESTATE::P_CENTERLEFT;
				}
				break;
			case MOVESTATE::P_CENTERLEFT:
				if (P_initF == true)
				{
					easing::Start("cutin_phase2");
					P_initF = false;
				}
				easing::GetPos("cutin_phase2", phaseC_x);
				if (phaseC_x <= -600)
				{
					kill();
				}
				break;
			}

		case UITYPE::CUTINTURN:
			switch (T_state_)
			{
			case MOVESTATE::T_RIGHTCENTER:
				if (T_initF == true)
				{
					easing::Start("cutin_turn1");
					T_initF = false;
				}
				easing::GetPos("cutin_turn1", turnC_x);
				if (turnC_x <= 640)
				{
					T_state_ = MOVESTATE::T_CENTER;
					T_initF = true;
				}
				break;
			case MOVESTATE::T_CENTER:
				if (T_timer->get() > 3.0f)
				{
					T_state_ = MOVESTATE::T_CENTERLEFT;
				}
				break;
			case MOVESTATE::T_CENTERLEFT:
				if (T_initF == true)
				{
					easing::Start("cutin_turn2");
					T_initF = false;
				}
				easing::GetPos("cutin_turn2", turnC_x);
				if (turnC_x <= -500)
				{
					kill();
				}
				break;


			}

		}

		easing::Step();

	}

	void UI::receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg)
	{

		//メッセージ分割
		auto msgVec = gplib::text::split(msg, ",");

		for (auto ms : msgVec){
			//さらに分割
			auto mVec = gplib::text::split(ms, "=");

			if (mVec[0] == "player")
			{
				ChangeTurn(stoi(mVec[1]));
			}
			if (mVec[0] == "phase")
			{
				phase_ = stoi(mVec[1]);
			}
		}
	}


}