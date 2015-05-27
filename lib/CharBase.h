#pragma once
#include "ci_ext/color.hpp"
namespace charabase{

	//����\�ȃx�[�X�f�[�^
	//�����l��0,1.0f�̂��̂͏��O����B
	struct BaseData{
		int		wid, hgt;		//�摜������
		const std::string 	resname;		//�g�p�C���[�W��
	};

	struct Vec3f{
		float x, y, z;
	};

	//
	//
	//
	//
	struct Anim{
		int		no;					//���݂̃A�j��NO�@���̒l��0,1,2,3,4,�ƃJ�E���g���邾��
		float	cnt;				//�A�j���X�V�J�E���^
		float	speed;			//�A�j���X�V���x
		int		max;				//�A�j�������@���̒l��no�������0����J�E���g
	};

	struct CharaBase
	{
		Vec3f   pos;							//���W
		Vec3f		add;							//�ړ���
		int			width, height;		//�摜������

		Vec3f   scale;						//�g�嗦
		Vec3f   src;							//���o�ʒu
		std::string resname;			//�g�p�C���[�W��

		bool		show;							//���p�t���O

		float		angle;						//�p�x
		float		a;								//�A���t�@
		float		r, g, b;					//�F
	};

	
	//----------------------
	//�ėp�ړ�����
	//cb : ���삵����CharaBase�\����
	extern void MoveChar(CharaBase& cb);
	//----------------------
	//�ėp�����ʏ���
	//cb : ���삵����CharaBase�\����
	extern bool CheckUseChar(const CharaBase& cb);
	//----------------------
	//�ėp��������
	//cb : ���삵����CharaBase�\����
	extern void NoUseChar(CharaBase& cb);
	//----------------------
	//�ėp�o������
	//cb : ���삵����CharaBase�\����
	extern void UseChar(CharaBase& cb);
	//----------------------
	//����p��`�쐬 ������W�
	//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
	//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
	//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
	extern void MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	//----------------------
	//����p��`�쐬 �������W�
	//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
	//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
	//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
	extern void MakeRect(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	//----------------------
	//����p��`�쐬 �������W��@�g�嗦�l��
	//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
	//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
	//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
	extern void MakeRectScales(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
  //----------------------
  //�ėp�`�揈�� �����
  //cb : ���삵����CharaBase�\����
  extern void Draw_CharBuruburu(const CharaBase& cb, int range);
  //----------------------
	//�ėp�`�揈�� �����
	//cb : ���삵����CharaBase�\����
	extern void Draw_Char(const CharaBase& cb);
	//----------------------
	//�ėp�`�揈�� ����
	//cb : ���삵����CharaBase�\����
	extern void Draw_Char2(const CharaBase& cb);
	//----------------------
	//�ėp�`�揈�� ����� �J�����T�O����
	//cb : ���삵����CharaBase�\����
	extern void Draw_CharNC(const CharaBase& cb);
	//----------------------
	//�ėp�`�揈�� ���� �J�����T�O����
	//cb : ���삵����CharaBase�\����
	extern void Draw_Char2NC(const CharaBase& cb);
	//----------------------
	//������`�F�b�N ������RECT�𐶐������ׂ�
	//cb1,cb2 : ����𒲂ׂ���CharaBase�\����
  extern bool HitCheck(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);
	//----------------------
	//������`�F�b�N ������RECT�𐶐������ׂ�B�g�嗦���l���ɓ����
	//cb1,cb2 : ����𒲂ׂ���CharaBase�\����
  extern bool HitCheckScales(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);
  void Draw_Frame(const CharaBase& cb, ci_ext::Color color, int lineweight);
  bool HitCheck2(const CharaBase& cb1, const CharaBase& cb2, bool debug_mode = true);	//------------------------------------------------------
	//CharBase����������
	//
	/*
	//��{�ƂȂ�`�b�v�̃T�C�Y�Ɨ��p����摜
	BaseData base = {CHARWID,CHARHGT,charIMG};
	//�������@�w��O�̒l�͏����l�ɂȂ�B
	//���̏ꍇ��wid,hgt,img�ȊO�͏����l
	InitCharBase(player.pos,base);

	//���̏ꍇ��wid,hgt,img,x,y�ȊO�͏����l
	InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

	���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
	UseChar�ŉ���Ԃɂ��邱�ƁB
	*/
	extern void InitCharBase(CharaBase& cb, const BaseData& base,
		float x = 0.f, float y = 0.f, float z = 0.f, float addx = 0.f, float addy = 0.f, float addz = 0.f,
		int srcX = 0, int srcY = 0, int srcZ = 0, float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);

	//------------------------------------------------------
	//CharBase����������
	//
	/*
	//�������@�w��O�̒l�͏����l�ɂȂ�B
	���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
	UseChar�ŉ���Ԃɂ��邱�ƁB
	*/
	extern void	InitCharBase(CharaBase& cb, float x, float y, float z, float addx, float addy, float addz,
		const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
		float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);
	
	//------------------------------------------------------
	//CharBase����������
	//�����\���̂��O�N���A����
	extern void InitCharBase(CharaBase& cb);
	//----------------------
	//�A�j���[�V�����ݒ�
	//max : �O�ɖ߂閇��
	//speed : �A�j�����x
	extern void SetAnim(Anim& anim, int max, float speed);
	//�A�j���X�V
	//���̊֐���no�̒l���X�V����Bno�̒l��
	//CharBase.src.x�ɑ������B
	//�A�j���̏�����ύX�������ꍇ�͈ȉ�
	/*
	StepAnim(obj.anim);
	//�ȉ��ŃA�j���[�V�����w��\
	int animTable[] = {0,1,0,2};
	//���ۂɗ��p����ꍇ��
	animTable[obj.anim.no]
	StepAnim(obj.anim);
	*/
	extern bool StepAnim(Anim& anim);
	//----------------------
	//�A�j���[�V������~
	//speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
	extern void StopAnim(Anim& anim);
	//----------------------
	//�A�j���[�V�����J�n
	//speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
	extern void StartAnim(Anim& anim, float speed);


}



