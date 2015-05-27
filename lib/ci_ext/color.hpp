#pragma once
#include <vector>
namespace ci_ext
{

	
/*
//‚Â‚©‚Á‚Ä‚È‚¢‚æ
union ColorU
{
  unsigned int val;
  struct 
  {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
  };
};
*/

template<typename T>
unsigned int GetAlpha(T color)
{
  return (color >> 24) & 0xFF;
}

template<typename T>
unsigned int GetRed(T color)
{
  return (color >> 16) & 0xFF;
}

template<typename T>
unsigned int GetGreen(T color)
{
  return (color >> 8) & 0xFF;
}

template<typename T>
unsigned int GetBlue(T color)
{
  return color & 0xFF;
}

template<typename T>
unsigned int MakeColor(T a, T r, T g, T b)
{
  int color = ((a & 0xFF) << 24) |
              ((r & 0xFF) << 16) |
              ((g & 0xFF) << 8)  |
               (b & 0xff);
  return color;
}

class Color
{
	int a_, r_, g_, b_;
  Color() {}

public:
  Color(const unsigned int val)
  {
    set(val);
  }
  Color(unsigned int a, unsigned int r, unsigned int g, unsigned int b)
  {
    set(a, r, g, b);
  }

	Color(const std::vector<int>& color)
	{
		set(color);
	}

  unsigned int val() const { return MakeColor(a_, r_, g_, b_); }

	operator int()           const { return val(); }
  operator unsigned int()  const { return val(); }
  operator long()          const { return val(); }
  operator unsigned long() const { return val(); }

	unsigned int a() const { return a_; }
  unsigned int r() const { return r_; }
  unsigned int g() const { return g_; }
  unsigned int b() const { return b_; }
  void a(unsigned int a) { a_ = (a & 0xFF); }
  void r(unsigned int r) { r_ = (r & 0xFF); }
  void g(unsigned int g) { g_ = (g & 0xFF); }
  void b(unsigned int b) { b_ = (b & 0xFF); }
  void offseta(int a) {	a_ = offset(a_, a); }
  void offsetr(int r) { r_ = offset(r_, r); }
  void offsetg(int g) { g_ = offset(g_, g); }
  void offsetb(int b) { b_ = offset(b_, b); }

	void set(unsigned int val) 
	{
		a_ = GetAlpha(val);
		r_ = GetRed(val);
		g_ = GetGreen(val);
		b_ = GetBlue(val);
	}
  void offset(int a, int r, int g, int b)
	{
		offseta(a);
		offsetr(r);
		offsetg(g);
		offsetb(b);
	}
	void set(unsigned int a, unsigned int r, unsigned int g, unsigned int b)
	{
		a_ = a;
		r_ = r;
		g_ = g;
		b_ = b;
	}
	void set(const std::vector<int>& color)
  {
    a_ = r_ = g_ = b_ = 0;
		if (color.size() >= 1)
		{
			a_ = color[0];
			if (color.size() >= 2)
			{
				r_ = color[1];
				if (color.size() >= 3)
				{
					g_ = color[2];
					if (color.size() >= 4)
						b_ = color[3];
				}
			}
		}
	}

  Color& operator=(unsigned int color)
  {
    set(color);
    return *this;
  }

	int offset(int color, int offset)
	{
		color += offset;
		if(color >= 256)   color = 255;
		else if(color < 0) color = 0;
		return color;
	}

};


}