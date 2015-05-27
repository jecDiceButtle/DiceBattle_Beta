#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include "math.h"
namespace ci_ext
{

template<typename T = float>
class Vec3
{
  T x_, y_, z_;
public:
  operator D3DXVECTOR3() { return D3DXVECTOR3(x_, y_, z_); }
  static Vec3<T> one()
  {
    return Vec3<T>(1,1,1);
  }
  static Vec3<T> zero()
  {
    return Vec3<T>(0, 0, 0);
  }
  void x(T x) { x_ = x; }
  void y(T y) { y_ = y; }
  void z(T z) { z_ = z; }
  Vec3(T x = 0.0f, T y = 0.0f, T z = 0.0f) :
    x_(x),
    y_(y),
    z_(z) {}
  Vec3(const Vec3<T>& v) :
    x_(v.x()),
    y_(v.y()),
    z_(v.z()) {}
	Vec3(const std::vector<float>& v)
	{
		set(v);
	}
	~Vec3() {}
  T x() const { return x_; }
  T y() const { return y_; }
  T z() const { return z_; }
  int ix() const { return static_cast<int>(x_); }
  int iy() const { return static_cast<int>(y_); }
  int iz() const { return static_cast<int>(z_); }

  Vec3<T>& offset(T xx, T yy, T zz = 0.0f)
  {
    x(x() + xx);
    y(y() + yy);
    z(z() + zz);
    return *this;
  }

  Vec3<T>& offset(const Vec3<T>& p)
  {
    x(x() + p.x());
    y(y() + p.y());
    z(z() + p.z());
    return *this;
  }

  Vec3<T>& set(const Vec3<T>& p)
  {
    x(p.x());
    y(p.y());
    z(p.z());
    return *this;
  }
  Vec3<T>& set(T xx = 0.0f, T yy = 0.0f, T zz = 0.0f)
  {
    x(xx);
    y(yy);
    z(zz);
    return *this;
  }

	Vec3<T>& set(const std::vector<float>& v)
	{
		x_ = y_ = z_ = T(0);
		if (v.size() >= T(1))
			x_ = v[0];
		if (v.size() >= T(2))
			y_ = v[1];
		if (v.size() >= T(3))
			z_ = v[2];
		return *this;
	}

  operator POINT() 
  {
    POINT p = {static_cast<long>(x_), static_cast<long>(y_)};
    return p;
  }

  Vec3<T>& operator=(const Vec3<T>& v)
  {
    set(v);
    return *this;
  }

  Vec3<T>& operator=(const POINT& p)
  {
    set(static_cast<T>(p.x), static_cast<T>(p.y));
    return *this;
  }

  Vec3<T>& operator+=(const Vec3<T>& p)
  {
    x(x() + p.x());
    y(y() + p.y());
    z(z() + p.z());
    return *this;
  }

  Vec3<T> operator+(const Vec3<T>& p) const
  {
    return Vec3<T>(x() + p.x(), y() + p.y(), z() + p.z());
  }

  Vec3<T>& operator-=(const Vec3<T>& p)
  {
    x(x() - p.x());
    y(y() - p.y());
    z(z() - p.z());
    return *this;
  }

  Vec3<T> operator-(const Vec3<T>& p) const
  {
    return Vec3<T>(x() - p.x(), y() - p.y(), z() - p.z());
  }
  bool operator==(const Vec3<T>& p)
  {
    return (x() == p.x() && y() == p.y() && z() == p.z());
  }
  bool operator==(T c)
  {
    return (x() == c && y() == c && z() == c);
  }
  Vec3<T>& translate(const Vec3<T>& translate)
  {
    x( x() + translate.x());
    y( y() + translate.y());
    return *this;
  }
  Vec3<T>& rotate(T radian)
  {
    T xx = x();
    T yy = y();
    x( xx * std::cos(-radian) - yy * std::sin(-radian));
    y( xx * std::sin(-radian) + yy * std::cos(-radian));
    return *this;
  }
  Vec3<T>& rotateDegree(T degree)
  {
    rotate(DegreeToRadian(degree));
    return *this;
  }
  Vec3<T>& scale(const Vec3<T>& scale)
  {
    x(x() * scale.x());
    y(y() * scale.y());
    z(z() * scale.z());
    return *this;
  }
  static Vec3<T>* null() { return static_cast<Vec3<T>*>(nullptr); }
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;

}