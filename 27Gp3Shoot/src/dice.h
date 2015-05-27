
#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"



#include "../../lib/dx_media_/dx_graphics3d.h"
#include "../../lib/dx_media_/dx3dmeshmanager.h"
#include "../../lib/ci_ext/DrawObj.hpp"
#include "movableObject.h"



namespace game
{

	class Dice : public MovableObject
	{
		//*************************************************//
		//　定数
		//*************************************************//
	private:
		enum DIRECTION{ NORTH, SOUTH, EAST, WEST, CENTER };
		enum TYPE{ GU, CH, PA, DEF };

		//行動ステータス
		enum STATE
		{
			DEAD,			// 死に状態（描画なし動作なし）
			IDOL,			// 待機状態
			MOVE,			// 移動中
			ATTACK,			// 攻撃中
		};

		//描画ステータス
		enum DISPSTATE
		{
			DICE,			// サイコロのみ
			DICE_CHARA,		// サイコロとキャラクター
			CHARA,			// キャラのみ
		};

		//アニメーション関連の定数
		const float OFFSET;
		const int ANIMFRAMES;
		const float VELOCITY = OFFSET / ANIMFRAMES;


		//*************************************************//
		//　変数
		//*************************************************//
	private:
		ci_ext::Vec3f angle;
		ci_ext::Vec3f scale;
		int alpha;
		int rgb; //今は3色まとめておく


		TYPE face[6];
		TYPE defType_;
		STATE state_;										// 行動状態
		DISPSTATE dispstate_;								// 描画状態
		bool selected_;										// 選択状態

		int playerID_;
		int animcnt_;										// アニメカウンタ

		bool RGBFLAG;


		ci_ext::Vec3f prepos_;		//関数化_
		//今使っていない
		//float nowpos_y;
		//float nowpos_x;
		//D3DXVECTOR3 nowpos_;
		ci_ext::Vec3f nextpos_;		//関数化_


		//Qauternion
		D3DXQUATERNION middleQ;
		D3DXQUATERNION startQ;
		D3DXQUATERNION endQ;
		float rollAnim;
		D3DXMATRIX matRot;

		DIRECTION hougaku_; //関数化_


		//追加
		std::weak_ptr<ci_ext::Object> p_mons;			//モンスターのポインタ

		static const TYPE ATKTYPE[6];						// 攻撃属性の初期化	（仮）			

		//*************************************************//
		//　関数
		//*************************************************//
	private:

		/*
		@brief							待機中の処理
		@return							なし
		*/
		void DoIdol();

		/*
		@brief							移動中の処理
		@return							なし
		*/
		void DoMove();

		/*
		@brief							移動中の処理
		@return							なし
		*/
		void Attack();


		/*
		@brief							移動中の処理
		@return							なし
		*/
		void DoDead();


		/*
		@brief							コンストラクタで目の値をセット
		@return							なし
		*/
		void setFace(TYPE ONE_SIX, TYPE TWO_FIVE, TYPE THREE_FOUR);


		void swap(DIRECTION dir);


		//***************//
		//　内部利用関数　//
		//***************//
		//-------------DoMove------------//
		/*
		@brief							移動計算
		@return							なし
		*/
		void MoveDice(STATE &pOut);
		void ClacMove(ci_ext::Vec3f &pOut_nowpos, const DIRECTION &hougaku);
		void setStartQuaternion(D3DXQUATERNION &out_end, const D3DXQUATERNION &in_middle, const DIRECTION &hougaku);
		//-------------------------------//
		//-------------DoIdol------------//
		/*
		@brief							移動計算
		@return							なし
		*/
		void change_rgb();
		//-------------------------------//
		//----------prepareMove----------//
		/*
		@brief							移動方向フラグをセット
		@return							DIRECT
		*/
		DIRECTION setHougaku(const ci_ext::Vec3f &pos);
		/*
		@brief							移動方向フラグをセット
		@return							DIRECT
		*/
		ci_ext::Vec3f MasuToPos(const ci_ext::Vec3i &masu);
		//-------------------------------//


		void setDicePosX(const ci_ext::Vec3i &masu);
		void setDicePosY(const ci_ext::Vec3i &masu);




		

		//***************/
		//　 関数化変数 　//
		//***************//
		//----------DIRECTION関数----------//
		//@brief							デフォルトDIRECTON関数
		DIRECTION hougaku();
		//@brief							移動方向フラグをセット
		DIRECTION hougaku(const DIRECTION &in_dir);
		//---------------------------------//
		//-----------nextpos関数-----------//
		//@brief							デフォルトnextpos関数
		ci_ext::Vec3f nextpos();
		//@brief							nextposをセット
		ci_ext::Vec3f nextpos(const ci_ext::Vec3f &in_pos);
		//---------------------------------//




	public:
		/*
		@brief							コンストラクタ
		@param[in] objectName			オブジェクト名
		@param[in] type					防御側の種族
		@param[in] playerID				プレイヤー
		@param[in] pos					初期座標
		@return							なし
		*/
		Dice(const std::string& objectName, const int type, const int playerID, const ci_ext::Vec3i& pos = ci_ext::Vec3i::zero());

		void init()	override;
		void render() override;
		void update() override;

		int getTopType();	//使ってるのこれ？

		/*
		@brief							アクセサ、待機中かどうか
		@return							待機中かどうか
		@true							待機中
		*/
		bool isIdoling();


		/*
		@brief							アクセサ、状態の確認
		@return							移動中かどうか
		@true							移動中
		*/
		bool isMoving();

		/*
		@brief							アクセサ、状態の確認
		@return							死亡中かどうか
		@true							死亡中
		*/
		bool isDying();


		/*
		@brief							アクセサ、蘇生
		@return							なし
		*/

		void Spawn();


		/*
		@brief							アクセサ、ダイスを死亡状態へ
		@return							なし
		*/
		void destroy();


		/*
		@brief							アクセサ、攻撃属性のチェック
		@return							属性値
		*/
		int getAtkSpecies();

		/*
		@brief							アクセサ、防御属性のチェック
		@return							属性値
		*/
		int getDefSpecies();

		/*
		@brief							移動を準備（移動に必要な値等をセット）
		@return							なし
		*/
		void prepareMove(ci_ext::Vec3i &masu);

		/*
		@brief							selectflagの切り替え
		@return							なし
		*/
		void OnSelectFlag();
		void OffSelectFlag();

	};

}