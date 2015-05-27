namespace easing{
	//���p�ł���C�[�W���O���ꗗ
	enum EASINGTYPE{
		LINEAR,
		BACKIN,
		BACKOUT,
		BACKINOUT,
		BOUNCEOUT,
		BOUNCEIN,
		BOUNCEINOUT,
		CIRCIN,
		CIRCOUT,
		CIRCINOUT,
		CUBICIN,
		CUBICOUT,
		CUBICINOUT,
		ELASTICIN,
		ELASTICOUT,
		ELASTICINOUT,
		EXPOIN,
		EXPOOUT,
		EXPOINOUT,
		QUADIN,
		QUADOUT,
		QUADINOUT,
		QUARTIN,
		QUARTOUT,
		QUARTINOUT,
		QUINTIN,
		QUINTOUT,
		QUINTINOUT,
		SINEIN,
		SINEOUT,
		SINEINOUT,
	};

	//-----------------------------------------------------------
	//�C�[�W���O�̏����ݒ���s���B
	//���̎��_�ł͏����͍s���Ȃ��B
	//name�@�C�[�W���O��
	//type�@�C�[�W���O��ށ@esing.h�Q��
	//start	�����l
	//end		�I���l
	//duration �����t���[��
	//-----------------------------------------------------------
	void Set(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  void Create(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  //-----------------------------------------------------------
	//�C�[�W���O�̊J�n�������s���B
	//-----------------------------------------------------------
  void Start(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  void Start(const std::string& name);
	//-----------------------------------------------------------
	//�r���o�ߒl�̎擾
	//-----------------------------------------------------------
	void GetPos(const std::string& name, float& pos);
	float GetPos(const std::string& name);
  void Step(const std::string& name, float& pos);

	//-----------------------------------------------------------
	//�C�[�W���O�����̍X�V�@-�@���t���[���s�����ƁB
	//-----------------------------------------------------------
	void Step();
	//-----------------------------------------------------------
	//�C�[�W���O�����̏������@-�@���p�O�ɍs�����ƁB
	//-----------------------------------------------------------
	void Init();
  //-----------------------------------------------------------
  //�C�[�W���O�����̈ꎞ��~
  //-----------------------------------------------------------
  void Pause(const std::string& name);
  //-----------------------------------------------------------
  //�C�[�W���O�����̍ĊJ
  //-----------------------------------------------------------
  void Resume(const std::string& name);
  //-----------------------------------------------------------
  //�C�[�W���O�����̃��Z�b�g
  //-----------------------------------------------------------
  void Reset(const std::string& name);

}


