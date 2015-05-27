
//**************************************************************************
//アニメーションコントローラ補助クラス
//基本型　：　なし
//派生型　：　なし
//**************************************************************************

#pragma once

#include "dx.h"

//アニメーションコントローラ補助クラス
class Dx_AnimeAuxiliary
{
	private:
		//アニメーションコントローラ
		LPD3DXANIMATIONCONTROLLER lpAnimCont;

		//登録トラック数
		short max_num_track;

	public:
		//コンストラクタ
		Dx_AnimeAuxiliary();
		Dx_AnimeAuxiliary(LPD3DXANIMATIONCONTROLLER lpAnimCont);

		//アニメーション制御
		void PlayAnimation(double speed);

		//アニメーションコントローラの設置
		void SetAnimController(LPD3DXANIMATIONCONTROLLER lpAnimCont);

		//トラック情報の設置
		void SetTrackDesc(short no,D3DXTRACK_DESC desc);
		//トラック情報の取得
		void GetTrackDesc(short no,LPD3DXTRACK_DESC desc);
		//アニメーションの切り替え
		void ChangeAnim(int ID);
		//アニメーションの終了チェック
		BOOL CheckAnimEnd(int ID);

};
