#include "dicemanager.h"
#include "../../lib/gplib.h"
#include "dice.h"
#include "stage.h"
#include "ui.h"

#include "../../lib/ci_ext/object.hpp"

namespace game
{
	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//
	const int DiceManager::JUDGE[3][3]
	{
	//		  G  C  P
	/*G*/	{ 0, 1,-1},
	/*C*/	{-1, 0, 1},
	/*P*/	{ 1,-1, 0}
	};

	const ci_ext::Vec3i DiceManager::STARTMASU[2][2] =
	{
		{ ci_ext::Vec3i(1, 0, 0), ci_ext::Vec3i(3, 0, 0) },
		{ ci_ext::Vec3i(1, 4, 0), ci_ext::Vec3i(3, 4, 0) }
	};



	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//
	

	int DiceManager::getAttackJudge(int player, int enemy){
		return JUDGE[player][enemy];
	}
	
	std::weak_ptr<ci_ext::Object> DiceManager::getDicePtr(const int player, const int id)
	{
		std::string str = "dice_p" + std::to_string(player) + "_no" + std::to_string(id);
		return getObjectFromChildren(str);
	}

	
	void DiceManager::key()
	{
		if (gplib::input::CheckPush(gplib::input::KEY_LEFT))
		{
			MovingPos(selectDice_, ci_ext::Vec3i(-1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_RIGHT))
		{
			MovingPos(selectDice_, ci_ext::Vec3i(1, 0, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_UP))
		{
			MovingPos(selectDice_, ci_ext::Vec3i(0, 1, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_DOWN))
		{
			MovingPos(selectDice_, ci_ext::Vec3i(0, -1, 0));
		}
		if (gplib::input::CheckPush(gplib::input::KEY_BTN1))
		{
			//�_�C�X�ɑI��`��
			// dice->select();

			bool selectF = true;			//�I���t���O

			//�_�C�X�̂����ꂩ���ړ����͑I��ύX��s��
			std::string str = "p" + std::to_string(turnPlayer_);
			auto objects = getObjectsFromChildren({ "dice", str });
			for (auto obj : objects)
			{
				auto dice = ci_ext::weak_to_shared<game::Dice>(obj);

				if (dice->isMoving())
					selectF = false;
			}


			if (selectF)
			{
				auto ptr = getDicePtr(turnPlayer_, selectDice_);
				ci_ext::weak_to_shared<Dice>(ptr)->OffSelectFlag();

				//�I���\�ȃ_�C�X��T���B
				for (int i = 1; i <= (int)dicemasu[turnPlayer_].size(); i++)
				{
					int no = (selectDice_ + i) % (int)dicemasu[turnPlayer_].size();

					//�I������Ă���_�C�X�̎����I���ł��邩
					auto dice = ci_ext::weak_to_shared<Dice>(getDicePtr(turnPlayer_, no));

					if (dice->isIdoling())
					{
						selectDice_ = no;
						break;
					}
				}
			}
		}
	}
	void DiceManager::MovingPos(const int no, const ci_ext::Vec3i& dir)
	{
		//=============================
		// �_�C�X�ֈړ��ł��邩�m�F
		//=============================
		auto dice = ci_ext::weak_to_shared<Dice>(getDicePtr(turnPlayer_, no));
		//�ҋ@���ȊO�͈ړ��s��
		if (!dice->isIdoling()) 
			return;

		//=============================
		// �ׂ荇�����_�C�X���m�F
		//=============================

		for (int i = 0; i < static_cast<int>(dicemasu.size()); i++)	//�v���C���[
		{
			for (int j = 0; j < static_cast<int>(dicemasu[i].size()); j++)	//�_�C�X
			{
				if (i == turnPlayer_ && j == no)
					continue;

				if ((dicemasu[turnPlayer_][no] + dir).x() == dicemasu[i][j].x() 
					&& (dicemasu[turnPlayer_][no] + dir).y() == dicemasu[i][j].y())
				{
					return;
				}
			}
			
		}

		//=============================
		// �R�X�g�̊m�F�ƃ_�C�X�̈ړ�
		//=============================

		auto stage = ci_ext::weak_to_shared<CSceneStage>(p_parent);
		if (stage->checkMovableDice((dicemasu[turnPlayer_][no] + dir)))
		{
			dicemasu[turnPlayer_][no].offset(dir.x(),dir.y());

			auto dice = ci_ext::weak_to_shared<Dice>(getDicePtr(turnPlayer_, selectDice_));
			
			//�֐��ύX���s�����̂Œ��ӁI
			//���@dice->Move(dir, dicemasu[turnPlayer_][no]);
			dice->prepareMove(dicemasu[turnPlayer_][no]);
		}


	}
	void DiceManager::ChangeTurn(const int playerID)
	{
		if (turnPlayer_ != playerID)
		{
			turnPlayer_ = playerID;
			selectDice_ = 0;

			auto objects = getObjectsFromChildren({ "dice"});

			for (auto obj : objects)
			{
				auto dice = ci_ext::weak_to_shared<game::Dice>(obj);
				
				dice->OffSelectFlag();
			}
		}
	}

	//**************************************************************************************//
	void DiceManager::Check()
	{
		//=============================
		// �ׂ荇�����_�C�X���m�F
		//=============================

		ci_ext::Vec3i dir[] =
		{

			ci_ext::Vec3i(-1, 0, 0),		// ��
			ci_ext::Vec3i(1, 0, 0),			// �E
			ci_ext::Vec3i(0, 1, 0),			// ��
			ci_ext::Vec3i(0, -1, 0),		// ��

		};

		int enemyPlayer = (turnPlayer_ + 1) % 2;
		battledice.clear();


		for (int i = 0; i < static_cast<int>(dicemasu[turnPlayer_].size()); i++)	//�����̃_�C�X�̐�
		{
			bool inflag= false;				//�}���t���O
			pBattleDice temp;
			temp.selectOffF = false;

			//��������_�C�X���U���\���ǂ����i����łȂ����ǂ����j
			auto offdice = ci_ext::weak_to_shared<Dice>(getDicePtr(turnPlayer_, i));
			if (!offdice->isIdoling()) continue;

			temp.p_offense = offdice;

			for (int j = 0; j < static_cast<int>(dicemasu[enemyPlayer].size()); j++)	//����̃_�C�X�̐�
			{
				//�G�_�C�X�������Ă��邩�ǂ���
				auto defdice = ci_ext::weak_to_shared<Dice>(getDicePtr(enemyPlayer, j));
				if (!defdice->isIdoling()) continue;

				for (int k = 0; k < 4; k++)							//�����̊m�F�̉�
				{
					//�ׂ荇���Ă��邩����
					if ((dicemasu[turnPlayer_][i] + dir[k]).x() == dicemasu[enemyPlayer][j].x()
						&& (dicemasu[turnPlayer_][i] + dir[k]).y() == dicemasu[enemyPlayer][j].y())
					{
						inflag = true;
						temp.p_defense.push_back(defdice);
						break;
					}
				}

			}

			if (inflag)
				battledice.push_back(temp);

		}
	}
	
	

	//**************************************************************************************//


	void DiceManager::Summon()
	{
		//�����t�F�C�Y�͂Ƃ肠�����ۗ�
		//�f�X��Ԃ��Ȃ��ꍇ�͎��̃t�F�[�Y��

		bool nextF = true;		//�t�F�C�Y�ړ��t���O

		std::string str = "p" + std::to_string(turnPlayer_);
		auto objects = getObjectsFromChildren({ "dice", str });

		for (auto obj : objects)
		{
			auto dice = ci_ext::weak_to_shared<game::Dice>(obj);
			if (dice->isDying())
				nextF = false;
		}

		if (nextF)
		{
			auto parent = ci_ext::weak_to_shared<CSceneStage>(p_parent);
			parent->NextPhase();
		}
		else
		{
			//����ł���_�C�X�͂��̏�ɏ���������B
			for (auto obj : objects)
			{
				auto dice = ci_ext::weak_to_shared<game::Dice>(obj);
				if (dice->isDying())
					dice->Spawn();
			}
		}

	}
	void DiceManager::Main()
	{
		key();

		batphase_ = check;
		auto ptr = getDicePtr(turnPlayer_, selectDice_);
		ci_ext::weak_to_shared<Dice>(ptr)->OnSelectFlag();
	}
	void DiceManager::Battle()
	{
		switch (batphase_)
		{
			//======================
			//�U������L�����̊m�F
			//======================
		case game::DiceManager::check:
			
			Check();
			//�U���ҋ@�O�Ȃ�end

			if (battledice.empty())
			{
				batphase_ = end;
			}
			//1�ȏ�Ȃ�AtkSelect
			else
			{
				battledice[0].selectOffF = true;
				battledice[0].selectDef = 0;

				batphase_ = atkSelect;
			}
			break;

			//======================
			//�U�����̃L�����I��
			//======================
		case game::DiceManager::atkSelect:
			
			//�U���ҋ@��1�L�����Ȃ�emySelect
			if (battledice.size() == 1)
			{
				batphase_ = emySelect;
			}
			//�U���ҋ@2�Ȃ�I��������
			else
			{
				gplib::font::Draw_FontText(500, 500, 0.f, "�U���Z���N�g", ARGB(255, 255, 0, 0), 0);

				//�_�C�X�̑I����ύX����
				if (gplib::input::CheckPush(gplib::input::KEY_BTN1))
				{
					for (int i = 0; i < (int)battledice.size(); i++)
					{
						//�I������Ă���_�C�X�m�F
						if (battledice[i].selectOffF == true)
						{
							battledice[i].selectOffF = false;

							//�I������Ă��鎟�̃_�C�X�֑I�����ڂ��B
							int no = (i + 1) % (int)battledice.size();

							battledice[no].selectOffF = true;

							//�_�C�X�ɑI��`��
							// dice->select();
						}
					}
				}

				//�_�C�X�̌���
				if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
				{
					//�I����Ԃ��������邱��
					batphase_ = emySelect;
				}


			}

			break;

			//======================
			//�h�䑤�̑I��
			//======================
		case game::DiceManager::emySelect:


			for (auto bdice : battledice)
			{
				if (bdice.selectOffF)
				{
					//�U���ҋ@1�Ȃ�battle
					if (bdice.p_defense.size() == 1)
					{
						bdice.selectDef = 0;
						batphase_ = battle;
						batinit_ = true;
					}
					//�U���ҋ@2�Ȃ�I��������
					else
					{
						gplib::font::Draw_FontText(500, 500, 0.f, "�U���Z���N�g", ARGB(255, 255, 0, 0), 0);
						//�_�C�X�̑I����ύX����
						if (gplib::input::CheckPush(gplib::input::KEY_BTN1))
						{
								//�I������Ă��鎟�̃_�C�X�֑I�����ڂ��B
								bdice.selectDef = (bdice.selectDef + 1) % (int)bdice.p_defense.size();

								//�_�C�X�ɑI��`��
								// dice->select();
						}

						//�_�C�X�̌���
						if (gplib::input::CheckPush(gplib::input::KEY_BTN0))
						{
							//�I��`��̉���

							batphase_ = battle;
							batinit_ = true;
						}
					}
					
					break;
				}
			}
			break;

			//======================
			//���s�̔���ƍU���A�j���[�V����
			//======================
		case game::DiceManager::battle:
			
			//���s����
			if (batinit_)
			{
				int offenseNum;

				for (auto &bdice : battledice){
					if (bdice.selectOffF){

						int offense = bdice.p_offense->getAtkSpecies();
						int defense = bdice.p_defense[bdice.selectDef]->getDefSpecies();

						bdice.result = getAttackJudge(offense, defense);
						
						offenseNum = bdice.p_offense->getDefSpecies();

						break;
					}
				}
				
				batcutin_ = true;
				batinit_ = false;
			}
			else
			{
				auto object = getObjectsFromRoot({ "cutin" });

				if (object.empty() && batcutin_)
				{
					int offenseNum;

					for (auto &bdice : battledice){
						if (bdice.selectOffF){
							
							offenseNum = bdice.p_offense->getDefSpecies();
							break;
						}
					}
					//�J�b�g�C��������
					insertAsChild(new game::UI("cutin_attack", UI::UITYPE::CUTINMONSTER, -500.f, gplib::system::WINH / 2.f, offenseNum));
					batcutin_ = false;
				}
				else if (!batcutin_)
				{
					auto object = getObjectFromChildren("cutin_attack");
					if (ci_ext::weak_to_shared<UI>(object)->isDestroy())
					{
						batphase_ = destroy;
					}
				}
				
			}
			break;

			//======================
			//�_���[�W�����ƃ_�C�X�̍폜
			//======================
		case game::DiceManager::destroy:

			for (auto &bdice : battledice){
				if (bdice.selectOffF){

					switch (bdice.result)
					{
						//�U�����̕���
					case -1:
						bdice.p_offense->destroy();
						break;

						//������
					case 0:
						bdice.p_offense->destroy();
						bdice.p_defense[bdice.selectDef]->destroy();
						break;

						//�U�����̏���
					case 1:
						bdice.p_defense[bdice.selectDef]->destroy();
						break;
					}

				}
			}

			batphase_ = check;
			break;

			//======================
			//�U���_�C�X���Ȃ��ꍇ�A���̃t�F�[�Y��
			//======================
		case game::DiceManager::end:
			
			auto parent = ci_ext::weak_to_shared<CSceneStage>(p_parent);
			parent->NextPhase();
			break;
		}
	}



	//**************************************************************************************//
	//�f�t�H���g�֐�
	//**************************************************************************************//


	DiceManager::DiceManager(const std::string& objectName,const std::weak_ptr<Object> ptr)
		:
		Object(objectName),
		p_parent(ptr),
		turnPlayer_(0),
		selectDice_(0)
	{
		// �_�C�X�̃}�X���W�̏�����
		for (int i = 0; i < 2; i++)
		{
			std::vector<ci_ext::Vec3i> temp;
			for (int j = 0; j < 2; j++)
			{
				temp.push_back(STARTMASU[i][j]);
			}
			dicemasu.push_back(temp);
		}
	}

	void DiceManager::init()
	{
		//�_�C�X�̐���
		for (int i = 0; i < 2; i++)	//�v���C���[
		{
			for (int j = 0; j < 2; j++)	//�_�C�X�̐�
			{
				std::string str = "dice_p" + std::to_string(i) + "_no" + std::to_string(j);
				auto ptr = insertAsChild(new game::Dice(str,gplib::math::GetRandom<int>(0,2),i, dicemasu[i][j]));
			}
		}

		
	}
	

	void DiceManager::render()
	{
#ifdef _DEBUG
		gplib::graph::Draw_2DClear();//tuika
		std::string str = "���݃v���C���[ :" + std::to_string(turnPlayer_) +
			",�I���_�C�X :" + std::to_string(selectDice_) + "  X:" +
			std::to_string(dicemasu[turnPlayer_][selectDice_].x()) +
			", Y:" + std::to_string(dicemasu[turnPlayer_][selectDice_].y());

		gplib::font::Draw_FontTextNC(100, 100, 0.f, str, ARGB(255, 0, 0, 0), 0);


		for (int i = 0; i < dicemasu.size(); i++)
		{
			for (int j = 0; j < dicemasu[i].size(); j++)
			{
				std::string str = "�v���C���[" + std::to_string(i) +
					",  �_�C�X" + std::to_string(j) + "  X:" +
					std::to_string(dicemasu[i][j].x()) +
					", Y:" + std::to_string(dicemasu[i][j].y());

				if (i == turnPlayer_ && j == selectDice_)
				{
					gplib::font::Draw_FontTextNC(500 + (i * 300), 100 + 30 * j, 0.f, str, ARGB(255, 255, 0, 0), 0);
				}
				else
				{
					gplib::font::Draw_FontTextNC(500 + (i * 300), 100 + 30 * j, 0.f, str, ARGB(255, 0, 0, 0), 0);
				}	
			}
		}
		gplib::graph::Draw_2DRefresh();//tuika



#endif
	}

	void DiceManager::update()
	{
		switch (phase_)
		{
			//�����t�F�C�Y
		case 0:
			Summon();
			break;
			//���C���t�F�C�Y
		case 1:
			Main();
			break;
			//�o�g���t�F�C�Y
		case 2:
			Battle();
			break;
		}

	}

	//���b�Z�[�W��M
	void DiceManager::receiveMsg(std::weak_ptr<ci_ext::Object>& sender, const std::string& msg)
	{
		//���b�Z�[�W����
		auto msgVec = gplib::text::split(msg, ",");

		for (auto ms : msgVec){
			//����ɕ���
			auto mVec = gplib::text::split(ms, "=");
			
				if (mVec[0] == "player")
				{
					ChangeTurn(stoi(mVec[1]));
				}
				if (mVec[0] == "phase")
				{
					phase_ = stoi(mVec[1]);
				}
			}
	}
}