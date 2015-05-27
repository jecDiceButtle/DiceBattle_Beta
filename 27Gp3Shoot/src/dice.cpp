

#include "dice.h"
#include "../../lib/gplib.h"
//tuika
#include "monster.h"


#include "movableObject.h"

//3Dカメラ
extern LPDIRECT3DDEVICE9 pD3DDevice;
//メッシュ全体管理クラス
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;




namespace game
{
	using namespace ci_ext;
	using namespace gplib;


	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//

	const Dice::TYPE Dice::ATKTYPE[6] =
	{
		TYPE::GU, TYPE::CH, TYPE::PA, TYPE::PA, TYPE::CH, TYPE::GU,
	};


	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//


	//===================================
	//	デフォルト関数
	//===================================
	Dice::Dice(const std::string& objectName, const int type, const int playerID, const ci_ext::Vec3i& masu)
		:
		MovableObject(
		DrawObjf(objectName)
		),
		angle(0, 0, 0),
		scale(10.f, 10.f, 10.f),
		alpha(255),
		rgb(255),
		OFFSET(10.0f),

		state_(IDOL),
		dispstate_(DICE),
		selected_(false),
		defType_((TYPE)type),
		playerID_(playerID),

		ANIMFRAMES(30),
		hougaku_(CENTER),
		prepos_(0.f, 0.f, 0.f),
		nextpos_(0.f, 0.f, 0.f),

		RGBFLAG(false),

		animcnt_(0),

		middleQ(0.f, 0.f, 0.f, 1.f),
		startQ(0.f, 0.f, 0.f, 1.f),
		endQ(0.f, 0.f, 0.f, 1.f),
		rollAnim(0)


	{
		setDicePosX(masu);
		setDicePosY(masu);
		pos_.y(5.f);
		setFace(GU, CH, PA);
		
		matRot._11 = 1;
		matRot._12 = 0;
		matRot._13 = 0;
		matRot._14 = 0;
		matRot._21 = 0;
		matRot._22 = 1;
		matRot._23 = 0;
		matRot._24 = 0;
		matRot._31 = 0;
		matRot._32 = 0;
		matRot._33 = 1;
		matRot._34 = 0;
		matRot._41 = 0;
		matRot._42 = 0;
		matRot._43 = 0;
		matRot._44 = 1;


	}

	bool Dice::isDying()
	{
		return state_ == DEAD;
	}
	
	bool Dice::isMoving()
	{
		return state_ == MOVE;
	}

	void Dice::destroy()
	{
		state_ = DEAD;
	}

	void Dice::Spawn()
	{
		state_ = IDOL;
	}

	int Dice::getAtkSpecies()
	{
		return static_cast<int>(face[0]);
	}
	int Dice::getDefSpecies()
	{
		return static_cast<int>(defType_);
	}


	//render()
	void Dice::render()
	{

		meshManage->drawMeshQuaternion(pos_, "dice", angle, ARGB(255, rgb, rgb, rgb), scale, matRot);

		
	}

	//update
	void Dice::update()
	{
		/*getDicePosY();
		getDicePosX();*/

		switch (state_)
		{
		case game::Dice::DEAD:
			DoDead();
			break;
		case game::Dice::IDOL:
			pos_.y(5.0f);
			DoIdol();
			break;

		case game::Dice::MOVE:
			DoMove();
			break;

		case game::Dice::ATTACK:
			Attack();
			break;
		}

		//追加
		auto monsobj = ci_ext::weak_to_shared<Monster>(p_mons);
		monsobj->monster_move(pos_);

	}
	//===================================


	//===================================
	//	DoMove
	//===================================
	void Dice::DoMove()
	{
		if (state_ != MOVE) return;

		MoveDice(state_);

	}

	//------------DoMove-------------//
	/*
	@brief							移動計算
	@return							なし
	*/
	void Dice::MoveDice(Dice::STATE &pOut)
	{

		if (animcnt_ > ANIMFRAMES)
		{
			pos_.x(nextpos().x());
			pos_.z(nextpos().z());

			middleQ = endQ;
			startQ = endQ;

			pOut = IDOL;
			animcnt_ = 0;
			rollAnim = 0;

		}

		else
		{
			animcnt_++;
			ClacMove(pos_, hougaku());
			rollAnim += 1.0f / (float)ANIMFRAMES;
			D3DXQuaternionSlerp(&middleQ, &startQ, &endQ, rollAnim);
			pOut = MOVE;

		}

		D3DXMatrixRotationQuaternion(&matRot, &middleQ);

		return;
	}

	//------------MoveDice------------//
	/*
	@brief							移動計算
	@return							なし
	*/
	void Dice::ClacMove(ci_ext::Vec3f &pOut_nowpos, const Dice::DIRECTION &hougaku)
	{

		float temp_pos_x = pOut_nowpos.x();
		float temp_pos_z = pOut_nowpos.z();


		switch (hougaku)
		{
		case NORTH:
			temp_pos_z += VELOCITY;
			break;

		case SOUTH:
			temp_pos_z -= VELOCITY;
			break;

		case EAST:
			temp_pos_x += VELOCITY;
			break;

		case WEST:
			temp_pos_x -= VELOCITY;
			break;

		case CENTER:
			break;
		default:
			break;
		}

		pOut_nowpos.x(temp_pos_x);
		pOut_nowpos.z(temp_pos_z);

	}
	//-------------------------------//
	/*
	@brief							クォータニオン初期化
	@return							なし
	*/
	void Dice::setStartQuaternion(D3DXQUATERNION &out_end, const D3DXQUATERNION &in_middle, const Dice::DIRECTION &hougaku)
	{
		D3DXVECTOR3 axis;
		D3DXQUATERNION calcQ;

		if (hougaku == NORTH)
		{
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
			D3DXVec3Normalize(&axis, &axis);
			D3DXQuaternionRotationAxis(&calcQ, &axis, D3DXToRadian(90));

			out_end = in_middle * calcQ;

		}

		if (hougaku == SOUTH)
		{
			axis.x = -1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
			D3DXVec3Normalize(&axis, &axis);
			D3DXQuaternionRotationAxis(&calcQ, &axis, D3DXToRadian(90));

			out_end = in_middle * calcQ;
		}

		if (hougaku == WEST)
		{
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = 1.0f;
			D3DXVec3Normalize(&axis, &axis);
			D3DXQuaternionRotationAxis(&calcQ, &axis, D3DXToRadian(90));

			out_end = in_middle * calcQ;

		}

		if (hougaku == EAST)
		{
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = -1.0f;
			D3DXVec3Normalize(&axis, &axis);
			D3DXQuaternionRotationAxis(&calcQ, &axis, D3DXToRadian(90));

			out_end = in_middle * calcQ;
		}
	}
	//-------------------------------//
	//===================================


	//===================================
	//	DoIdol
	//===================================
	void Dice::DoIdol()
	{
		if (selected_)
		{
			change_rgb();
		}
		else
		{
			rgb = 255;
		}
	}

	//------------DoIdol-------------//
	void Dice::change_rgb()
	{
		if (RGBFLAG == false)
		{
			rgb -= 5;
			if (rgb < 200)
			{
				rgb = 200;
				RGBFLAG = true;
			}
		}
		if (RGBFLAG == true)
		{
			rgb += 5;
			if (rgb > 255)
			{
				rgb = 255;
				RGBFLAG = false;
			}
		}
	}
	//-------------------------------//
	//===================================




	//===================================
	//	Attack
	//===================================
	//-------------Attack------------//
	void Dice::Attack()
	{

	}
	//-------------------------------//
	//===================================

	//===================================
	//	DoMove
	//===================================
	void Dice::DoDead()
	{
		pos_.y(255); //死んでる間は吹っ飛ばす
		//monster_move
		//auto monsobj = ci_ext::weak_to_shared<Monster>(p_mons);
		//monsobj->monster_move(pos_);
		//

	}
	//===================================


	//===================================
	//	アクセサ
	//===================================
	/*
	@brief							アクセサ、待機中かどうか
	@return							待機中かどうか
	@true							待機中
	*/
	bool Dice::isIdoling()
	{
		return state_ == IDOL;
	}

	void Dice::OnSelectFlag()
	{
		selected_ = true;
	}
	void Dice::OffSelectFlag()
	{
		selected_ = false;
		rgb = 255;
	}

	/*
	@brief							移動を準備（移動に必要な値等をセット）
	@return							なし
	*/
	void Dice::prepareMove(Vec3i &masu)
	{
		Vec3f this_pos = MasuToPos(masu);


		//移動先方角を準備
		hougaku(setHougaku(this_pos));

		//方角から値をセット
		swap(hougaku());

		//方角からクォータニオンをセット
		setStartQuaternion(endQ, middleQ, hougaku());

		//移動先座標を設定
		nextpos(this_pos);

		//移動ステートに変更
		state_ = MOVE;
	}

	//===================================



	//----------prepareMove----------//
	/*
	@brief							移動方向フラグをセット
	@return							DIRECT
	*/
	Dice::DIRECTION Dice::setHougaku(const Vec3f& p_pos)
	{
		if (pos_.x() == p_pos.x() && pos_.z() == p_pos.z())	return CENTER;

		else if (pos_.x() < p_pos.x()) return EAST;

		else if (p_pos.x() < pos_.x()) return WEST;

		else if (pos_.z() < p_pos.z()) return NORTH;

		else if (p_pos.z() < pos_.z()) return SOUTH;

	}
	//-------------------------------//


	/*
	@brief							マスをポジションに変更
	@return							vec3f (pos)
	*/
	Vec3f Dice::MasuToPos(const Vec3i& masu)
	{
		Vec3f pos = (0.f, 0.f, 0.f);

		pos.x((float)((masu.x() - 2) * 10));
		pos.z((float)((masu.y() - 2) * 10));

		return pos;
	}

	void Dice::init(){

		Vec3f rot = ((playerID_ == 0) ? Vec3f(0.f, 180.f, 0.f): Vec3f(0.f, 0.f, 0.f));

		p_mons = insertAsChild(new Monster("monster", pos_, (int)defType_, rot));

	}

	void Dice::setDicePosX(const ci_ext::Vec3i &masu)
	{
		pos_.x((masu.x() - 2) * 10.f);
	}
	void Dice::setDicePosY(const ci_ext::Vec3i &masu)
	{
		pos_.z((masu.y() - 2) * 10.f);
	}
	int Dice::getTopType(){
		return face[0];
	}



	void Dice::setFace(TYPE ONE_SIX, TYPE TWO_FIVE, TYPE THREE_FOUR)
	{

		face[1 - 1] = ONE_SIX;
		face[6 - 1] = ONE_SIX;

		face[2 - 1] = TWO_FIVE;
		face[5 - 1] = TWO_FIVE;

		face[3 - 1] = THREE_FOUR;
		face[4 - 1] = THREE_FOUR;
	}

	void Dice::swap(DIRECTION dir)
	{

		TYPE temp = face[0];

		if (dir == WEST){
			face[0] = face[2];
			face[2] = face[5];
			face[5] = face[3];
			face[3] = temp;
		}
		if (dir == EAST){
			face[0] = face[3];
			face[3] = face[5];
			face[5] = face[2];
			face[2] = temp;
		}
		if (dir == NORTH){
			face[0] = face[1];
			face[1] = face[5];
			face[5] = face[4];
			face[4] = temp;
		}
		if (dir == SOUTH){
			face[0] = face[4];
			face[4] = face[5];
			face[5] = face[1];
			face[1] = temp;
		}
	}









	//----------DIRECTION関数----------//
	//@brief							デフォルトDIRECTON関数
	Dice::DIRECTION Dice::hougaku()
	{
		return Dice::hougaku_;
	}
	//@brief							DIRECTONをセット
	Dice::DIRECTION Dice::hougaku(const DIRECTION& hougaku_)
	{
		return Dice::hougaku_ = hougaku_;
	}
	//---------------------------------//

	//-----------nextpos関数-----------//
	//@brief							デフォルトnextpos関数
	Vec3f Dice::nextpos()
	{
		return nextpos_;
	}
	//@brief							nextposをセット
	Vec3f Dice::nextpos(const Vec3f& in_pos)
	{
		return nextpos_ = in_pos;
	}
	//---------------------------------//





}





