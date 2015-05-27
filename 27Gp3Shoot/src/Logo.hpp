#pragma once

#define CHECK

#ifdef CHECK


#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/Counter.hpp"

#include "../../lib/gplib.h"
#include <iostream>
#include "title.hpp"
namespace game
{

class Logo final : public ci_ext::Object
{
  enum class State//シーン切り替え用
  {
    up,    //透明から非透明へ
    stop,  //ロゴ表示
    down,  //非透明から透明へ
    end    //終了処理
  };
  State state_;  //現在のシーン
  int alpha_;    //非透明度
  int delta_;    //加算減算量
  std::weak_ptr<ci_ext::Object> timer_; // 制限時間を管理
  std::string resname_;

  //表示させる時間を秒で設定
  const float waitSecond_ = 1.0f;     //非表示からフェードイン開始までの秒数
  const float chhangeAlphaSec_ = 0.6f;//フェードインフェードアウトの秒数
  const float drawLogoSec_ = 2.0f;    //ロゴを表示している時間

  void changeState(State nextState)
  {
    using std::chrono::system_clock;
    using std::chrono::seconds;
    switch (nextState)
    {
    case State::up:
      alpha_ = alpha_ < 0 ? 0 : alpha_;
      break;

    case State::stop:
      //非透明になったら、ロゴ表示に変える
      alpha_ = 255;
      break;

    case State::down:
      alpha_ = alpha_ > 255 ? 255 : alpha_;
      break;

    case State::end:
      {
        //シーン遷移
        auto p = insertToParent(new Title("Title"));
//        p.lock()->init();
      }
      kill();
      break;
    }
    state_ = nextState;
  }
public:
  //このオブジェクトができた時に呼ばれる、コンストラクタ
  Logo(const std::string& taskname)
    :
    Object(taskname),
    state_(State::up),
    alpha_(0),
    delta_(2),
    resname_("logo")
  {
    gplib::graph::Draw_LoadObject(resname_, "res/gra/logo.png",0);
  }

  void init() override
  {
    timer_ = insertAsChild(new ci_ext::TimeCounter<float>(name() + "timecounter_"));
  }

  //このオブジェクトがメモリから破棄されるときに呼ばれる、デストラクタ
  ~Logo() override
  {
//    gplib::graph::Draw_DeleteObject(resname_);
  }

  //１フレームで行う表示処理
  void render() override
  {
    gplib::graph::Draw_Clear();
    gplib::graph::Draw_Graphics(
      gplib::system::WINW / 2, gplib::system::WINH / 2, 0.5f,
      resname_,
      0, 0,
      gplib::graph::Draw_GetImageWidth(resname_), gplib::graph::Draw_GetImageHeight(resname_),
      0,
      1.0f, 1.0f,
      alpha_);
    gplib::graph::Draw_Refresh();
  }

  //１フレームで行う処理
  void update() override
  {
    float ms = 0;
    {
      auto timer = ci_ext::weak_to_shared<ci_ext::TimeCounter<float>>(timer_);
      float temp = (timer->get() - waitSecond_);
      ms = temp < 0 ? 0 : temp;
    }
    //現在のステートによって処理を変更
    switch (state_)
    {
      //非透明度を加算
      case State::up:
        alpha_ = static_cast<int>(255.0f * (ms / chhangeAlphaSec_));
        if (ms > chhangeAlphaSec_)
          changeState(State::stop);
        break;

      //停止中
      case State::stop:
        //規定秒たったら、透明化の処理へ移行
        if (ms > chhangeAlphaSec_ + drawLogoSec_)
          changeState(State::down);
        break;

      //透明へ
    case State::down:
      {
        auto temp = (chhangeAlphaSec_ + chhangeAlphaSec_ + drawLogoSec_) - ms;
        alpha_ = static_cast<int>(255.0f * (temp / chhangeAlphaSec_));
        if (temp < 0.0f)
          changeState(State::end);
      }
      break;
    }
  }

};

}

#endif