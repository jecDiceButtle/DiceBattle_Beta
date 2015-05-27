#pragma once

#include <vector>
namespace ci_ext
{

template<typename T>
T DegreeToRadian(T degree)
{
  return degree * static_cast<T>(M_PI) * static_cast<T>(2) / static_cast<T>(360);
}
template<typename T>
T RadianToDegree(T radian)
{
  return radian / static_cast<T>(M_PI) * static_cast<T>(180);
}

template<typename T>
T RotateX(T radian, T radius, T center)
{
  return cos(radian) * radius + center;
}
template<typename T>
T RotateY(T radian, T radius, T center)
{
  return -sin(radian) * radius + center;
}

template<typename T>
T RotateXDegree(T degree, T radius, T center)
{
  return RotateX(DegreeToRadian(degree), radius, center);
}
template<typename T>
T RotateYDegree(T degree, T radius, T center)
{
  return RotateY(DegreeToRadian(degree), radius, center);
}

template<typename T>
Vec3<T> Rotate(const Vec3<T>& pos, const Vec3<T>& center, T radian)
{
  return Vec3<T>(pos.x() * std::cos(-radian) - pos.y() * std::sin(-radian) + center.x(),
                 pos.x() * std::sin(-radian) + pos.y() * std::cos(-radian) + center.y());
}

template<typename T>
Vec3<T> RotateDegree(const Vec3<T>& pos, const Vec3<T>& center, T degree)
{
  return Rotate(pos, center, DegreeToRadian(degree));
}

template<typename T>
T Distance2(const Vec3<T>& a, const Vec3<T>& b)
{
  T x = pow(a.x() - b.x(), 2);
  T y = pow(a.y() - b.y(), 2);
  return sqrt(x + y);
}

template<typename T>
T Distance2(T x1, T y1, T x2, T y2)
{
  return Distance2(Vec3<T>(x1, y1), Vec3<T>(x2, y2));
}

template<typename T>
T Distance3(const Vec3<T>& a, const Vec3<T>& b)
{
  T x = pow(a.x() - b.x(), 2);
  T y = pow(a.y() - b.y(), 2);
  T z = pow(a.z() - b.z(), 2);
  return sqrt(x + y + z);
}

template<typename T>
T Distance3(T x1, T y1, T z1, T x2, T y2, T z2)
{
  return Distance2(Vec3<T>(x1, y1, z1), Vec3<T>(x2, y2, z2));
}

template<typename T>
T DotProduct(const Vec3<T>& a, const Vec3<T>& b)
{
  return (a.x() * b.x() + a.y() * b.y() + a.z() + b.z());
}

template<typename T>
T Length(const Vec3<T>& a)
{
  return sqrt(pow(a.x(), 2) + pow(a.y(), 2) + pow(a.z(), 2));
}

template<typename T>
T RadianOf2Vector(const Vec3<T>& a, const Vec3<T>& b)
{
  T lenA = Length(a);
  T lenB = Length(b);

  if(lenA == 0 || lenB == 0)
    assert(0);
  //  throw "長さ0では演算できません";

  return acos(DotProduct(a, b) / (lenA * lenB));
}

template<typename T>
T DegreeOf2Vector(const Vec3<T>& a, const Vec3<T>& b)
{
  return RadianToDegree(RadianOf2Vector(a, b));
}

template<typename T>
T RadianOfPoints2(const Vec3<T>& basis, const Vec3<T>& target)
{
  T x =  target.x() - basis.x();
  T y = -(target.y() - basis.y());  //ディスプレイ座標に合わせるために、マイナスにする
  return atan2(y, x);
}

template<typename T>
T DegreeOfPoints2(const Vec3<T>& basis, const Vec3<T>& target)
{
  return RadianToDegree(RadianOfPoints2(basis, target));
}

template<typename T>
T CrossProduct(const Vec3<T>& a, const Vec3<T>& b)
{
  return (a.x() * b.y() - a.y() * b.x());
}

//三角形頂点a->b->cは反時計回りで指定のこと
template<typename T>
bool CheckPointInTriangle(const Vec3<T>& p, const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& c)
{
  if(CrossProduct(p - a, b - a) < 0.0) return false;
  if(CrossProduct(p - b, c - b) < 0.0) return false;
  if(CrossProduct(p - c, a - c) < 0.0) return false;
  return true;
}
//三角形頂点a->b->cは反時計回りで指定のこと
template<typename T>
bool CheckPointInTriangle(const Vec3<T>& p, const std::vector<Vec3<T>>& triangle)
{
  if(CrossProduct(p - triangle[0], triangle[1] - triangle[0]) < 0.0) return false;
  if(CrossProduct(p - triangle[1], triangle[2] - triangle[1]) < 0.0) return false;
  if(CrossProduct(p - triangle[2], triangle[0] - triangle[2]) < 0.0) return false;
  return true;
}
/*
template<typename T>
class Ellipse
{
  Vec3<T> center_;
  T radius_;
  Ellipse() {}
public:
  Ellipse(Vec3<T> center, T radius):
    center_(center),
    radius_(radius) {}
};
*/
template<typename T>
int ChackPointLine(const Vec3<T>& lineStartPoint, const Vec3<T>& lineEndPoint, const Vec3<T>& point)
{
  Vec3<T> v1(lineEndPoint.x() - lineStartPoint.x(), lineEndPoint.y() - lineStartPoint.y());
  Vec3<T> v2(point.x() - lineStartPoint.x(), point.y() - lineStartPoint.y());
  int retval;

  T cross = CrossProduct(v1, v2);
  if(cross > 0)
  {
    retval = 1;
  }
  else if (cross < 0)
  {
    retval = -1;
  }
  else
  {
    retval = 0;
  }
  return retval;
}

/*////////////////////////////////////////////////////////////////////
//  線と線のあたりを調べる
//  ＜戻り値＞
//    ture  あたっている
//    fasle  あたっていない
/*////////////////////////////////////////////////////////////////////
template<typename T>
bool CheckCrossLine(const Vec3<T>& a1, const Vec3<T>& a2,
                    const Vec3<T>& b1, const Vec3<T>& b2)
{
  if(ChackPointLine(a1, a2, b1) != ChackPointLine(a1, a2, b2))
  {
    if(ChackPointLine(b1, b2, a1) != ChackPointLine(b1, b2, a2))
    {
      return true;
    }
  }
  return false;
}

/*////////////////////////////////////////////////////////////////////
//  線と線のあたりを調べる
//  crossにnullptr以外を入れた場合は、その交点を代入
//  ＜戻り値＞
//    ture  あたっている
//    fasle  あたっていない
/*////////////////////////////////////////////////////////////////////
template<typename T>
bool CheckCrossLine(const Vec3<T>& a1, const Vec3<T>& a2,
                    const Vec3<T>& b1, const Vec3<T>& b2,
                    Vec3<T>* cross = 0)
{
  T b, a, d;
  T ramda, mu;
  bool ret = false;

  b = (b2.y() - b1.y()) * (b2.x() - a1.x()) - 
      (b2.x() - b1.x()) * (b2.y() - a1.y());

  a = (a2.x() - a1.x()) * (b2.y() - a1.y()) -
      (a2.y() - a1.y()) * (b2.x() - a1.x());

  d = (a2.x() - a1.x()) * (b2.y() - b1.y()) -
      (a2.y() - a1.y()) * (b2.x() - b1.x());

  ramda = b / d;
  mu    = a / d;

  if((ramda >= 0 && ramda <= 1) &&
     (mu >= 0 && mu <= 1))
  {
    if(cross != 0)
      cross->set(a1.x() + ramda * (a2.x() - a1.x()),
                 a1.y() + ramda * (a2.y() - a1.y()));
    ret = true;
  }
  return ret;
}

template<typename T>
T	SetRandomValue(int max, int min)
{
	return 	static_cast<T>((rand()%(max))+min);
}

}