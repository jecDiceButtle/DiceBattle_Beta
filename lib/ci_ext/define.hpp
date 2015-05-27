#pragma once

namespace ci_ext
{

//#define PI 3.1415926535897323

  //VisualStudo2012以前には nullptr が無いので定義
#if (_MSC_VER<1700)
  const class {
  public:
      template<class T>
      operator T*() const { return 0; }
      // あらゆる型のメンバポインタへの変換
      template<class C, class T>
      operator T C::*() const  { return 0; }
  private:
      void operator&() const;
  } nullptr;
#endif

}