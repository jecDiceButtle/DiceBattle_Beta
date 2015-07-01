

#include "dice.h"
#include "../../lib/gplib.h"
//tuika
#include "monster.h"


#include "movableObject.h"

//3D�J����
extern LPDIRECT3DDEVICE9 pD3DDevice;
//���b�V���S�̊Ǘ��N���X
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;




namespace game
{
	using namespace ci_ext;
	using namespace gplib;


	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	//const Dice::TYPE Dice::ATKTYPE[6] =
	//{
	//	TYPE::GU, TYPE::CH, TYPE::PA, TYPE::PA, TYPE::CH, TYPE::GU,
	//};


	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//


	//===================================
	//	�f�t�H���g�֐�
	//===================================
	Dice::Dice(const std::string& objectName, const int type, const int playerID, const ci_ext::Vec3i& masu)
		:
		MovableObject(
		DrawObjf(objectName)
		),
		//-------------�萔------------//
		playerID_(playerID),
		defType_((TYPE)type),
		/*p_mons*/
		OFFSET(10.0f),

		middleQ(0.f, 0.f, 0.f, 1.f),
		startQ(0.f, 0.f, 0.f, 1.f),
		endQ(0.f, 0.f, 0.f, 1.f)
		


	{
		//-------------init------------//
		/*playerID_*/
		/*OFFSET*/
		/*defType_*/
		/*p_mons*/

		//-------------render------------//
		scale.set(10.f, 10.f, 10.f);
		if (playerID_ == 0)
		{
			rgb.set(150, 150, 250);
		
		}
		else
		{
			rgb.set(250, 150, 150);

		}

		//-------------update------------//
		frames_ = 0;
		state_anim = WAIT;
		state_do = DEFAULT;
		selected_ = false;
		RGBFLAG = false;
		velocity_.set(0.f, 0.f, 0.f);
		nowpos_.set((MasuToPos(masu).x()), 5.0f, (MasuToPos(masu).z()));
		nextpos_.set(nowpos_);

		//Qauternion
		/*middleQ*/
		/*startQ*/
		/*endQ*/
		frames_roll = 1.0f;
		velocity_roll = 0.f;
		D3DXMatrixIdentity(&matRot);

		//-------------�p�����[�^�[------------//
		setFace(GU, CH, PA);
		/*defType_*/
		/*nowpos_*/
	}


	//render()
	void Dice::render()
	{

		meshManage->drawMeshQuaternion(nowpos_, "dice", D3DXVECTOR3(0, 0, 0), ARGB(255, rgb.ix(), rgb.iy(), rgb.iz()), scale, matRot);

		
	}

	//update
	void Dice::update()
	{
		switch (state_anim)
		{
		case ANIMATION:
			
			switch (state_do)
			{
			case SUMMON:
				Do_Summon();
				break;
			case PARALLEL:
				Do_Parallel();
				break;
			case ROTATION:
				Do_Rotation();
				break;
			case FALL:
				Do_Fall();
				break;
			case BATTLE:
				break;
			}

			break;

		case END:
			End();
			break;

		case WAIT:
		
			break;
		}

		if (selected_)
		{
			Do_Select();
		}

		ci_ext::weak_to_shared<Monster>(p_mons)->monster_move(nowpos_);
	}


	void Dice::receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg)
	{
		//���b�Z�[�W����
		auto msgVec = gplib::text::split(msg, ",");

		//�ړ��̂Ƃ��̏���
		if (msgVec[0] == "move")
		{
			ci_ext::Vec3i masu = ci_ext::Vec3i::zero();		//�ړ���̃}�X���W
			int		fTime;									//�ړ��t���[������

			//�X�ɕ���
			for (auto ms : msgVec){
				auto mVec = gplib::text::split(ms, "=");
				
				//X�}�X���W���擾
				if (mVec[0] == "x" && mVec.size() > 1)
					masu.x(stoi(mVec[1]));

				//Z�}�X���W���擾
				if (mVec[0] == "z" && mVec.size() > 1)
					masu.z(stoi(mVec[1]));

				//�t���[�����Ԃ��擾
				if (mVec[0] == "frame" && mVec.size() > 1)
					fTime = stoi(mVec[1]);
			}

			state_do = ROTATION;
			Calc_Rotation(masu, fTime);
		}


		//�I������
		if (msgVec[0] == "select")
		{
			if (msgVec[1] == "on")
				OnSelectFlag();
			else if(msgVec[1] == "off")
				OffSelectFlag();

		}

		//�����ꏈ��
		if (msgVec[0] == "push"){
			//state_do = PARALLEL;
			//Calc_Parallel(masu, fTime);
			ci_ext::Vec3i masu = ci_ext::Vec3i::zero();		//�ړ���̃}�X���W
			int		fTime;									//�ړ��t���[������

			//�X�ɕ���
			for (auto ms : msgVec){
				auto mVec = gplib::text::split(ms, "=");

				//X�}�X���W���擾
				if (mVec[0] == "x" && mVec.size() > 1)
					masu.x(stoi(mVec[1]));

				//Z�}�X���W���擾
				if (mVec[0] == "z" && mVec.size() > 1)
					masu.z(stoi(mVec[1]));

				//�t���[�����Ԃ��擾
				if (mVec[0] == "frame" && mVec.size() > 1)
					fTime = stoi(mVec[1]);
			}

			state_do = PARALLEL;
			Calc_Parallel(masu, fTime);
		}

		//��������
		if (msgVec[0] == "fall"){
			int		fTime;
		for (auto ms : msgVec){

			auto mVec = gplib::text::split(ms, "=");

			//�t���[�����Ԃ��擾
			if (mVec[0] == "frame" && mVec.size() > 1)
				fTime = stoi(mVec[1]);
			}
			state_do = FALL;
			Calc_Fall(fTime);
		}
			

		/////////////////////////////6/09�ǉ�
		if (msgVec[0] == "summon")
		{
			ci_ext::Vec3i masu = ci_ext::Vec3i::zero();		//�ړ���̃}�X���W
			int		fTime;									//�ړ��t���[������

			//�X�ɕ���
			for (auto ms : msgVec){
				auto mVec = gplib::text::split(ms, "=");

				//X�}�X���W���擾
				if (mVec[0] == "x" && mVec.size() > 1)
					masu.x(stoi(mVec[1]));

				//Z�}�X���W���擾
				if (mVec[0] == "z" && mVec.size() > 1)
					masu.z(stoi(mVec[1]));

				//�t���[�����Ԃ��擾
				if (mVec[0] == "frame" && mVec.size() > 1)
					fTime = stoi(mVec[1]);
			}
			//�����֐�
			state_do = SUMMON;
			Calc_Summon(masu, fTime);

		}

		if (msgVec[0] == "end")
		{
			state_anim = END;

		}
		////////////////////////////////


	}

	//===================================


	void Dice::End()
	{
		middleQ = endQ;
		startQ = endQ;
		/*D3DXQuaternionSlerp(&middleQ, &startQ, &endQ, 1.0f);*/

		D3DXMatrixRotationQuaternion(&matRot, &endQ);

		nowpos_.set(nextpos_);

		state_anim = WAIT;
	}

	//�A�j���[�V����
	//���s�ړ��A�j���[�V����
	void Dice::Do_Parallel()
	{
		if (frames_-- >= 0 )
		{
			nowpos_ += velocity_;
		}
		else
		{
			state_anim = END;
		}

	}

	void Dice::Do_Rotation()
	{
		if (frames_-- >= 0)
		{
			nowpos_ += velocity_;
			frames_roll += velocity_roll;
			D3DXQuaternionSlerp(&middleQ, &startQ, &endQ, frames_roll);
			D3DXMatrixRotationQuaternion(&matRot, &middleQ);
		}
		else
		{
			state_anim = END;
		}
	}

	void Dice::Do_Summon()
	{
		if (frames_-- >= 0)
		{
			nowpos_ += velocity_;
		}
		else
		{
			state_anim = END;
		}
	
	}



	void Dice::Calc_Summon(const ci_ext::Vec3i &masu, const int in_frames)
	{
		float V = 0;
		
		frames_ = in_frames;

		nextpos_.set(MasuToPos(masu));

		V = OFFSET / frames_;

		velocity_.set(0.f, V, 0.f);

		nowpos_.set(nextpos_);
		nowpos_.offset(0.f, -OFFSET, 0.f);

		state_anim = ANIMATION;
	}


	void Dice::Do_Fall()
	{
		if (frames_-- >= 0)
		{
			nowpos_ += velocity_;
		}
		else
		{
			state_anim = END;
		}

	}



	void Dice::Calc_Fall(const int in_frames)
	{
		float V = 0;

		frames_ = in_frames;

		V = -((OFFSET / frames_) * 2.0f);

		nextpos_.set(nowpos_);
		nextpos_.offset(0.f, -OFFSET*2.f, 0.f);

		velocity_.set(0.f, V, 0.f);
		
		state_anim = ANIMATION;
	}
	void Dice::Calc_Rotation(const ci_ext::Vec3i &masu, const int in_frames)
	{
		float V = 0;
		DIRECTION direction;

		frames_ = in_frames;

		nextpos_.set(MasuToPos(masu));

		direction = TranceDirection(nextpos_, nowpos_);

		V = OFFSET / frames_;


		switch (direction)
		{
		case NORTH:
			velocity_.set(0.f, 0.f, V);

			break;

		case SOUTH:
			velocity_.set(0.f, 0.f, -V);
			break;

		case EAST:
			velocity_.set(V, 0.f, 0.f);
			break;

		case WEST:
			velocity_.set(-V, 0.f, 0.f);
			break;

		case CENTER:
			break;
		default:
			break;
		}
	

		velocity_roll = 1.0f / frames_;
		frames_roll = 0.f;
		setStartQuaternion(endQ, middleQ, direction);

		swap(direction);

		state_anim = ANIMATION;

	}


	void Dice::Calc_Parallel(const ci_ext::Vec3i &masu, const int in_frames)
	{
		float V = 0;

		frames_ = in_frames;

		nextpos_.set(MasuToPos(masu));

		V = OFFSET / frames_;
		

		switch (TranceDirection(nextpos_, nowpos_))
		{
		case NORTH:
			velocity_.set(0.f, 0.f, V);

			break;

		case SOUTH:
			velocity_.set(0.f, 0.f, -V);
			break;

		case EAST:
			velocity_.set(V, 0.f, 0.f);
			break;

		case WEST:
			velocity_.set(-V, 0.f, 0.f);
			break;

		case CENTER:
			break;
		default:
			break;
		}

		state_anim = ANIMATION;
	
	}





































	//===================================
	//	DoMove
	//===================================
	



	//-------------------------------//
	/*
	@brief							�N�H�[�^�j�I��������
	@return							�Ȃ�
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
	void Dice::Do_Select()
	{
		if (RGBFLAG == false)
		{
			rgb.offset(-5,-5,-5);
			if (rgb.iy() < 100)
			{
				rgb.offset(5, 5, 5);
				RGBFLAG = true;
			}
		}
		if (RGBFLAG == true)
		{
			rgb.offset(5, 5, 5);
			if (rgb.iy() > 150)
			{
				rgb.offset(-5, -5, -5);
				RGBFLAG = false;
			}
		}
	}
	//===================================




	//===================================
	//	�A�N�Z�T
	//===================================

	void Dice::OnSelectFlag()
	{
		selected_ = true;
	}
	void Dice::OffSelectFlag()
	{
		selected_ = false;
		RGBFLAG = false;
		if (playerID_ == 0)
		{
			rgb.set(150, 150, 250);

		}
		else
		{
			rgb.set(250, 150, 150);

		}
	}

	//===================================



	//----------prepareMove----------//
	/*
	@brief							�ړ������t���O���Z�b�g
	@return							DIRECT
	*/
	Dice::DIRECTION Dice::TranceDirection(const Vec3f &p_nextpos, const Vec3f &p_nowpos)
	{

		if ((int)p_nowpos.x() < (int)p_nextpos.x()) return EAST;

		else if ((int)p_nextpos.x() < (int)p_nowpos.x()) return WEST;

		else if ((int)p_nowpos.z() < (int)p_nextpos.z()) return NORTH;

		else if ((int)p_nextpos.z() < (int)p_nowpos.z()) return SOUTH;

		else return CENTER;

	}
	//-------------------------------//


	/*
	@brief							�}�X���|�W�V�����ɕύX
	@return							vec3f (pos)
	*/
	Vec3f Dice::MasuToPos(const Vec3i& masu)
	{
		Vec3f pos = (0.f, 0.f, 0.f);

		pos.x(((float)((masu.x()) * OFFSET)+ 5.f));
		pos.y(5.0f);
		pos.z(((float)((masu.z()) * OFFSET)-5.f));

		return pos;
	}

	void Dice::init(){

		Vec3f rot = ((playerID_ == 0) ? Vec3f(0.f, 180.f, 0.f): Vec3f(0.f, 0.f, 0.f));

		p_mons = insertAsChild(new Monster("monster", nowpos_, (int)defType_, rot));

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











}





