

#include "gplib.h"
#include "CharBase.h"
#include "ci_ext/Polygon.hpp"

using namespace gplib;

//------------------------------------------------------
//CharBase利用時汎用処理
//----------------------
//アニメーション設定
//max : ０に戻る枚数
//speed : アニメ速度
void charabase::SetAnim(Anim& anim, int max, float speed)
{
	anim.cnt = 0.f;
	anim.max = max;
	anim.speed = speed;
	anim.no = 0;
}

//アニメ更新
//この関数はnoの値を更新する。noの値は
//CharBase.src.xに代入する。
//アニメの順序を変更したい場合は以下
/*
	StepAnim(obj.anim);
	//以下でアニメーション指定可能
	int animTable[] = {0,1,0,2};
	//実際に利用する場合は
	animTable[obj.anim.no]
	StepAnim(obj.anim);

*/
bool charabase::StepAnim(Anim& anim)
{
	anim.cnt += anim.speed;
	if(anim.cnt >= 1.f){
		anim.cnt = 0;
		anim.no++;
		if(anim.no >= anim.max){
			anim.no = 0;
		}
		return true;
	}
	return false;
}

//----------------------
//アニメーション停止
//speedを0にするだけなので、再開時は再度SetAnimが必要
void charabase::StopAnim(Anim& anim)
{
	anim.speed = 0.f;
}

//----------------------
//アニメーション開始
//speedを指定した値にするだけなので、再開時は再度SetAnimが必要
//anim : アニメーションを行いたいアニメ構造体
//speed : アニメーションの速度
void charabase::StartAnim(Anim& anim,float speed)
{
	anim.speed = speed;
}
//------------------------------------------------------
//CharBase初期化処理
//
/*
//基本となるチップのサイズと利用する画像
BaseData base = {CHARWID,CHARHGT,charIMG};
//初期化　指定外の値は初期値になる。
//この場合はwid,hgt,img以外は初期値
InitCharBase(player.pos,base);

//この場合はwid,hgt,img,x,y以外は初期値
InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

利用する際は可視フラグshowも偽になっているため、
UseCharで可視状態にすること。
*/
void charabase::InitCharBase(CharaBase& cb, const BaseData& base,
						float x,float y,float z,float addx,float addy,float addz,
					  int srcX,int srcY,int srcZ,float angle, float scaleX,float scaleY,float scaleZ,
						float alpha, float red, float green, float blue)
{
	cb.width = base.wid;
	cb.height = base.hgt;
	cb.resname = base.resname;
	
	cb.pos.x = x;
	cb.pos.y = y;
	cb.pos.z = z;
	cb.add.x = addx;
	cb.add.y = addy;
	cb.add.z = addz;
	cb.scale.x = scaleX;
	cb.scale.y = scaleY;
	cb.scale.z = scaleZ;
	cb.angle = 0.f;
	cb.src.x = (float)srcX;
	cb.src.y = (float)srcY;
	cb.src.z = (float)srcZ;
	
	cb.a = alpha;
	cb.r = red;
	cb.g = green;
	cb.b = blue;
	
	cb.show = false;
}

void charabase::InitCharBase(CharaBase& cb, float x, float y, float z, float addx, float addy, float addz,
					const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
					float angle, float scaleX,float scaleY,float scaleZ,
					float alpha, float red, float green, float blue)
{
	cb.pos.x = x;
	cb.pos.y = y;
	cb.pos.z = z;
	cb.add.x = addx;
	cb.add.y = addy;
	cb.add.z = addz;
	cb.scale.x = scaleX;
	cb.scale.y = scaleY;
	cb.scale.z = scaleZ;
	cb.src.x = (float)srcX;
	cb.src.y = (float)srcY;
	cb.src.z = (float)srcZ;
	cb.resname = resname;
	cb.width = w;
	cb.height = h;
	cb.angle = angle;
	cb.a = alpha;
	cb.r = red;
	cb.g = green;
	cb.b = blue;
	cb.show = false;

}

//------------------------------------------------------
//CharBase初期化処理
//すべて０にする。
void charabase::InitCharBase(CharaBase& cb)
{
	CharaBase zeroCharBase = {};
	cb = zeroCharBase;
}


//----------------------
//利用チェック
bool charabase::CheckUseChar(const CharaBase& cb)
{
	return (cb.show == true);
}

//----------------------
//汎用移動
void charabase::MoveChar(CharaBase& cb)
{
	cb.pos.x += cb.add.x;
	cb.pos.y += cb.add.y;
	cb.pos.z += cb.add.z;
}

//----------------------
//汎用消去処理
void charabase::NoUseChar(CharaBase& cb)
{
	cb.show = false;
}

//----------------------
//汎用出現処理
void charabase::UseChar(CharaBase& cb)
{
	cb.show = true;
}

//----------------------
//判定用矩形作成 左上座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x+startx, (int)cb.pos.y + starty,
				(int)cb.pos.x+cb.width - endx,(int)cb.pos.y+cb.height - endy);
}

//----------------------
//判定用矩形作成 中央座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRect(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x - cb.width / 2 + startx, (int)cb.pos.y - cb.height / 2 + starty,
				(int)cb.pos.x+cb.width/2 - endx,(int)cb.pos.y+cb.height/2 - endy);
}

//----------------------
//判定用矩形作成 中央座標基準　拡大率考慮
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRectScales(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(
    &rt,
    int(cb.pos.x - ((float)cb.width*cb.scale.x) / 2.f + (float)startx),
    int(cb.pos.y - ((float)cb.height * cb.scale.y) / 2.f + (float)starty),
    int(cb.pos.x + ((float)cb.width*cb.scale.x) / 2.f - (float)endx),
    int(cb.pos.y + ((float)cb.height * cb.scale.y) / 2.f - (float)endy));
}
//----------------------
//汎用描画処理 中央基準
void charabase::Draw_CharBuruburu(const CharaBase& cb, int range)
{
  int buruburu_x = math::GetRandom(-range, range);
	int buruburu_y = math::GetRandom(-range, range);
	graph::Draw_Graphics(
    cb.pos.x+buruburu_x, cb.pos.y+buruburu_y, cb.pos.z,
    cb.resname,
    cb.width	* (int)(cb.src.x),
    cb.height * (int)(cb.src.y),
    cb.width, cb.height,
    cb.angle,
    cb.scale.x, cb.scale.y,
    (u_char)cb.a, (u_char)cb.r, (u_char)cb.g, (u_char)cb.b);
}

//----------------------
//汎用描画処理 中央基準
void charabase::Draw_Char(const CharaBase& pos)
{
	graph::Draw_Graphics(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				pos.angle,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2(const CharaBase& pos)
{
  graph::Draw_GraphicsLeftTop(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				pos.angle,nullptr,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}


//----------------------
//汎用描画処理 中央基準
void charabase::Draw_CharNC(const CharaBase& pos)
{

	graph::Draw_GraphicsNC(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				pos.angle,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2NC(const CharaBase& pos)
{
	graph::Draw_GraphicsLeftTopNC(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				pos.angle,nullptr,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}
//----------------------
//あたりチェック
bool charabase::HitCheck(const CharaBase& v1, const CharaBase& v2, bool debug_mode)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2,v2);
//デバッグ----------------
  if (debug_mode)
  {
    graph::Draw_CkRect(rt1);
    graph::Draw_CkRect(rt2);
  }
//ここまで----------------
	if(math::RectCheck(&rt1,&rt2) ){
			return true;
	}else{
		return false;
	}
}

//----------------------
//あたりチェック 拡大率考慮
bool charabase::HitCheckScales(const CharaBase& v1, const CharaBase& v2, bool debug_mode)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRectScales(rt1, v1);
	charabase::MakeRectScales(rt2, v2);
	//デバッグ----------------
  if (debug_mode)
  {
    graph::Draw_CkRect(rt1);
    graph::Draw_CkRect(rt2);
  }
  //ここまで----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//枠描画
//  cb_ charbase情報
//  color 色情報（ARGBでも指定可能）
//  lineweight ラインの太さ
void charabase::Draw_Frame(const CharaBase& cb, ci_ext::Color color, int lineweight)
{
  ci_ext::Polygon<float> a(
    ci_ext::Vec3<float>(cb.pos.x, cb.pos.y, cb.pos.z), //pos
    {
    ci_ext::Vec3<float>(-cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb.width / 2.0f, cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb.width / 2.0f, cb.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb.width / 2.0f, -cb.height / 2.0f, 0.0f),
    },//vertex
    cb.angle,//degree
    ci_ext::Vec3<float>(cb.scale.x, cb.scale.y, 1.0f)//scale
    );

    a.draw(color, lineweight);
}
//----------------------
//あたりチェック
bool charabase::HitCheck2(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode)
{
  //ポリゴン作って判定させる
  ci_ext::Polygon<float> a(
    ci_ext::Vec3<float>(cb1.pos.x, cb1.pos.y, cb1.pos.z), //pos
    {
    ci_ext::Vec3<float>(-cb1.width / 2.0f, -cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb1.width / 2.0f, cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb1.width / 2.0f, cb1.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb1.width / 2.0f, -cb1.height / 2.0f, 0.0f),
    },//vertex
    cb1.angle,//degree
    ci_ext::Vec3<float>(cb1.scale.x, cb1.scale.y, 1.0f)//scale
    );

  ci_ext::Polygon<float> b(
    ci_ext::Vec3<float>(cb2.pos.x, cb2.pos.y, cb2.pos.z),
    {
    ci_ext::Vec3<float>(-cb2.width / 2.0f, -cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(-cb2.width / 2.0f, cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb2.width / 2.0f, cb2.height / 2.0f, 0.0f),
    ci_ext::Vec3<float>(cb2.width / 2.0f, -cb2.height / 2.0f, 0.0f),
    },
    cb2.angle,
    ci_ext::Vec3<float>(cb2.scale.x, cb2.scale.y, 1.0f)
    );

  if (debug_mode)
  {
    charabase::Draw_Frame(cb1, 0xFFFF0000, 1);
    charabase::Draw_Frame(cb2, 0xFFFF0000, 1);
  }
  if (a.intersect(b))
  {
    return true;
  }
  return false;
}


//ここまで
//------------------------------------------------------
