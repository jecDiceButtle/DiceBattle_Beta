#pragma once
#include "ci_ext/color.hpp"
namespace charabase{

	//代入可能なベースデータ
	//初期値が0,1.0fのものは除外する。
	struct BaseData{
		int		wid, hgt;		//画像幅高さ
		const std::string 	resname;		//使用イメージ名
	};

	struct Vec3f{
		float x, y, z;
	};

	//
	//
	//
	//
	struct Anim{
		int		no;					//現在のアニメNO　この値は0,1,2,3,4,とカウントするだけ
		float	cnt;				//アニメ更新カウンタ
		float	speed;			//アニメ更新速度
		int		max;				//アニメ枚数　この値にnoが来ると0からカウント
	};

	struct CharaBase
	{
		Vec3f   pos;							//座標
		Vec3f		add;							//移動量
		int			width, height;		//画像幅高さ

		Vec3f   scale;						//拡大率
		Vec3f   src;							//抽出位置
		std::string resname;			//使用イメージ名

		bool		show;							//利用フラグ

		float		angle;						//角度
		float		a;								//アルファ
		float		r, g, b;					//色
	};

	
	//----------------------
	//汎用移動処理
	//cb : 操作したいCharaBase構造体
	extern void MoveChar(CharaBase& cb);
	//----------------------
	//汎用可視判別処理
	//cb : 操作したいCharaBase構造体
	extern bool CheckUseChar(const CharaBase& cb);
	//----------------------
	//汎用消去処理
	//cb : 操作したいCharaBase構造体
	extern void NoUseChar(CharaBase& cb);
	//----------------------
	//汎用出現処理
	//cb : 操作したいCharaBase構造体
	extern void UseChar(CharaBase& cb);
	//----------------------
	//判定用矩形作成 左上座標基準
	//大きさがクリッピングサイズと異なる場合に利用する。
	//startx,starty : 左上点からのオフセット値、値分左上から加算される
	//endx,endy : 右下点からのオフセット値、値分右下から減算される
	extern void MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	//----------------------
	//判定用矩形作成 中央座標基準
	//大きさがクリッピングサイズと異なる場合に利用する。
	//startx,starty : 左上点からのオフセット値、値分左上から加算される
	//endx,endy : 右下点からのオフセット値、値分右下から減算される
	extern void MakeRect(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	//----------------------
	//判定用矩形作成 中央座標基準　拡大率考慮
	//大きさがクリッピングサイズと異なる場合に利用する。
	//startx,starty : 左上点からのオフセット値、値分左上から加算される
	//endx,endy : 右下点からのオフセット値、値分右下から減算される
	extern void MakeRectScales(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
  //----------------------
  //汎用描画処理 中央基準
  //cb : 操作したいCharaBase構造体
  extern void Draw_CharBuruburu(const CharaBase& cb, int range);
  //----------------------
	//汎用描画処理 中央基準
	//cb : 操作したいCharaBase構造体
	extern void Draw_Char(const CharaBase& cb);
	//----------------------
	//汎用描画処理 左上基準
	//cb : 操作したいCharaBase構造体
	extern void Draw_Char2(const CharaBase& cb);
	//----------------------
	//汎用描画処理 中央基準 カメラ概念無視
	//cb : 操作したいCharaBase構造体
	extern void Draw_CharNC(const CharaBase& cb);
	//----------------------
	//汎用描画処理 左上基準 カメラ概念無視
	//cb : 操作したいCharaBase構造体
	extern void Draw_Char2NC(const CharaBase& cb);
	//----------------------
	//あたりチェック 内部でRECTを生成し調べる
	//cb1,cb2 : 判定を調べたいCharaBase構造体
  extern bool HitCheck(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);
	//----------------------
	//あたりチェック 内部でRECTを生成し調べる。拡大率を考慮に入れる
	//cb1,cb2 : 判定を調べたいCharaBase構造体
  extern bool HitCheckScales(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);
  void Draw_Frame(const CharaBase& cb, ci_ext::Color color, int lineweight);
  bool HitCheck2(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);	//------------------------------------------------------
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
	extern void InitCharBase(CharaBase& cb, const BaseData& base,
		float x = 0.f, float y = 0.f, float z = 0.f, float addx = 0.f, float addy = 0.f, float addz = 0.f,
		int srcX = 0, int srcY = 0, int srcZ = 0, float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);

	//------------------------------------------------------
	//CharBase初期化処理
	//
	/*
	//初期化　指定外の値は初期値になる。
	利用する際は可視フラグshowも偽になっているため、
	UseCharで可視状態にすること。
	*/
	extern void	InitCharBase(CharaBase& cb, float x, float y, float z, float addx, float addy, float addz,
		const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
		float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);
	
	//------------------------------------------------------
	//CharBase初期化処理
	//内部構造体を０クリアする
	extern void InitCharBase(CharaBase& cb);
	//----------------------
	//アニメーション設定
	//max : ０に戻る枚数
	//speed : アニメ速度
	extern void SetAnim(Anim& anim, int max, float speed);
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
	extern bool StepAnim(Anim& anim);
	//----------------------
	//アニメーション停止
	//speedを0にするだけなので、再開時は再度SetAnimが必要
	extern void StopAnim(Anim& anim);
	//----------------------
	//アニメーション開始
	//speedを0にするだけなので、再開時は再度SetAnimが必要
	extern void StartAnim(Anim& anim, float speed);


}



