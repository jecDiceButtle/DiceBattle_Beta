#include "timer.h"
#include "../../lib/gplib.h"
#include <iostream>
#include <string>

namespace game
{

Timer::Timer(const std::string& objectName)
  :
  Object(objectName)
{
}

void Timer::init()
{
  limitTimer_ = insertAsChild(new ci_ext::TimeCounter<float>(""));
}

void Timer::render()
{
  auto limitTimer = ci_ext::weak_to_shared<ci_ext::TimeCounter<float>>(limitTimer_);
  std::string name(std::to_string(limitTimer->get()));
  gplib::font::Draw_FontText(0, 0, 0, name, -1, 0);
}

void Timer::update()
{
}

float Timer::get() const
{
  auto limitTimer = ci_ext::weak_to_shared<ci_ext::TimeCounter<float>>(limitTimer_);
  return limitTimer->get();
}

}