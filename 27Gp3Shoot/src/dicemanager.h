#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include <vector>
#include <memory>
#include "dice.h"

namespace game
{

class DiceManager : public ci_ext::Object
{
	//*************************************************//
	//�@�萔
	//*************************************************//
private:

	static const int JUDGE[3][3];						//����
	static const ci_ext::Vec3i STARTMASU[2][2];			//�_�C�X�����ʒu

	enum BattlePhase{
		check,							// �U������_�C�X�̃`�F�b�N
		atkSelect,						// �U�����̑I��
		emySelect,						// �U���ڕW�̑I��
		battle,							// �o�g����
		destroy,						// ����
		end,
	};

	//�o�g���m�F�p�ɁB
	struct pBattleDice
	{
		bool									selectOffF;			// �U���̑I���t���O
		std::shared_ptr<game::Dice>				p_offense;			// �U�����_�C�X�̃|�C���^
		int										selectDef;			// ����̑I��
		std::vector <std::shared_ptr<game::Dice>> p_defense;		// �h�䑤�_�C�X�̃|�C���^
		int										result;				// ���s���ʁi�U����::-1:�s�k 0:������ 1:����)
	};



	//*************************************************//
	//�@�ϐ�
	//*************************************************//
private:

	std::weak_ptr<ci_ext::Object> p_parent;				// �e�̃|�C���^

	std::vector<std::vector<ci_ext::Vec3i>> dicemasu;	// �_�C�X�I�u�W�F�N�g�̍��W�i�O�v���C���[:��_�C�X�j

	BattlePhase					batphase_;				// �o�g�����̃t�F�[�Y

	std::vector<pBattleDice>	battledice;				// �o�g���ҋ@���̃_�C�X�̍��W


	int phase_;											// �t�F�[�Y
	int	turnPlayer_;									// ���݂̃^�[���v���C���[
	int selectDice_;									// �w�肵�Ă���_�C�X
	bool batinit_;										// �o�g���t�F�C�Y���̏����t���O
	bool batcutin_;										// �o�g���t�F�C�Y���̃J�b�g�C���ҋ@�t���O

	//*************************************************//
	//�@�֐�
	//*************************************************//
private:

	/*
		@brief							�L�[���͏���
		@return							�Ȃ�
	*/
	void key();

	/*
		@brief							���̃_�C�X�̍��W�����ɂ�
		@param[in] no					�_�C�X�̔z��
		@param[in] dir					�����x�N�g��
		@return							�Ȃ�
	*/
	void MovingPos(const int no, const ci_ext::Vec3i& pos);	


	/*
		@brief			���s����
		@param			�Ȃ�
		@return			���s����		-1:�s�k 0:������ 1:����
	*/
	int getAttackJudge(int player, int enemy);


	/*
		@brief							�_�C�X�I�u�W�F�N�g�̎擾
		@param[in] player				�_�C�X�̔z��
		@param[in] id					���ʔԍ�
		@return							�I�u�W�F�N�g�|�C���^
	*/
	std::weak_ptr<Object> getDicePtr(const int player,const int id);


	/*
		@brief			�����t�F�C�Y
		@return			�Ȃ�
	*/
	void Summon();

	
	/*
		@brief			���C���t�F�C�Y
		@return			�Ȃ�
	*/
	void Main();

	
	/*
		@brief			�o�g���t�F�C�Y
		@return			�Ȃ�
	*/
	void Battle();


	/*
		@brief			�U���̃`�F�b�N
		@return			�Ȃ�
	*/
	void Check();




	

public:

	/*
		@brief							�^�[���̕ύX
		@param[in]	playerID			�v���C���[���ʔԍ�
		@return							�Ȃ�
	*/
	void ChangeTurn(const int playerID);


	/*
		@brief							�R���X�g���N�^
		@param[in]	objectName			�I�u�W�F�N�g��
		@param[in]	ptr					�e�̃|�C���^
		@return							�Ȃ�
	*/
	DiceManager(const std::string& objectName, const std::weak_ptr<Object> ptr);

	//tuika
	//void SetMasu();

	/*
		@brief							�T�C�R���쐬
		@return							�Ȃ�
	*/
	void init() override;


	/*
		@brief							�`��
		@return							�Ȃ�
	*/
	void render() override;

	/*
		@brief							�t���[���X�V
		@return							�Ȃ�
	*/
	void update() override;

	/*
		@brief							���b�Z�[�W�ǂݎ��
		@param[in] sender				�����|�C���^
		@param[in] msg					���b�Z�[�W�i������j
		@return							�Ȃ�
	*/
	void receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg) override;


};

}