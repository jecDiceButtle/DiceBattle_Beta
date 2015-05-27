//S.Ueyema wrote...

#pragma once

#include "Vec3.hpp"
#include "Line.hpp"
#include <vector>
#include <assert.h>

namespace ci_ext
{

template<typename T>
class Triangle
{
  std::vector<Vec3<T>> points_;
public:
  Triangle() :
    points_(3)
  {
    points_[0] = Vec3<T>();
    points_[1] = Vec3<T>();
    points_[2] = Vec3<T>();
  }
  Triangle(const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& c) :
    points_(3)
  {
    points_[0] = a;
    points_[1] = b;
    points_[2] = c;
  }
  Triangle(const std::vector<Vec3<T>>& points) :
    points_(points) {}

  Vec3<T> getPoint(size_t n) const 
  {
    if(n >= 3) assert(0);
    return points_[n];
  }
  Line<T> getLine(size_t n) const
  {
    if(n >= 3) assert(0);
    size_t to = n+1 >= 3? 0: n+1;
    return Line<T>(points_[n], points_[to]);
  }
  bool intersect(const Vec3<T>& p) const
  {
    return CheckPointInTriangle(p, points_);
  }
  void draw(Color color = 0xFFFFFFFF, int size = 1) const
  {
    for(size_t i = 0; i < points_.size(); ++i)
    {
      Line<T> line = getLine(i);
      line.draw(color, size);
    }
  }
};

typedef Triangle<float> Trianglef;
typedef Triangle<double> Triangled;

}