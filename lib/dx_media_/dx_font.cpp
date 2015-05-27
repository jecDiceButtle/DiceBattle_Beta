
//**************************************************************************
//フォント管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************
#include "dx.h"

#include "dx_font.h"


//**************************************************************************
//デフォルト設定
//**************************************************************************
//デフォルトフォント数
int Dx_Font::default_font_val = 17;
//デフォルトフォント高さ
int Dx_Font::default_height = 40;
//デフォルトフォント郡
FontType Dx_Font::default_type[17] = {

		"ＭＳ Ｐゴシック",			// 0
		"ＭＳ Ｐ明朝",				// 1
		"ＤＦＰ特太ゴシック体",     // 2
		"ＤＦ特太ゴシック体",       // 3
		"ＤＦＰPOP体",				// 4
		"HGP教科書体",              // 5
		"HGP行書体",                // 6
		"HGP創英角ﾎﾟｯﾌﾟ体",         // 7
		"HGS教科書体",              // 8
		"HGS行書体",                // 9
		"HG丸ｺﾞｼｯｸM-PRO",           // 10
		"HG教科書体",               // 11
		"HG行書体",                 // 12
		"HG正楷書体-PRO",           // 13
		"ＭＳ ゴシック",            // 14
		"ＭＳ 明朝",                // 15
		"HGP創英角ｺﾞｼｯｸUB"			// 16
};


//コンストラクタ
Dx_Font::Dx_Font(LPDIRECT3DDEVICE9 dev,short def_height)
{
	//デバイスの取得
	this->dev = dev;
	//登録フォント数
	this->font_val = 0;

	if(def_height!=0)
	{
		this->default_height = def_height;
	}

	//デフォルトフォントをロード
	this->DefaultLoad();

	//フォーマットの設定
	this->SetDefaultFormat();
}
//デストラクタ
Dx_Font::~Dx_Font()
{
	for(short i=0;i<this->font_val;i++)
	{
		SAFE_RELEASE(this->font[i]);
	}
	this->font.clear();
}

//**************************************************************************
//フォーマット関連
//**************************************************************************
//フォーマットの設置
void Dx_Font::SetFormat(DWORD format)
{
	this->format = format;
}
//デフォルトフォーマットを設定
void Dx_Font::SetDefaultFormat()
{
	this->SetFormat( DT_LEFT | DT_VCENTER );
}

//デフォルトフォントのロード
void Dx_Font::DefaultLoad()
{
	for(int i=0;i<this->default_font_val;i++)
	{
		this->CreateDxFont((LPCTSTR)this->default_type[i],this->default_height,FW_HEAVY);
	}
}

//**************************************************************************
//フォント生成関連
//**************************************************************************
//フォントの生成
void Dx_Font::CreateDxFont(LPCTSTR font_type,UINT height,UINT weight)
{
	if(this->font_val<20)
	{
		LPD3DXFONT create;

		// フォントの作成
		D3DXCreateFont(this->dev,			//LPDIRECT3DDEVICE9	pD3DDevice,
			height,							//UINT				Height,
			0,								//UINT				Width,
			weight,							//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			SHIFTJIS_CHARSET,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			font_type,						//LPCTSTR			pFacename,
			&create							//LPD3DXFONT		*ppFont		&pD3DXFont
		);
		//フォント郡に登録
		this->font.push_back(create);
		this->font_val++;
	}
}

//**************************************************************************
//フォント描画関連
//**************************************************************************
//フォント描画
void Dx_Font::DrawString(LPD3DXSPRITE sprite,int x,int y,int w,int h,LPCSTR string,D3DCOLOR color,short font_no)
{
	LPD3DXFONT select_font = NULL;

	//指定ナンバーのフォントが存在する場合
	if(font_no < this->font_val)
	{
		//選択されたフォントを取得
		select_font = this->font[font_no];
	}
	//ない場合にはデフォルトフォントを使用する
	else
	{
		select_font = this->font[0];
	}

	//表示領域の定義
	RECT	rt = {x,y,x+w,y+h};
	//フォント描画関数
	select_font->DrawText(sprite,string,-1,&rt,this->format,color);

	//デフォルトフォーマットを設定
	this->SetDefaultFormat();
}
