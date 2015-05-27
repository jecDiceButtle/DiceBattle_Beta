
//**************************************************************************
//テクスチャ管理クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#include "dx.h"
#include "texture_manager.h"


//コンストラクタ
TextureManager::TextureManager(LPDIRECT3DDEVICE9 dev)
{
	//デバイスの登録
	this->dev = dev;

	//登録数の初期化
	this->tex_val = 0;
}
//デストラクタ
TextureManager::~TextureManager()
{
	vector<DxTexture*>::iterator it;

	//vector内のオブジェクトをすべて破棄および開放
	for(it=this->tex_data.begin();it!=this->tex_data.end();it++)
	{
		this->ReleaseTexture((*it));
	}
	this->tex_data.clear();
}

//**************************************************************************
//外部参照用ロード関数
//**************************************************************************
//イメージファイルからDxTextureをロード　　（簡易版）
LPDIRECT3DTEXTURE9 TextureManager::LoadTextureFromFile(const string& fName,const string& path)
{
	//生成用オブジェクト
	DxTexture *create=NULL;

	//絶対パス設定になっている可能性があるので文字列チェックを行う
	string s_f_name;
	for(unsigned i=0;i<(fName.size());i++)
	{
		s_f_name.push_back(fName[i]);

		if(fName[i]=='\\' || fName[i]=='/')
		{
			s_f_name.clear();
		}
	}

	//重複ロード防止のためファイル名チェックを行う
	if((create=this->CheckFileName(s_f_name))!=NULL)
	{
		//登録済みオブジェクトを戻り値とする
		return create->lpTex;
	}

	//オブジェクトの生成
	create = new DxTexture;
	//生成用オブジェクトにパス及びファイル名からロード
	if(FAILED(this->LoadTextureFromFile(create,s_f_name,path)))
	{
		//ロードに失敗した場合、領域確保した情報を破棄
		SAFE_DELETE(create);
		return NULL;
	}

	//テクスチャ郡に登録
	this->tex_data.push_back(create);
	//テクスチャの総数を加算
	this->tex_val++;

	//新たに生成されたオブジェクトを返す
	return create->lpTex;
}
//イメージファイルからDxTextureをロード　　（詳細版）
DxTexture* TextureManager::LoadTextureFromFileEx(const string& fName,const string& path)
{
	//生成用オブジェクト
	DxTexture *create=NULL;

	//絶対パス設定になっている可能性があるので文字列チェックを行う
	string s_f_name;
//	char c_f_name[20]="";
	for(unsigned i=0;i<fName.size();i++)
	{
		s_f_name.push_back(fName[i]);

		if(fName[i]=='\\' || fName[i]=='/')
		{
			s_f_name.clear();
		}
	}

	//重複ロード防止のためファイル名チェックを行う
	if((create=this->CheckFileName(s_f_name))!=NULL)
	{
		//登録済みオブジェクトを戻り値とする
		return create;
	}

	//オブジェクトの生成
	create = new DxTexture;
	//生成用オブジェクトにパス及びファイル名からロード
	if(FAILED(this->LoadTextureFromFileEx(create,s_f_name,path)))
	{
		//ロードに失敗した場合、領域確保した情報を破棄
		SAFE_DELETE(create);
		return NULL;
	}

	//テクスチャ郡に登録
	this->tex_data.push_back(create);
	//テクスチャの総数を加算
	this->tex_val++;

	//新たに生成されたオブジェクトを返す
	return create;
}

//**************************************************************************
//内部参照用ロード関数
//**************************************************************************
//イメージファイルからDxTextureをロード　　（簡易版）
HRESULT TextureManager::LoadTextureFromFile(DxTexture* tex,const string& fName,const string& path)
{
	HRESULT hr=S_OK;
	string  str = path + fName;

	D3DXGetImageInfoFromFile(str.c_str(),&tex->info);

	if(FAILED(D3DXCreateTextureFromFile(this->dev,str.c_str(),&tex->lpTex)))
	{
		return E_FAIL;
	}

	//ファイル名の設置
	tex->file_name += fName;

  return S_OK;
}
//イメージファイルからDxTextureをロード　　（詳細版）
HRESULT TextureManager::LoadTextureFromFileEx(DxTexture* tex,const string& fName,const string& path)
{
	HRESULT hr=S_OK;
	string str = path + fName;

    if(FAILED(D3DXCreateTextureFromFileEx(
		this->dev,				//LPDIRECT3DDEVICE9     pDevice,
		str.c_str(),					//LPCTSTR               pSrcFile,
		D3DX_DEFAULT,			//UINT                  Width,
    D3DX_DEFAULT,			//UINT                  Height,
    D3DX_DEFAULT,			//UINT                  MipLevels,
    0,						//DWORD                 Usage,
    D3DFMT_UNKNOWN,			//D3DFORMAT             Format,
    D3DPOOL_MANAGED,        //D3DPOOL               Pool,
		D3DX_FILTER_NONE,			//DWORD                 Filter,
    D3DX_DEFAULT,			//DWORD                 MipFilter,
		0,						//D3DCOLOR              ColorKey,   //抜け色を設定
		&tex->info,					//D3DXIMAGE_INFO        *pSrcInfo,
		NULL,					//PALETTEENTRY          *pPalette,
		&tex->lpTex				//LPDIRECT3DTEXTURE9    *ppTexture
    ))){
       return E_FAIL;
    }

	//ファイル名の領域確保
	tex->file_name = fName;
	//ファイル名の設置
	tex->file_name = fName;

    return S_OK;
}

//**************************************************************************
//破棄及び開放用関数
//**************************************************************************
//DxTextureに含まれるデータの破棄及び開放
void TextureManager::ReleaseTexture(DxTexture *tex)
{
	//テクスチャの開放
	SAFE_RELEASE(tex->lpTex);

	//DxTexture領域の破棄
	SAFE_DELETE(tex);
	this->tex_val--;
}

//**************************************************************************
//その他の関数
//**************************************************************************
//登録済みテクスチャデータのファイル名をチェックし重複ロードを防止する
DxTexture* TextureManager::CheckFileName(const string& fName)
{
	string str1;
	string str2;
	if(fName.size()>25)
	{
		return NULL;
	}

	str1 += fName;
	for(short i=0;i<this->tex_val;i++)
	{
		str2=this->tex_data[i]->file_name;
		if(str1==str2)
		{
			return this->tex_data[i];
		}
	}

	return NULL;
}

