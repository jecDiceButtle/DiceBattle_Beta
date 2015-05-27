
//**************************************************************************
//フォント管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#pragma once

typedef LPCSTR FontType;

//フォント管理クラス
class Dx_Font
{
	private:
		//デバイス
		LPDIRECT3DDEVICE9	dev;

		//登録フォント郡
		vector<LPD3DXFONT>	font;
		//登録フォント郡に対応するフォント名
		vector<FontType>	font_name;
		//登録フォント数
		short				font_val;

		//テキストフォーマット
		DWORD				format;

		/*デフォルトフォント関連*/
		//デフォルトフォントタイプ郡
		static FontType		default_type[17];
		//デフォルトフォント数
		static int			default_font_val;
		//デフォルトフォント高さ
		static int			default_height;

		//デフォルトフォントのロード
		void DefaultLoad();

	public:
		//コンストラクタ
		Dx_Font(LPDIRECT3DDEVICE9 dev,short def_height=0);
		//デストラクタ
		~Dx_Font();

		//フォーマットの設置
		void SetFormat(DWORD format);
		//デフォルトフォーマットを設定
		void SetDefaultFormat();

		//フォントの生成
		void CreateDxFont(LPCTSTR font_type,UINT height,UINT weight=FW_NORMAL);

		//フォント描画
		void DrawString(LPD3DXSPRITE sprite,int x,int y,int w,int h,LPCSTR string,D3DCOLOR color,short font_no);
};

