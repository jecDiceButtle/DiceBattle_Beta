
//**************************************************************************
//アニメーションコントローラ補助クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#include "dx_anime_cont_auxiliary.h"


//コンストラクタ
Dx_AnimeAuxiliary::Dx_AnimeAuxiliary()
{
	//アニメーションコントローラポインタの初期化
	this->lpAnimCont = NULL;
	//最大トラック数の初期化
	this->max_num_track = 0;
}
//コンストラクタ　　（アニメーションコントローラ設置型）
Dx_AnimeAuxiliary::Dx_AnimeAuxiliary(LPD3DXANIMATIONCONTROLLER lpAnimCont)
{
	//アニメーションコントローラポインタの初期化
	this->lpAnimCont = NULL;
	//最大トラック数の初期化
	this->max_num_track = 0;

	this->SetAnimController(lpAnimCont);
}

//アニメーション制御
void Dx_AnimeAuxiliary::PlayAnimation(double speed)
{
	if(this->lpAnimCont!=NULL)
	{
		this->lpAnimCont->AdvanceTime(speed,NULL);
	}
}

//アニメーションコントローラの設置
void Dx_AnimeAuxiliary::SetAnimController(LPD3DXANIMATIONCONTROLLER lpAnimCont)
{
	//
	if(lpAnimCont!=NULL)
	{
		//アニメーションコントローラのアドレスを登録
		this->lpAnimCont = lpAnimCont;

		//登録トラック数を取得
		this->max_num_track = lpAnimCont->GetMaxNumTracks();
	}
}

//トラック情報の設置
void Dx_AnimeAuxiliary::SetTrackDesc(short no,D3DXTRACK_DESC desc)
{
	if(no < this->max_num_track)
	{
		this->lpAnimCont->SetTrackDesc(no,&desc);
	}
}
//トラック情報の取得
void Dx_AnimeAuxiliary::GetTrackDesc(short no,LPD3DXTRACK_DESC desc)
{
	if(no < this->max_num_track)
	{
		this->lpAnimCont->GetTrackDesc(no,desc);
	}
}

//アニメーションの切り替え
void Dx_AnimeAuxiliary::ChangeAnim(int ID)
{
	D3DXTRACK_DESC desc;
	//一度トラックをクリアする
	for(int i = 0 ; i <= this->max_num_track ; i ++)
	{	
		this->lpAnimCont->GetTrackDesc(i,&desc);
		desc.Enable = FALSE;
		desc.Position = 0.0f;
		desc.Speed = 0.0f;
		desc.Weight = 0;
		this->lpAnimCont->SetTrackDesc(i,&desc);
	}
	//その後必要なトラックの追加を行う
	this->lpAnimCont->GetTrackDesc(ID,&desc);
	desc.Enable = TRUE;
	desc.Position = 0.0f;
	desc.Speed = 0.1f;
	desc.Weight = 10;
	this->lpAnimCont->SetTrackDesc(ID,&desc);
	this->lpAnimCont->AdvanceTime(0.0f,NULL);

}

//指定のアニメーションの終了を調べる
BOOL Dx_AnimeAuxiliary::CheckAnimEnd(int ID)
{
	D3DXTRACK_DESC desc;
	this->lpAnimCont->GetTrackDesc(ID,&desc);
	//今のモーションが１ループ終わったら
	if(desc.Position >= 1.0f){
		return TRUE;
	}
	return FALSE;
}
