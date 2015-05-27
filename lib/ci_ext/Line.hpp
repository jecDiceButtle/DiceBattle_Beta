//S.Ueyema wrote...

#pragma once

#include "vec3.hpp"
#include "math.hpp"
#include "color.hpp"
#include <vector>
#include <assert.h>
namespace ci_ext
{

template<typename T>
class Line
{
  std::vector<Vec3<T>> points_;
  Line()
  {
    points_.clear();
    points_.push_back(Vec3<T>());
    points_.push_back(Vec3<T>());
  }
public:
  Line(T point1X, T point1Y, T point2X, T point2Y)
  {
    points_.clear();
    points_.push_back(Vec3<T>(point1X, point1Y));
    points_.push_back(Vec3<T>(point2X, point2Y));
  }
  Line(const Vec3<T>& point1, const Vec3<T>& point2)
  {
    points_.clear();
    points_.reserve(2);
    points_.push_back(point1);
    points_.push_back(point2);
  }
  Line(std::vector<Vec3<T>> points)
  {
    if(points.empty()) assert(0);
    points_.clear();
    points_ = points;
  }
  Vec3<T> getPoint(size_t i) const
  {
    if(i >= points_.size()) assert(0);
    return Vec3<T>(points_[i]);
  }
  void draw(Color color = 0xFFFFFFFF, int size = 1) const
  {
		graph::Draw_Line(points_[0].ix(), points_[0].iy(),
              points_[1].ix(), points_[1].iy(),
              points_[0].z(),
              color, size); 
  }
};

template<typename T>
bool CheckCrossLine(const Line<T>& point1,
                    const Line<T>& point2,
                    Vec3<T>* cross = 0)
{
  Vec3<T> p1 = point1.getPoint(0),
          p2 = point1.getPoint(1),
          p3 = point2.getPoint(0),
          p4 = point2.getPoint(1);
  return CheckCrossLine(p1, p2, p3, p4, cross); 
}

typedef Line<float>  Linef;
typedef Line<double> Lined;


}