//このヘッダをインクルードし、ShowConsole()を呼ぶことで、デバッグ用コンソールが出現します。
//printf() scanf() cin coutなど、標準的な入出力が可能となります
//<注意>
//ShowConsole()がスコープから抜けると、コンソールは破棄されます！

#pragma once
#include <windows.h>
#include <cstdio>
#include <iostream>
namespace ci_ext
{

//Win32アプリでcin、coutを許可するクラス
class Console
{
  std::FILE* in_;
	std::FILE* out_;
public:
  Console()
  {
    AllocConsole();
    freopen_s(&out_, "CONOUT$", "w", stdout); //標準出力をコンソールにする
    freopen_s(&in_,  "CONIN$" , "r", stdin);  //標準入力をコンソールにする
  }
  ~Console()
  {
    std::fclose(in_);
    std::fclose(out_);
    FreeConsole();
  }
};

//coutでデバッグウインドウに出力できるようになるクラス
class coutDbgString
{
	class DbgStreambuf : public std::streambuf
	{
	public:
		std::streamsize xsputn(const char* s, std::streamsize n)
		{
			OutputDebugString(s);
			return n;
		}
		int_type overflow(int_type c = EOF)
		{
			if(c != EOF)
			{
				char buf[2] = {c, '\0'};
				OutputDebugString(buf);
			}
			return c;
		}
	};
	std::streambuf* default_stream;
	DbgStreambuf debug_stream;
public:
	coutDbgString()
	{
		default_stream = std::cout.rdbuf(&debug_stream);
	}
	~coutDbgString()
	{
		std::cout.rdbuf(default_stream);
	}
};

#ifdef _DEBUG
//複数はサポートしない
# define ShowConsole() ci_ext::Console c
# define StartOutputDbgString() ci_ext::coutDbgString c
#else
# define ShowConsole() __noop
# define StartOutputDbgString() __noop
#endif

#define dout std::cout << __FUNCTION__ << ":"
#define FILENAME_AND_LINE __FILE__ << ":" << __LINE__
}