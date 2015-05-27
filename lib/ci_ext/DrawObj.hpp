#pragma once
#include "vec3.hpp"
#include "color.hpp"
#include "../gplib.h"
#include <string>

namespace ci_ext
{

//描画オブジェクト
template<typename T>
class DrawObj : public ci_ext::Object
{
protected:
  Vec3<T> pos_;
  Vec3<T> scale_;
  std::string resname_;
  Vec3<T> src_;
  Vec3<T> srcSize_;
  Color color_;
  T degree_;
  bool draw_;

public:

  virtual ~DrawObj() {}
  DrawObj() = delete;
  
	//名前のみで生成
	DrawObj(	const std::string& objectName		)
		:
		pos_(Vec3f::one()),
		scale_(Vec3f::one()),
		resname_(objectName),
		src_(Vec3f::one()),
		srcSize_(Vec3f::one()),
		color_(Color(0xFFFFFFFF)),
		degree_(0),
		draw_(false),
		Object(objectName)
	{
	}

  //自分自身から
  DrawObj(
    const std::string& objectName,
    const DrawObj& o)
    :
    pos_(o.pos_),
    scale_(o.scale_),
    resname_(o.resname_),
    src_(o.src_),
    srcSize_(o.srcSize_),
    color_(o.color_),
    degree_(o.degree_),
    draw_(o.draw_),
    Object(objectName)
  {
  }
  //リソース名から
  DrawObj(
    const std::string& objectName,
    const Vec3<T>& pos, const Vec3<T>& scale,
    const std::string& resname,
    const Vec3<T>& src, const Vec3<T>& srcSize,
    const Color& color, const T degree)
    :
    pos_(pos),
    scale_(scale),
    resname_(resname),
    src_(src),
    srcSize_(srcSize),
    color_(color),
    degree_(degree),
    draw_(true),
    Object(objectName)
  {}

  //ファイル名から
  DrawObj(
    const std::string& objectName,
    const Vec3<T>& pos, const Vec3<T>& scale,
    const std::string& resname,
    const std::string& filename,
    const Vec3<T>& src, const Vec3<T>& srcSize,
    const Color& color, const T degree)
    :
    pos_(pos),
    scale_(scale),
    resname_(resname),
    src_(src),
    srcSize_(srcSize),
    color_(color),
    degree_(degree),
    draw_(true),
    Object(objectName)
  {
    gplib::graph::Draw_LoadObject(resname, filename);
  }

  void invisible() { draw_ = false; }
  void visible()   { draw_ = true; }
  void changeVisible(bool f) { draw_ = f; }

  void render() override
  {
    if (!draw_) return;

    gplib::graph::Draw_Graphics(
      pos_.ix(), pos_.iy(), pos_.z(),
      resname_,
      src_.ix() * srcSize_.ix(), src_.iy()*srcSize_.iy(),
      srcSize_.ix(), srcSize_.iy(),
      degree_,
      scale_.x(), scale_.y(),
      color_.a(), color_.r(), color_.g(), color_.b());
  }

  void update() override{}

  void setPos(const Vec3<T>& pos)
  {
    pos_ = pos;
  }

  void setPos(const T x, const T y, const T z)
  {
    pos_ = Vec3<T>(x, y, z);
  }

  void offsetPos(const Vec3<T>& offset)
  {
    pos_ += offset;
  }

  void setScale(const Vec3<T>& scale)
  {
    scale_ = scale;
  }

  void offsetScale(const Vec3<T>& offset)
  {
    scale_ += offset;
  }

  void setSrc(const Vec3<T>& src)
  {
    src_ = src;
  }

  void setSrcSize(const Vec3<T>& size)
  {
    srcSize_ = size;
  }

  void setSrcInfo(const Vec3<T>& src, const Vec3<T>& size)
  {
    src_ = src;
    srcSize_ = size;
  }

  void setDegree(const T degree)
  {
    degree_ = degree;
  }

  void offsetDegree(const T offset)
  {
    T temp0 = static_cast<T>(0);
    T temp360 = static_cast<T>(360);
    degree_ += offset;
    if (degree_ > temp360)    degree_ -= temp360;
    else if (degree_ < temp0) degree_ += temp360;
  }

  void setColor(const Color& color)
  {
    color_ = color;
  }
  void offsetColor(const Color& offset)
  {
    color_ += offset;
  }
  void setColor(
    unsigned int a, unsigned int r,
    unsigned int g, unsigned int b)
  {
    color_.set(a, r, g, b);
  }
  void offsetColor(
    unsigned int a, unsigned int r,
    unsigned int g, unsigned int b)
  {
    color_.offset(a, r, g, b);
  }

  template<typename T1>
  Vec3<T1> halfsize() const
  {
    return Vec3<T1>(
      static_cast<T1>(scale().x() * srcSize().x() / 2),
      static_cast<T1>(scale().y() * srcSize().y() / 2)
      );
  }
  template<typename T1>
  Vec3<T1> size() const
  {
    return Vec3<T1>(
      static_cast<T1>(scale().x() * srcSize().x()),
      static_cast<T1>(scale().y() * srcSize().y())
      );
  }
  RECT makeDrawRect() const
  {
    auto size = halfsize<long>();
    RECT rc =
    {
      pos().ix() - size.x(),
      pos().iy() - size.y(),
      pos().ix() + size.x(),
      pos().iy() + size.y(),
    };
    return rc;
  }
  Vec3<T> pos()     const { return pos_; }
  Vec3<T> scale()   const { return scale_; }
  Vec3<T> src()     const { return src_; }
  Vec3<T> srcSize() const { return srcSize_; }
  T degree()        const { return degree_; }
  Color color()     const { return color_; }
  T x()             const { return pos_.x(); }
  T y()             const { return pos_.y(); }
  T z()             const { return pos_.z(); }
  int ix()          const { return pos_.ix(); }
  int iy()          const { return pos_.iy(); }
  int iz()          const { return pos_.iz(); }
  DrawObj obj()     const { return *this; }
  int idegree()     const { return static_cast<int>(degree_); }
  std::string resname() const { return resname_; }
//オフセット値考慮の矩形生成
  RECT makeDrawRect(int startx,int starty,int endx,int endy) const
  {
    auto size = halfsize<long>();
    RECT rc =
    {
      pos().ix() - size.x() + startx,
      pos().iy() - size.y() + starty,
      pos().ix() + size.x() - endx,
      pos().iy() + size.y() - endy,
    };
    return rc;
  }

  //void kill(){
  //  Object::kill();
  //}
};
//typedef DrawObj<float> DrawObjf;
using DrawObjf = DrawObj < float > ;

}