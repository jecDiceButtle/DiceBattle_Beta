#pragma once

#include "object.hpp"
#include <chrono>

namespace ci_ext
{

//dには100ナノ秒単位で指定
template <typename T = float>
inline T changeMS(const __int64& d)
{
  return d / static_cast<T>(10000000);
}

template <typename T = float>
class TimeCounter : public ci_ext::Object
{
  using system_clock = std::chrono::system_clock;
  system_clock::time_point createTimePoint_; //生成が行われた時間
  system_clock::time_point intervalTimePoint_;//止まっていた時間

  float durationSec_, intervalSec_, intervalTotalSec_;

  void updateIntervalTime()
  {
    auto d = std::chrono::system_clock::now() - intervalTimePoint_;
    intervalSec_ = changeMS(d.count());
  }

public:
  void init()
  {
    createTimePoint_ = std::chrono::system_clock::now();
    intervalTimePoint_ = std::chrono::system_clock::time_point();
  }
  //このオブジェクトができた時に呼ばれる、コンストラクタ
  TimeCounter(const std::string& taskname)
    :
    Object(taskname),
    intervalSec_(),
    intervalTotalSec_()
  {
    init();
  }

  //このオブジェクトがメモリから破棄されるときに呼ばれる、デストラクタ
  virtual ~TimeCounter() override
  {
  }

  //１フレームで行う表示処理
  virtual void render() override
  {
  }

  //１フレームで行う処理
  virtual void update() override
  {
  }

  //Pause移行時に時間を取得しておく
  virtual void pause() override
  {
    if (!isPause())
      intervalTimePoint_ = system_clock::now();
    Object::pause();
  }

  //
  virtual void run() override
  {
    intervalTotalSec_ += intervalSec_;
    intervalSec_ = 0;
    Object::run();
  }

  //pause進行中の時間を計測しておく
  virtual void updatePause() override
  {
    updateIntervalTime();
    Object::updatePause();
  }

  //経過時間取得
  T get()
  {
    auto d = system_clock::now() - createTimePoint_;
    durationSec_ = changeMS(d.count());
    return durationSec_ - intervalSec_ - intervalTotalSec_;
  }
};

template <typename T = float>
class Counter
{
  T period_; //カウンターの間隔周期秒
  TimeCounter<T> tm_;
public:
  Counter(float second)
    :
    period_(second),//秒からミリ秒へ変更
    tm_("Counter")
  {
  }
  bool run()
  {
    T ms = tm_.get();
    if (ms >= period_)
    {
      tm_.init();
      return true;
    }
    return false;
  }
  bool operator ()()
  {
    return run();
  }
};
}