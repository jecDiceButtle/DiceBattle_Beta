#pragma once
#include <windows.h>
#include "vorbis/vorbisfile.h"
#include "ds2008.h"

//スタティックライブラリ組み込み。
//ライブラリはvcprocから見て、以下のフォルダにあるものとする。
#ifdef _DEBUG
#pragma comment(lib, "../lib/ci_ext/oggvorbis/debug/libogg_static.lib")
#pragma comment(lib, "../lib/ci_ext/oggvorbis/debug/libvorbis_static.lib")
#pragma comment(lib, "../lib/ci_ext/oggvorbis/debug/libvorbisfile_static.lib")
#else
#pragma comment(lib, "./src/lib/ci_ext/oggvorbis/release/libogg_static.lib")
#pragma comment(lib, "./src/lib/ci_ext/oggvorbis/release/libvorbis_static.lib")
#pragma comment(lib, "./src/lib/ci_ext/oggvorbis/release/libvorbisfile_static.lib")
#endif

bool ovIsOgg(const std::string& filename);
int  ovLoadOggSE(int, const std::string& filename);
ci_ext::MYERROR_DS ovLoadOggBGM(int, const std::string& filename, ci_ext::MYSOUNDINFO* s_ptr);
ci_ext::MYERROR_DS ovLoadOggStream(int bufNumber, DWORD iWriteOffset, ci_ext::MYSOUNDINFO* s_ptr);
void ovInitOggEnvAll(ci_ext::MYSOUNDINFO* s_ptr);
void InitOggData(ci_ext::OggVorbis_File* s_ptr);
void ovSeekBegin(ci_ext::OggVorbis_File* s_ptr);


