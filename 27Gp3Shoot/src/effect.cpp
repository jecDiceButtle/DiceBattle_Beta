


#include	"../../lib/GpLib.h"
#include	"../../lib/charbase.h"


#include "effect.h"

#include <vector>
#include <algorithm>

using namespace gplib;


struct Effect
{
	charabase::CharaBase pos;
	effect::EFFECTTYPE type;
	int	count;		//出現フレーム数
	int renderMode;	//描画方法
	int repertDraw;	//重ねる回数
	//charabase::CharaBaseの描画、処理に直接関わらないためこちらへ
	float	animCnt;
	float	animFrm;			//アニメ速度 切り替えフレーム
	int		animMax;			//アニメ最大数
	float	addAngle;			//角度加算速度

	//プロミネンス用追加データ
	int		resWSize;			//ファイルの横配置数
	int		no;

};

typedef void(*create)(int x, int y, effect::EFFECTTYPE type);
typedef bool (*step)(Effect& effect);



std::vector<Effect> effects;


//---------------------------
//エフェクトのカウント処理
//カウント終了でfalse
//本来はcharabase::CharaBaseにて行う
//charabase::CharaBaseを経由しない方法のサンプル
//---------------------------
bool EffectCount(Effect& effect)
{
	effect.count --;
	if(effect.count <= 0){ 
		return false;
	}
	return true;
}

//---------------------------
//エフェクトの回転処理
//回転終了でfalse
//---------------------------
bool EffectAngle(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	pos.angle += effect.addAngle;
	if(pos.angle > 360){ 
		return false;
	}
	return true;
}

//---------------------------
//エフェクトのアニメ処理
//アニメ終了でfalse
//---------------------------
bool EffectAnime(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//アニメは0.1フレーム更新
	effect.animCnt ++;
	if(effect.animCnt >= effect.animFrm){ 
		effect.animCnt = 0;
		pos.src.x ++;
		if (pos.src.x >= effect.animMax){
			pos.src.x = 0;
			return false;
		}
	}
	return true;
}

//---------------------------
//エフェクトのアルファ処理
//アルファ終了でfalse
//---------------------------
bool EffectAlpha(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//おおよそ0.5secで消滅
	pos.a -= system::ONEFRAME_TIME * 255.f / 3.f;
	if(pos.a < 0){
		return false;
	}
	return true;
}

//---------------------------
//エフェクトの移動処理
//画面外でfalse
//---------------------------
bool EffectMove(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	MoveChar(pos);
	if (pos.pos.x + pos.width / 2 > system::WINW ||
		pos.pos.y + pos.height/2 > system::WINH || 
		pos.pos.x - pos.width/2 < 0 || 
		pos.pos.y - pos.height/2 < 0){
		return false;
	}
	return true;
}

//charabase::CharaBase& pos, 
//float x, float y,					//出現座標
//float addx, float addy,		//移動量
//std::string img,					//画像名
//int imgY, int w, int h,		//imgY = src.y ,w,hエフェクトの幅、高さ
//int anim = 0, float angle = 0.f,//anim = src.c , 角度
//float scaleX = 1.0f, float scaleY = 1.0f		//倍率

void CreateOneEffect(charabase::CharaBase& pos, float x, float y, float addx, float addy,
						std::string img, int imgY, int w, int h,
					  int anim = 0,float angle = 0.f,
					  float scaleX = 1.0f,float scaleY = 1.0f)
{
	charabase::InitCharBase(pos,
		x, y, 0.f,
		addx, addy, 0.f, 
		img,
		anim, imgY, 0,
		w, h,
		0.f,scaleX,scaleY,0.f,
		255.f);
	UseChar(pos);

}

//メイン文字	
void CreateEffect_WARNING(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 300;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"warning",0,EFFECTASZX,EFFECTASZY);

	effect.type = type;					//種類
	effect.count = 60;					//消滅までのフレーム
	effect.renderMode = NORMAL;	//レンダリングモード
	effect.repertDraw = 1;			//描画回数
	effect.animMax = 1;					//アニメーション時の最大枚数
	effect.animCnt = 0.f;				//アニメーションスタートカウント　通常は０
	effect.animFrm = 0;					//アニメーションの速度　何フレームで切り替わるか	
	effect.addAngle = 0.f;			//回転時の速さ	

	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_WARNING(Effect& effect)
{
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}

//ぼかし
void CreateEffect_WARNINGSHAD(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 300;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"warning",0,EFFECTASZX,EFFECTASZY,1);

	effect.type = type;
	effect.count = 120;
	effect.renderMode = ADD;
	effect.repertDraw = 2;
	effect.animMax = 1;
	effect.animCnt = 0.f;
	effect.animFrm = 0;
	effect.addAngle = 0.f;

	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_WARNINGSHAD(Effect& effect)
{
	if(!EffectAlpha(effect)){
		return false;
	}
	//拡大も行う
	effect.pos.scale.x += system::ONEFRAME_TIME * 10.f;
	effect.pos.scale.y += system::ONEFRAME_TIME * 10.f;
	return true;
}


//霧
void CreateEffect_MYST(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 80;
	const int EFFECTASZY = 80;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"myst",0,EFFECTASZX,EFFECTASZY,0,1,0.3f,0.3f);

	effect.pos.a = 200.0f;
	effect.type = type;
	effect.count = 120;
	effect.renderMode = NORMAL;
	effect.repertDraw = 1;
	effect.animMax = 1;
	effect.animCnt = 0.f;
	effect.animFrm = 0;
	effect.addAngle = 0.f;


	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_MYST(Effect& effect)
{
	EffectMove(effect);
	effect.pos.scale.x += 0.01f;
	effect.pos.scale.y += 0.01f;
	if (!EffectAlpha(effect)){
		return false;
	}
	return true;
}


void CreateEffect_SPLASH(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;

	//表示の長さ指定
	effect.count = 30;
	//開始位置生成
	float len = math::GetRandom(30.f,100.f);
	float angle = math::GetRandom(0.f,360.f);
	//開始位置
	float xx = math::ROUND_X(angle,len,(float)x);
	float yy = math::ROUND_Y(angle, len, (float)y);
	//移動方向生成
//	拡散
//	float addx = ROUND_X(angle,len/effect.count,0);
//	float addy = ROUND_Y(angle,len/effect.count,0);
	float addx = -math::ROUND_X(angle, len / effect.count, 0.f);
	float addy = -math::ROUND_Y(angle, len / effect.count, 0.f);
	//種類
	int no = math::GetRandom(0,6);
	//集合
	CreateOneEffect(effect.pos,xx,yy,addx,addy,
		"exp",no,EFFECTASZX,EFFECTASZY,0,0.f,1.f,1.f);
	effect.pos.a = 200.0f;
	effect.type = type;
	effect.renderMode = ADD;
	effect.repertDraw = 1;
	effect.animMax = 7;
	effect.animCnt = 0.f;
	effect.animFrm = 5;
	effect.addAngle = 0.f;

	UseChar(effect.pos);
	effects.push_back(effect);

//	DSound_Play(flashWAV);

}
bool StepEffect_SPLASH(Effect& effect)
{
	EffectMove(effect);
	if(!EffectAnime(effect)){
		return false;
	}
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}


//爆発
void CreateEffect_BOMB(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 96;
	const int EFFECTASZY = 96;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"explode",0,EFFECTASZX,EFFECTASZY,0,0.f,2.f,2.f);

	effect.type = type;
	effect.count = 0;
	effect.renderMode = ADD;
	effect.repertDraw = 1;
	effect.animMax = 16;
	effect.animCnt = 0.f;
	effect.animFrm = 1;
	effect.addAngle = 3.f;

	UseChar(effect.pos);
	effects.push_back(effect);

//	DSound_Play(bombWAV);
}

bool StepEffect_BOMB(Effect& effect)
{
	EffectAngle(effect);
	if(!EffectAnime(effect)){
		return false;
	}
	return true;
}

//円
void CreateEffect_CIRCLE(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 400;

	const float angleTable[] = {5.f,-5.f,5.f,-5.f,5.f,0};
	const int imgType[] = {0,1,2,3,3,4};
	//円状のエフェクトを5個
	for(int i = 0 ; i < 6 ; i ++){
		CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
			"circle",0,
			EFFECTASZX,EFFECTASZY,0,0.f,1.f,1.f);

		effect.pos.src.x = (float)imgType[i];
		effect.pos.a = 200.f;
		effect.type = type;
		effect.count = 120;
		effect.renderMode = ADD;
		effect.repertDraw = 5;
		effect.animMax = 1;
		effect.animCnt = 0.f;
		effect.animFrm = 0;
		effect.addAngle = angleTable[i];
	
		UseChar(effect.pos);
		effects.push_back(effect);
	}
}

bool StepEffect_CIRCLE(Effect& effect)
{
	EffectAngle(effect);
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}

//円状に広がる
void CreateEffect_DIFFUSE(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = {0};
	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;
	for (int i = 0; i < 8; i++){
    float len = math::GetRandom(4.0f, 9.0f);
    float rad = math::GetRandom(0.0f, 360.0f);
    e = zero;
		//移動方向生成
		//	拡散
		float addx = math::ROUND_X(rad, len);
		float addy = math::ROUND_Y(rad, len);
		//種類
		int no = math::GetRandom(0, 6);
		//集合
		CreateOneEffect(e.pos, (float)x, (float)y, addx, addy,
			"exp", no, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.f, 1.f);
		e.pos.a = 200.0f;
		e.type = type;
		e.renderMode = NORMAL;
		e.repertDraw = 1;
		e.animMax = 7;
		e.animCnt = 0.f;
		e.animFrm = 5;
		e.addAngle = 0.f;

		UseChar(e.pos);
		effects.push_back(e);
	}
	//	DSound_Play(flashWAV);
}

bool StepEffect_DIFFUSE(Effect& effect)
{
	EffectMove(effect);
	if (!EffectAlpha(effect)){
		return false;
	}
	return true;
}

//拡散
void CreateEffect_DIFFUSE2(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = {0};

	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;
	const float len = 4.0f;
	for (int i = 0; i < 4; i++){
		e = zero;
		//移動方向生成
		//	拡散
		float addx = math::GetRandom(-2.f, 2.f);
		float addy = math::GetRandom(-10.f, 3.f);
		//種類
		int no = math::GetRandom(0, 6);
		//集合
		CreateOneEffect(e.pos, (float)x, (float)y, addx, addy,
			"exp", no, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.f, 1.f);
		e.pos.a = 200.0f;
		e.type = type;
		e.renderMode = ADD;
		e.repertDraw = 1;
		e.animMax = 7;
		e.animCnt = 0.f;
		e.animFrm = 5;
		e.addAngle = 0.f;

		UseChar(e.pos);
		effects.push_back(e);
	}
	//	DSound_Play(flashWAV);
}

bool StepEffect_DIFFUSE2(Effect& effect)
{
	effect.pos.add.y += 0.3f;
	if (!EffectMove(effect)){
		return false;
	}
	//	if (!EffectAlpha(effect)){
	//		return false;
	//	}
	return true;
}

//プロミネンス利用P1　収束
void CreateEffect_P1(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = {0};

	const int EFFECTASZX = 256;
	const int EFFECTASZY = 256;

	const float len = 4.0f;
	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
		"effect1_beam", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.f, 1.f);
	e.pos.a = 200.0f;
	e.type = type;
	e.renderMode = ADD;
	e.repertDraw = 3;
	e.animMax = 200;
	e.animCnt = 0.f;
	e.animFrm = 1;
	e.addAngle = 0.f;

	//プロミネンス専用
	e.no = 0;
	e.resWSize = 10;

	UseChar(e.pos);
	effects.push_back(e);
}

//プロミネンス
//Stepカスタマイズ
bool StepEffect_P1(Effect& effect)
{
	effect.animCnt++;
	if (effect.animCnt >= effect.animFrm){
		effect.animCnt = 0;
		effect.no++;
		effect.pos.src.x = (float)(effect.no % effect.resWSize);
		effect.pos.src.y = (float)(effect.no / effect.resWSize);
		if (effect.no == effect.animMax){
			return false;
		}
	}

	return true;
}

//プロミネンス利用P2　攻撃
void CreateEffect_PATTACK(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 512;
	const int EFFECTASZY = 512;

	const float len = 4.0f;
	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
		"effect2_hit", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 0.5f, 0.5f);
	e.pos.a = 200.0f;
	e.type = type;
	e.renderMode = ADD;
	e.repertDraw = 3;
	e.animMax = 30;
	e.animCnt = 0.f;
	e.animFrm = 1;
	e.addAngle = 0.f;

	//プロミネンス専用
	e.no = 0;
	e.resWSize = 10;

	UseChar(e.pos);
	effects.push_back(e);
}

//プロミネンス
//Stepカスタマイズ
bool StepEffect_PATTACK(Effect& effect)
{
	effect.animCnt++;
	if (effect.animCnt >= effect.animFrm){
		effect.animCnt = 0;
		effect.no++;
		effect.pos.src.x = (float)(effect.no % effect.resWSize);
		effect.pos.src.y = (float)(effect.no / effect.resWSize);
		if (effect.no == effect.animMax){
			return false;
		}
	}

	return true;
}


//プロミネンス利用P3 爆発
void CreateEffect_PBOMB(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 256;
	const int EFFECTASZY = 256;

	const float len = 4.0f;
	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
		"effect3_bomb1", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 0.5f, 0.5f);
	e.pos.a = 200.0f;
	e.type = type;
	e.renderMode = ADD;
	e.repertDraw = 3;
	e.animMax = 30;
	e.animCnt = 0.f;
	e.animFrm = 1;
	e.addAngle = 0.f;

	//プロミネンス専用
	e.no = 0;
	e.resWSize = 10;

	UseChar(e.pos);
	effects.push_back(e);
}

//プロミネンス
//Stepカスタマイズ
bool StepEffect_PBOMB(Effect& effect)
{
	effect.animCnt++;
	if (effect.animCnt >= effect.animFrm){
		effect.animCnt = 0;
		effect.no++;
		effect.pos.src.x = (float)(effect.no % effect.resWSize);
		effect.pos.src.y = (float)(effect.no / effect.resWSize);
		if (effect.no == effect.animMax){
			return false;
		}
	}

	return true;
}


//プロミネンス利用P4　ダッシュ
void CreateEffect_PDASH(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 256;
	const int EFFECTASZY = 256;

	const float len = 4.0f;
	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
		"effect4_dash", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.0f, 1.0f);
	e.pos.a = 200.0f;
	e.type = type;
	e.renderMode = ADD;
	e.repertDraw = 3;
	e.animMax = 30;
	e.animCnt = 0.f;
	e.animFrm = 1;
	e.addAngle = 0.f;

	//プロミネンス専用
	e.no = 0;
	e.resWSize = 10;

	UseChar(e.pos);
	effects.push_back(e);
}

//プロミネンス
//Stepカスタマイズ
bool StepEffect_PDASH(Effect& effect)
{
	effect.animCnt++;
	if (effect.animCnt >= effect.animFrm){
		effect.animCnt = 0;
		effect.no++;
		effect.pos.src.x = (float)(effect.no % effect.resWSize);
		effect.pos.src.y = (float)(effect.no / effect.resWSize);
		if (effect.no == effect.animMax){
			return false;
		}
	}

	return true;
}

//フェードアウト　黒
void CreateEffect_FadeInBlack(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 10;
	const int EFFECTASZY = 10;

	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
									"", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 
									(float)gplib::system::WINW / (float)EFFECTASZX, (float)gplib::system::WINH / (float)EFFECTASZY);
	e.pos.a = 255.0f;
	e.type = type;

	UseChar(e.pos);
	effects.push_back(e);
}

bool StepEffect_FadeInBlack(Effect& effect)
{
  charabase::CharaBase& pos = effect.pos;
  //おおよそ30フレームで消滅
  pos.a -= 255.f * system::FrameTime*2.f;
  if (pos.a <= 0.f){
    return false;
  }
  return true;
}

//フェードアウト　白
void CreateEffect_FadeInWhite(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 10;
	const int EFFECTASZY = 10;

	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
									"", 0, EFFECTASZX, EFFECTASZY, 0, 0.f,
									(float)gplib::system::WINW / (float)EFFECTASZX, (float)gplib::system::WINH / (float)EFFECTASZY);
	e.pos.a = 255.0f;
	e.type = type;

	UseChar(e.pos);
	effects.push_back(e);
}

bool StepEffect_FadeInWhite(Effect& effect)
{
  charabase::CharaBase& pos = effect.pos;
  //おおよそ30フレームで消滅
  pos.a -= 255.f * system::FrameTime*2.f;
  if (pos.a <= 0.f){
    return false;
  }
  return true;
}
//フェードアウト　黒
void CreateEffect_FadeOutBlack(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 10;
	const int EFFECTASZY = 10;

	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
									"", 0, EFFECTASZX, EFFECTASZY, 0, 0.f,
									(float)gplib::system::WINW / (float)EFFECTASZX, (float)gplib::system::WINH / (float)EFFECTASZY);
	e.pos.a = 0.f;
	e.type = type;

	UseChar(e.pos);
	effects.push_back(e);
}

bool StepEffect_FadeOutBlack(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//おおよそ30フレームで消滅
  pos.a += 255.f * system::FrameTime*2.f;
  if (pos.a >= 60){
		return false;
	}
	return true;
}

//フェードイン　しろ
void CreateEffect_FadeOutWhite(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = { 0 };

	const int EFFECTASZX = 10;
	const int EFFECTASZY = 10;

	e = zero;
	CreateOneEffect(e.pos, (float)x, (float)y, 0, 0,
									"", 0, EFFECTASZX, EFFECTASZY, 0, 0.f,
									(float)gplib::system::WINW / (float)EFFECTASZX, (float)gplib::system::WINH / (float)EFFECTASZY);
	e.pos.a = 0.f;
	e.type = type;

	UseChar(e.pos);
	effects.push_back(e);
}

bool StepEffect_FadeOutWhite(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//おおよそ30フレームで消滅
  pos.a += 255.f * system::FrameTime*2.f;
	if (pos.a >= 255.f){
		return false;
	}
	return true;
}


//TEROP
void CreateEffect_TEROP(int x, int y, effect::EFFECTTYPE type)
{
  Effect e;
  Effect zero = { 0 };

  const int EFFECTASZX = 1280;
  const int EFFECTASZY = 128;

  const float len = 4.0f;
  e = zero;
  CreateOneEffect(e.pos, (float)system::WINW + EFFECTASZX, (float)y, 0, 0,
    "stage_start", 0, EFFECTASZX, EFFECTASZY, 0, 0.f, 2.0f, 2.0f);
  e.pos.a = 255.0f;
  e.pos.r = 150.0f;
  e.pos.g = 150.0f;
  e.pos.b = 255.0f;
  e.pos.add.x = ((float)(-2600 - system::WINW) / 180.0f);
  e.type = type;
  e.renderMode = ADD;
  e.repertDraw = 1;

  UseChar(e.pos);
  effects.push_back(e);
}

//Stepカスタマイズ
bool StepEffect_TEROP(Effect& effect)
{
  effect.pos.pos.x += effect.pos.add.x;
  if (effect.pos.pos.x <= -1280){
    return false;
  }

  return true;
}

struct EffectFunc{
	create	create;
	step	step;
}EffectFuncTable[] = {
	{ CreateEffect_WARNING, StepEffect_WARNING },
	{ CreateEffect_WARNINGSHAD, StepEffect_WARNINGSHAD },
	{CreateEffect_MYST,StepEffect_MYST},
	{CreateEffect_SPLASH,StepEffect_SPLASH},
	{CreateEffect_BOMB,StepEffect_BOMB},
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
	{ CreateEffect_FadeInWhite, StepEffect_FadeInWhite },
  { CreateEffect_TEROP, StepEffect_TEROP },
};

void effect::Create(int x, int y, effect::EFFECTTYPE type)
{
	EffectFuncTable[(int)type].create(x,y,type);
}

void effect::Init()
{
	effects.clear();
}

void effect::Draw()
{
	for (auto& effect : effects){
    if (effect.type == effect::EFFECTTYPE::FADEINBLACK || effect.type == effect::EFFECTTYPE::FADEOUTBLACK){
      gplib::graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.f, 
        ARGB((int)effect.pos.a, 0, 0, 0), ARGB((int)effect.pos.a, 0, 0, 0), 1, 1);
    }
    else if (effect.type == effect::EFFECTTYPE::FADEINWHITE || effect.type == effect::EFFECTTYPE::FADEOUTWHITE){
      gplib::graph::Draw_Box(0, 0, system::WINW, system::WINH, 0.f, 
        ARGB((int)effect.pos.a, 255, 255, 255), ARGB((int)effect.pos.a, 255, 255, 255), 1, 1);
    }
    else{
      graph::Draw_SetRenderMode(effect.renderMode);
      if (effect.repertDraw <= 0){
        effect.repertDraw = 1;
      }
      for (int i = 0; i < effect.repertDraw; i++){
        Draw_Char(effect.pos);
      }
      graph::Draw_EndRenderMode();
    }
  }
}

static bool CheckUseEffect(Effect effect){ return !CheckUseChar(effect.pos);}

void effect::Step()
{
	for (auto& effect : effects){
		if(!EffectFuncTable[(int)effect.type].step(effect)){
			NoUseChar(effect.pos);
		}
	}
	auto it = remove_if(effects.begin(), effects.end(), CheckUseEffect);
	effects.erase(it, effects.end());
}

void effect::LoadReses()
{
	graph::Draw_LoadObject("warning","res/gra/effect/warning.png");
	graph::Draw_LoadObject("circle", "res/gra/effect/circle.png");
	graph::Draw_LoadObject("myst", "res/gra/effect/myst.png");
	graph::Draw_LoadObject("explode", "res/gra/effect/explode.png");
	graph::Draw_LoadObject("exp", "res/gra/effect/exp.png");
	
	graph::Draw_LoadObject("effect1_beam", "res/gra/effect/effect1_beam.png");
	graph::Draw_LoadObject("effect2_hit", "res/gra/effect/effect2_hit.png");
	graph::Draw_LoadObject("effect3_bomb1", "res/gra/effect/effect3_bomb1.png");
	graph::Draw_LoadObject("effect4_dash", "res/gra/effect/effect4_dash.png");

  graph::Draw_LoadObject("stage_start", "res/gra/effect/stage_start.png");

}




