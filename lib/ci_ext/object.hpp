#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <assert.h>

namespace ci_ext
{

//-----------------------------------------------------------------------
//キャスト関連
//-----------------------------------------------------------------------
//weak_ptrから任意の型へのshared_ptrへキャストします
template<typename T1, typename T2>
inline std::shared_ptr<T1> weak_to_shared(const std::weak_ptr<T2>& ptr)
{
  if (ptr.expired()) return std::shared_ptr<T1>();
  return std::static_pointer_cast<T1>(ptr.lock());
}
//weak_ptrから同じ型へのshared_ptrへキャストします
template<typename T>
inline std::shared_ptr<T> weak_to_shared(const std::weak_ptr<T>& ptr)
{
  if (ptr.expired()) return std::shared_ptr<T>();
  return std::static_pointer_cast<T>(ptr.lock());
}
//shared_ptrから違う型へのshared_ptrへキャストします
template<typename T1, typename T2>
inline std::shared_ptr<T1> shared_cast(const std::shared_ptr<T2>& ptr)
{
  return std::static_pointer_cast<T1>(ptr);
}

//-----------------------------------------------------------------------
//ベースオブジェクト
//-----------------------------------------------------------------------
class Object
{
public:
  enum class Status
  {
    null,    //未定義
    run,     //動作中
    sleep,   //停止中、規定フレーム後runに変化
    pause,   //ポーズ中（update()ではなくupdatePause()を処理）
    destroy, //破棄待ち
    dead,    //削除
  };

  //オブジェクト生成
  //継承時に呼び出し、名前と駆動ステータスを設定すること
  Object(
    const std::string& name,
    Status status = Status::run)
    :
    name_(name),
    status_(status),
    priority_(0),
    framecount_(0),
    hierarchyLevel_(1)
  {
    //      OutputDebugString("Object\n");
    initID();
  }

  //オブジェクト破棄、特に何もしないがvirtualにしておかないとリークが起きる可能性がある
  virtual ~Object()
  {
    //      OutputDebugString("~Object\n");
    //      DeleteStringFromList(id_);
    //      std::ofstream f("a.txt", std::ofstream::app);
    //      f << "destractor:" << name_ << std::endl;
  }

//-----------------------------------------------------------------------
//親にオブジェクトを追加
//-----------------------------------------------------------------------
  template<typename Ptr>
  std::weak_ptr<Object> insertToParent(Ptr* o)
  {
    auto parent = getParentPtr();
    if (parent.expired())
      assert(!"親のポインタがありませんよ");
    parent.lock()->insertAsChild(o);
    return o->selfPtr();
  }
  //ポーズ状態で親に追加
  template<typename Ptr>
  std::weak_ptr<Object> insertToParentPause(Ptr* o)
  {
    std::weak_ptr<Object> ret = insertToParent(o);
    o->pause();
    return ret;
  }
  //スリープ状態で親に追加
  //寝かせるフレーム数を指定
  template<typename Ptr>
  std::weak_ptr<Object> insertToParentSleep(Ptr* o, int framecount)
  {
    std::weak_ptr<Object> ret = insertToParent(o);
    o->sleep(framecount);
    return ret;
  }

//-----------------------------------------------------------------------
//子供にオブジェクトを追加
//-----------------------------------------------------------------------
  template<typename Ptr>
  std::weak_ptr<Object> insertAsChild(Ptr* p)
  {
    std::shared_ptr<Ptr> o(p);
    p->setWeakPtr(o);
    p->setParentPtr(selfPtr());
    p->hierarchyLevel_ = hierarchyLevel_ + 1;
    //AddStringToList(o->name(), o->ID());
    ins_.push_back(o);
    return o;
  }
  //ポーズ状態で子供に追加
  template<typename Ptr>
  std::weak_ptr<Object> insertAsChildPause(Ptr* o)
  {
    std::weak_ptr<Object> ret = insertAsChild(o);
    o->pause();
    return ret;
  }
  //スリープ状態で子供に追加
  //寝かせるフレーム数を指定
  template<typename Ptr>
  std::weak_ptr<Object> insertAsChildSleep(Ptr* o, int framecount)
  {
    std::weak_ptr<Object> ret = insertAsChild(o);
    o->sleep(framecount);
    return ret;
  }
    
//-----------------------------------------------------------------------
//ポインタ関連
//-----------------------------------------------------------------------
  //自分のweak_ptrを取得する
  std::weak_ptr<Object> selfPtr() const
  {
    return selfPtr_;
  }
  //自分自身のweak_ptrを設定する（ルート以外で使う必要は無い）
  void setWeakPtr(const std::weak_ptr<Object>& w)
  {
    selfPtr_ = w;
  }
  //親のweak_ptrを取得する
  std::weak_ptr<Object>  getParentPtr() const
  {
    return parentPtr_;
  }
  //ルートオブジェクトのweak_ptrを取得する
  std::weak_ptr<Object> getRootObject() const
  {
    auto parent = getParentPtr();
    if (parent.expired())
      return selfPtr();
    else
      return parent.lock()->getRootObject();
  }

//-----------------------------------------------------------------------
//オブジェクト取得関連
//-----------------------------------------------------------------------
  //全子供タスクを取得
  std::vector<std::weak_ptr<Object>> getChildren() const
  {
    std::vector<std::weak_ptr<Object>> ret;
    for (auto& child : children_)
      ret.push_back(child);
    return ret;
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていない場合はtrueが入る
  //<例1>
  // objectのname   "player_shot_3way"
  // findNames    = {"shot", "laser"}
  // findNotNames = {"enemy"}
  //  =>true
  //<例2>
  // objectのname   "player_shot_3way"
  // findNames    = {"shot", "laser"}
  // findNotNames = {"player"}
  //  =>false
  bool findObject(
    std::weak_ptr<Object> object,
    const std::vector<std::string>& findNames,
    const std::vector<std::string>& findNotNames) const
  {
    for (auto& name : findNames)
    {
      //探す名前がなかったら条件へ戻る
      if (!object.lock()->findName(name))
        continue;
      //入っていてはいけない名前が見つかったら検索終了
      for (auto& name2 : findNotNames)
        if (object.lock()->findName(name2))
          return false;
      //見つかった
      return true;
    }
    return false;
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものをvectorで戻す
  //例はfindObject()参照
  std::vector<std::weak_ptr<Object>> getObjectsFromChildren(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {}) const
  {
    std::vector<std::weak_ptr<Object>> ret;
    for (auto& child : children_)
    {
      auto r = child->getObjectsFromChildren(objectNames1, objectNotNames2);
      ret.insert(ret.end(), r.begin(), r.end());
      if (findObject(child, objectNames1, objectNotNames2))
      {
        ret.push_back(child);
      }
    }
    return ret;
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものをvectorで戻す
  //例はfindObject()参照
  std::vector<std::weak_ptr<Object>> getObjectsFromRoot(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {}) const
  {
    std::vector<std::weak_ptr<Object>> ret;
    std::shared_ptr<Object> root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      auto r = child->getObjectsFromChildren(objectNames1, objectNotNames2);
      ret.insert(ret.end(), r.begin(), r.end());
      if (findObject(child, objectNames1, objectNotNames2))
      {
        ret.push_back(child);
      }
    }
    return ret;
  }
  //指定した名前のobjectを自分の子供から検索し戻す
  std::weak_ptr<Object> getObjectFromChildren(
    const std::string& objectName) const
  {
    for (auto& child : children_)
    {
      if (child->name() == objectName)      return child;
      auto temp = child->getObjectFromChildren(objectName);
      if (!temp.expired()) return temp;
    }
    return std::weak_ptr<Object>();
  }
  //指定した名前のobjectをルートから検索し戻す
  std::weak_ptr<Object> getObjectFromRoot(
    const std::string& objectName) const
  {
    std::shared_ptr<Object> root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (child->name() == objectName)      return child;
      auto temp = child->getObjectFromChildren(objectName);
      if (!temp.expired()) return temp;
    }
    return std::weak_ptr<Object>();
  }
  //オブジェクトのユニークIDからオブジェクトを取得
  //但し、自分の子供以下から探し、親方向には検索しない
  std::weak_ptr<Object> getObjectFromChildren(int id) const
  {
    std::weak_ptr<Object> ret;
    if (ID() == id) return selfPtr();
    for (auto& child : children_)
    {
      ret = child->getObjectFromChildren(id);
      if (!ret.expired()) return ret;
    }
    return ret;
  }
  //オブジェクトのユニークIDからオブジェクトを取得
  //ルートオブジェクトから検索をかける
  std::weak_ptr<Object> getObjectFromRoot(int id) const
  {
    auto root = getRootObject();
    return root.lock()->getObjectFromChildren(id);
  }

//-----------------------------------------------------------------------
//オブジェクト名前関連
//-----------------------------------------------------------------------
  //オブジェクトの名前が一致しているかを調べる
  bool matchingName(const std::string& name) const
  {
    return (name == name_);
  }
  //オブジェクトの名前に指定の文字列含まれているかを調べる
  bool findName(const std::string& name) const
  {
    return (name_.find(name) != std::string::npos);
  }
  //オブジェクトの名前を変更する
  std::string changeName(const std::string& newname)
  {
    std::string oldname = name_;
    name_ = newname;
    return oldname;
  }
  //オブジェクトの名前へ追記する（\0以降に指定の文字列を追加）
  void appendName(const std::string& append)
  {
    name_ += append;
  }

//-----------------------------------------------------------------------
//オブジェクトID関連
//-----------------------------------------------------------------------
  //オブジェクトのIDが一致しているかを調べる
  bool matchingID(int id) const
  {
    return id_ == id;
  }
  //オブジェクトのIDをを収得する
  int ID() const
  {
    return id_;
  }
  //同じオブジェクトかどうかを調べる
  bool isSame(std::weak_ptr<Object>& w) const
  {
    auto temp = ci_ext::weak_to_shared(w);
    if (temp == nullptr) return false;
    return (temp->ID() == ID());
  }

//-----------------------------------------------------------------------
//オブジェクト駆動変更関連
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//オブジェクト破棄関連
//-----------------------------------------------------------------------
  //オブジェクトを破棄する予約をする
  virtual void kill()
  {
    status_ = Status::destroy;
    for (auto& child : children_)
      child->kill();
  }
  //指定した名前のオブジェクトを破棄する予約をする
  void killFromChildren(const std::string& name)
  {
    for (auto& child : children_)
    {
      if (child->name() == name)
        child->kill();
    }
  }
  //指定した名前のオブジェクトを破棄する予約をする
  void killFromRoot(const std::string& name)
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (child->name() == name)
        child->kill();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを破棄する予約をする
  void killFromChildren(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    for (auto& child : children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->kill();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを破棄する予約をする
  void killFromRoot(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->kill();
    }
  }

//-----------------------------------------------------------------------
//オブジェクトを駆動させる
//-----------------------------------------------------------------------
  virtual void run()
  {
    status_ = Status::run;
  }
  //子供を含めたオブジェクトを駆動させる
  virtual void runAll()
  {
    run();
    for (auto& child : children_)
      child->runAll();
  }
  //指定した名前のオブジェクトを駆動させる
  void runFromChildren(const std::string& name)
  {
    for (auto& child : children_)
    {
      if (child->name() == name)
        child->run();
    }
  }
  //指定した名前のオブジェクトを駆動させる
  void runFromRoot(const std::string& name)
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (child->name() == name)
        child->run();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを駆動させる
  void runFromChildren(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    for (auto& child : children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->run();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを駆動させる
  void runFromRoot(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->run();
    }
  }

//-----------------------------------------------------------------------
//オブジェクトをスリープ
//-----------------------------------------------------------------------

  virtual void sleep(int framecount)
  {
    status_ = Status::sleep;
    framecount_ = framecount;
  }
  //子供も含めたスリープ
  void sleepAll(int framecount)
  {
    sleep(framecount);
    for (auto& child : children_)
      child->sleepAll(framecount);
  }
  //指定した名前のオブジェクトをスリープさせる
  void pauseFromChildren(const std::string& name, int framecount)
  {
    for (auto& child : children_)
    {
      if (child->name() == name)
        child->sleep(framecount);
    }
  }
  //指定した名前のオブジェクトをスリープさせる
  void pauseFromRoot(const std::string& name, int framecount)
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (child->name() == name)
        child->sleep(framecount);
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものをスリープさせる
  void pauseFromChildren(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2,
    int framecount)
  {
    for (auto& child : children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->sleep(framecount);
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものをスリープさせる
  void pauseFromRoot(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2,
    int framecount)
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->sleep(framecount);
    }
  }

//-----------------------------------------------------------------------
//オブジェクトを停止させる
//-----------------------------------------------------------------------
  virtual void pause()
  {
    status_ = Status::pause;
  }
  //子供も含めた停止
  void pauseAll()
  {
    pause();
    for (auto& child : children_)
      child->pauseAll();
  }
  //指定した名前のオブジェクトを停止させる
  void pauseFromChildren(const std::string& name)
  {
    for (auto& child : children_)
    {
      if (child->name() == name)
        child->pause();
    }
  }
  //指定した名前のオブジェクトを停止させる
  void pauseFromRoot(const std::string& name)
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (child->name() == name)
        child->pause();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを停止させる
  void pauseFromChildren(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    for (auto& child : children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->pause();
    }
  }
  //objectの名前にfindNamesがふくまれているか、findNotNamesが含まれていないものを停止させる
  void pauseFromRoot(
    const std::vector<std::string>& objectNames1,
    const std::vector<std::string>& objectNotNames2 = {})
  {
    auto root = getRootObject().lock();
    for (auto& child : root->children_)
    {
      if (findObject(child, objectNames1, objectNotNames2))
        child->pause();
    }
  }
  //レジューム
  virtual void resume()
  {
    status_ = Status::run;
  }

//-----------------------------------------------------------------------
//アクセサ
//-----------------------------------------------------------------------
  const std::string& name() const { return name_; }
  bool isDestroy()  const { return (status_ == Status::destroy); }
  bool isDead()  const { return (status_ == Status::dead); }
  bool isRunning()  const { return (status_ == Status::run); }
  bool isSleeping() const  { return (status_ == Status::sleep); }
  bool isPause()    const { return (status_ == Status::pause); }
  Status status() const { return status_; }
  int  priority() const { return priority_; }
  void changePriority(int priority) { priority_ = priority; }


//-----------------------------------------------------------------------
//比較用叙述関数
//-----------------------------------------------------------------------
  static bool greaterPriority(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b)
  {
    return a->priority() > b->priority();
  }
  static bool lessPriority(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b)
  {
    return a->priority() < b->priority();
  }

//-----------------------------------------------------------------------
//メッセージ送信 (2015/04/16,maruyama)
//-----------------------------------------------------------------------

  virtual void receiveMsg(std::weak_ptr<Object>& sender,const std::string& msg){ }
  virtual void receiveMsg(std::weak_ptr<Object>& sender, const std::string& msg,const int num){ }

  void postMsg(std::weak_ptr<Object>& receiver, const std::string& msg){
	  if (isActive(receiver)) {
		  receiver.lock()->receiveMsg(selfPtr(), msg);
	  }
  }
  void postMsg(std::weak_ptr<Object>& receiver, const std::string& msg, const int num){
	  if (isActive(receiver)) {
		  receiver.lock()->receiveMsg(selfPtr(), msg, num);
	  }
  }

  void postMsgAllChildren(const std::string& msg) {
	  for (auto child : children_) {
		  if (child->isRunning()) {
			  std::weak_ptr<Object> obj = child;
			  postMsg(obj, msg);
			  child.get()->postMsgAllChildren(msg);
		  }
	  }
  }
  void postMsgAllChildren(const std::string& msg, const int num) {
	  for (auto child : children_) {
		  if (child->isRunning()) {
			  std::weak_ptr<Object> obj = child;
			  postMsg(obj, msg, num);
			  child.get()->postMsgAllChildren(msg, num);
		  }
	  }
  }

  void postMsgCurrentAndAllChildren(const std::string& msg) {
	  receiveMsg(selfPtr(), msg);

	  for (auto child : children_) {
		  if (child->isRunning()) {
			  std::weak_ptr<Object> obj = child;
			  postMsg(obj, msg);
			  child->postMsgAllChildren(msg);
		  }
	  }
  }
  void postMsgCurrentAndAllChildren(const std::string& msg,const int num) {
	  receiveMsg(selfPtr(), msg, num);

	  for (auto child : children_) {
		  if (child->isRunning()) {
			  std::weak_ptr<Object> obj = child;
			  postMsg(obj, msg, num);
			  child->postMsgAllChildren(msg, num);
		  }
	  }
  }

  // 有効かつrun状態であればtrue
  static bool isActive(std::weak_ptr<Object> object) {
	  return !object.expired() && object.lock()->isRunning();
  }

//-----------------------------------------------------------------------
//render関連
//-----------------------------------------------------------------------
  //ルート以外で呼ばないでください
  void renderWithChildren()
  {
    render();
    //TODO: Z軸及びアルファを考慮した描画順を構築すること
    renderSelect();
  }
  //overrideして各ステータスのrenderを記述する
  virtual void render() {};     //通常描画処理
  virtual void renderPause() {} //ポーズ中の処理
  virtual void renderSleep() {} //スリープ中の処理
  virtual void renderDestroy() {} //削除予約中の処理

//-----------------------------------------------------------------------
//update関連
//-----------------------------------------------------------------------
  //ルート以外で呼ばないでください
  void updateWithChildren()
  {
    update();
    updateChildObjects();
  }
  //overrideして各ステータスのupdateを記述する
  virtual void update() {};     //通常処理
  virtual void updatePause() {} //ポーズ中の処理
  virtual void updateSleep() {} //スリープ中の処理
  virtual void updateDestroy() {} //削除予約中の処理

//-----------------------------------------------------------------------
//あたり判定用（ゲームPG教材用に）
//-----------------------------------------------------------------------
  //やられ判定に攻撃を食らった
  virtual void defenseHit(std::weak_ptr<ci_ext::Object>&) {}
  //攻撃判定により攻撃を与えた
  virtual void offenseHit(std::weak_ptr<ci_ext::Object>&) {}

//-----------------------------------------------------------------------
//あたり判定用（ゲームPG教材用に）
//-----------------------------------------------------------------------
  //コンストラクタでは親子関係が構築できない仕様なので、
  //生成時に子供を登録したい場合はinit()で行う
  virtual void init() {}



private:
  //親子関係構築用
  std::weak_ptr<Object> selfPtr_;   //自分自身のweak_ptr
  std::weak_ptr<Object> parentPtr_; //親のweak_ptr
  //子供タスク登録できるようにする
  std::vector<std::shared_ptr<Object>> children_; //子供オブジェクト
  std::vector<std::shared_ptr<Object>> ins_;      //追加するオブジェクト

  static int uid_;     //ユニークなID、オブジェクトを生成するとインクリメントされる
  int id_;             //オブジェクトID、このIDでオブジェクトを特定したり検索をかけることができる
  int priority_;       //優先順位、現在未使用
  std::string name_;   //オブジェクトの名前、この名前で検索をかけることができる
  int hierarchyLevel_; //どの階層にいるか。ルートは1、子供になるにつれ+1していく
  int framecount_;     //寝かせるフレーム数、通常駆動時は0
  Status status_;      //駆動ステータス

  //親子関係構築用：親のweak_ptrを設定する
  void setParentPtr(const std::weak_ptr<Object>& w)
  {
    parentPtr_ = w;
  }
  //生成時にユニークIDを設定する
  void initID()
  {
    ++uid_;
    id_ = uid_;
    //      std::ofstream f("a.txt", std::ofstream::app);
    //      f << name_ << std::endl;
  }
  //sleepからrunへのチェック用
  void awake()
  {
    if (--framecount_ < 0) 
      resume();
  }

//-----------------------------------------------------------------------
//update関連
//１．ルートのみupdateWithChildren()を呼び、オブジェクトの親子関係の駆動をはじめる
//２．updateWithChildren()がupdateChildObjects()を呼び子供オブジェクトの駆動を行う
//    updateChildObjects()内でオブジェクトの削除、追加が行われる
//    その際updateSelect()が駆動ステータスによって各々のupdate()を呼び出す
//３．プログラマは各オブジェクトごとにupdate()を作成しなければならない
//-----------------------------------------------------------------------
  void updateWithChildrenNormal()
  {
    update();
    updateChildObjects();
  }
  void updatePauseWithChildren()
  {
    updatePause();
    updateChildObjects();
  }
  void updateSleepWithChildren()
  {
    updateSleep();
    awake();
    updateChildObjects();
  }
  void updateDestroyWithChildren()
  {
    status_ = Status::dead;
    updateDestroy();
    updateChildObjects();
  }
  void updateSelect()
  {
    switch (status())
    {
      case Status::run:
        updateWithChildrenNormal();
        break;
      case Status::pause:
        updatePauseWithChildren();
        break;
      case Status::sleep:
        updateSleepWithChildren();
        break;
      case Status::destroy:
        updateDestroyWithChildren();
        break;
    }
  }
  void updateChildObjects()
  {
    for (auto& child : children_)
    {
      child->updateSelect();
    }

    //オブジェクトの消去及び追加
    children_.erase(
      std::remove_if(children_.begin(), children_.end(),
      std::mem_fn(&Object::isDead)),
      children_.end()
      );
    insert();
  }
  //プールされているオブジェクトを追加する
  void insert()
  {
    if (!ins_.empty())
    {
      children_.insert(children_.end(), ins_.begin(), ins_.end());
      for (auto& ins : ins_)
        ins->init();
    }
    ins_.clear();
  }

//-----------------------------------------------------------------------
//render関連
//１．ルートのみrenderWithChildren()を呼び、オブジェクトの親子関係の描画をはじめる
//    アルファブレンド等で不具合が出る場合は、別途描画順を構築しなければならない
//２．プログラマは各オブジェクトごとにrender()を作成しなければならない
//-----------------------------------------------------------------------
  void renderWithChildrenNormal()
  {
    render();
    renderChildObjects();
  }
  void renderPauseWithChildren()
  {
    renderPause();
    renderChildObjects();
  }
  void renderSleepWithChildren()
  {
    renderSleep();
    renderChildObjects();
  }
  void renderDestroyWithChildren()
  {
    renderDestroy();
    renderChildObjects();
  }
  void renderSelect()
  {
    switch (status())
    {
      case Status::run:
        renderWithChildrenNormal();
        break;
      case Status::pause:
        renderPauseWithChildren();
        break;
      case Status::sleep:
        renderSleepWithChildren();
        break;
      case Status::destroy:
        renderDestroyWithChildren();
        break;
    }
  }
  void renderChildObjects()
  {
    for (auto& child : children_)
    {
      child->renderSelect();
    }
  }
};


}
