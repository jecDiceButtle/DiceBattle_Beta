#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/Counter.hpp"
namespace game
{

class Timer : public ci_ext::Object
{
  std::weak_ptr<ci_ext::Object> limitTimer_; // �������Ԃ��Ǘ�

public:
  Timer(const std::string& objectName);

  //
  void init()   override;
  void render() override;
  void update() override;

  //
  float get() const;
};

}