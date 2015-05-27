#pragma once
#include<ctime>
#include<iostream>
namespace ci_ext
{

class MeasureTime
{
  clock_t tm_;
  std::ostream& o_;
public:
  MeasureTime() :
    o_(std::cout)
  {
    tm_ = std::clock();
  }
  ~MeasureTime()
  {
    double tm1 = static_cast<double>(clock() - tm_) / static_cast<double>(CLOCKS_PER_SEC);
    o_ << tm1 << std::endl;
  }
};

}