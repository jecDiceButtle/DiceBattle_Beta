#pragma once

#include "object.hpp"
#include <chrono>

namespace ci_ext
{

//d�ɂ�100�i�m�b�P�ʂŎw��
template <typename T = float>
inline T changeMS(const __int64& d)
{
  return d / static_cast<T>(10000000);
}

template <typename T = float>
class TimeCounter : public ci_ext::Object
{
  using system_clock = std::chrono::system_clock;
  system_clock::time_point createTimePoint_; //�������s��ꂽ����
  system_clock::time_point intervalTimePoint_;//�~�܂��Ă�������

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
  //���̃I�u�W�F�N�g���ł������ɌĂ΂��A�R���X�g���N�^
  TimeCounter(const std::string& taskname)
    :
    Object(taskname),
    intervalSec_(),
    intervalTotalSec_()
  {
    init();
  }

  //���̃I�u�W�F�N�g������������j�������Ƃ��ɌĂ΂��A�f�X�g���N�^
  virtual ~TimeCounter() override
  {
  }

  //�P�t���[���ōs���\������
  virtual void render() override
  {
  }

  //�P�t���[���ōs������
  virtual void update() override
  {
  }

  //Pause�ڍs���Ɏ��Ԃ��擾���Ă���
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

  //pause�i�s���̎��Ԃ��v�����Ă���
  virtual void updatePause() override
  {
    updateIntervalTime();
    Object::updatePause();
  }

  //�o�ߎ��Ԏ擾
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
  T period_; //�J�E���^�[�̊Ԋu�����b
  TimeCounter<T> tm_;
public:
  Counter(float second)
    :
    period_(second),//�b����~���b�֕ύX
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