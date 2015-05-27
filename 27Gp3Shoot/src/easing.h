namespace easing{
	//利用できるイージング式一覧
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
	//イージングの初期設定を行う。
	//この時点では処理は行われない。
	//name　イージング名
	//type　イージング種類　esing.h参照
	//start	初期値
	//end		終了値
	//duration 持続フレーム
	//-----------------------------------------------------------
	void Set(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  void Create(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  //-----------------------------------------------------------
	//イージングの開始処理を行う。
	//-----------------------------------------------------------
  void Start(const std::string& name, const EASINGTYPE type, const float start, const float end, const int duration);
  void Start(const std::string& name);
	//-----------------------------------------------------------
	//途中経過値の取得
	//-----------------------------------------------------------
	void GetPos(const std::string& name, float& pos);
	float GetPos(const std::string& name);
  void Step(const std::string& name, float& pos);

	//-----------------------------------------------------------
	//イージング処理の更新　-　毎フレーム行うこと。
	//-----------------------------------------------------------
	void Step();
	//-----------------------------------------------------------
	//イージング処理の初期化　-　利用前に行うこと。
	//-----------------------------------------------------------
	void Init();
  //-----------------------------------------------------------
  //イージング処理の一時停止
  //-----------------------------------------------------------
  void Pause(const std::string& name);
  //-----------------------------------------------------------
  //イージング処理の再開
  //-----------------------------------------------------------
  void Resume(const std::string& name);
  //-----------------------------------------------------------
  //イージング処理のリセット
  //-----------------------------------------------------------
  void Reset(const std::string& name);

}


