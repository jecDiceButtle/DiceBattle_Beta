#pragma once 

namespace effect{
	enum class EFFECTTYPE{
		WARNING,
		WARNINGSHAD,
		MYST,
		SPLASH,
		BOMB,
		CIRCLE,
		DIFFUSE,
		DIFFUSE2,
		EFFECT_P1,
		PATTACK,
		PBOMB,
		PDASH,
		FADEOUTBLACK,
		FADEOUTWHITE,
		FADEINBLACK,
		FADEINWHITE,
    STARTTEROP,
		EFFECT_MAX,
	};
	
	/*
	{ CreateEffect_WARNING, StepEffect_WARNING },
	{ CreateEffect_WARNINGSHAD, StepEffect_WARNINGSHAD },
	{ CreateEffect_MYST, StepEffect_MYST },
	{ CreateEffect_SPLASH, StepEffect_SPLASH },
	{ CreateEffect_BOMB, StepEffect_BOMB },
	{ CreateEffect_CIRCLE, StepEffect_CIRCLE },
	{ CreateEffect_DIFFUSE, StepEffect_DIFFUSE },
	{ CreateEffect_DIFFUSE2, StepEffect_DIFFUSE2 },
	{ CreateEffect_P1, StepEffect_P1 },
	{ CreateEffect_PATTACK, StepEffect_PATTACK },
	{ CreateEffect_PBOMB, StepEffect_PBOMB },
	{ CreateEffect_PDASH, StepEffect_PDASH },
	{ CreateEffect_FadeOutBlack, StepEffect_FadeOutBlack },
	{ CreateEffect_FadeOutWhite, StepEffect_FadeOutWhite },
	{ CreateEffect_FadeInBlack, StepEffect_FadeInBlack },
	{ CreateEffect_FadeInWhite, StepEffect_FadeInWhite },*/
	void LoadReses();
	void Step();
	void Draw();
	void Init();
	void Create(int x, int y, EFFECTTYPE type);
}
