//--------------------------------------------------------------------------------------------
//	ファイル名	：	OggVorbis.cpp
//	アプリ名	：	汎用的なファイルなので、実行ファイルは不特定
//	作成者		：	植山沙欧
//	作成日		：	2011.03.03
//	機能		：	ds2008にOgg再生を組み込むラッピング関数郡です。
//	使用方法	：
//	使用ヘッダ	：	OggVorbis.h
//	更新履歴	：	2011.03.03	Ver1.00		植山沙欧	基本的な機能の実装
//				2014.01.20	Ver1.01		植山沙欧	ovSeekBeginの追加
//--------------------------------------------------------------------------------------------


#include <memory.h>
#include "ds2008.h"

#ifdef DS_OGG_ENABLED

namespace ci_ext
{

const int OV_BUFFER_SIZE	= 4096;
const int OV_PCM_BITRATE	= 16;
const int OV_PCM_BITRATE_B	= OV_PCM_BITRATE / 8;
const int OV_SUCCESSFUL		= 0;

static void	 ovInitOggEnv(MYSOUNDINFO* s_ptr);
static DWORD ovGetPcmSize(OggVorbis_File* ovf);
static DWORD ovGetPcmSize(const char* filename);
static long  ovRead(OggVorbis_File* ovf_ptr, char* b_ptr,int* bs_ptr);
static int   ovCreateBuffer(OggVorbis_File* ovf_ptr, int bufNumber, const char* filename, DWORD dwSize);

//--------------------------------------------------------------------------------------------
//	Ogg読み込み情報を初期化します
//--------------------------------------------------------------------------------------------
void InitOggData(OggVorbis_File* s_ptr)
{
	if(s_ptr->datasource != NULL){
		ov_clear(s_ptr);
	}
}

//--------------------------------------------------------------------------------------------
//	Ogg読み込み情報を初期化します
//--------------------------------------------------------------------------------------------
static void ovInitOggEnv(OggVorbis_File* s_ptr)
{
	if(s_ptr->datasource != NULL)
	{
		ov_clear(s_ptr);
	}
}

//--------------------------------------------------------------------------------------------
//	Ogg再生を先頭へ戻します
//--------------------------------------------------------------------------------------------
void ovSeekBegin(OggVorbis_File* s_ptr)
{
	ov_pcm_seek(s_ptr, 0);
}
//--------------------------------------------------------------------------------------------
//	Ogg読み込み情報を初期化します
//--------------------------------------------------------------------------------------------
void ovInitOggEnvAll(MYSOUNDINFO* s_ptr)
{
	for(int cnt = 0; cnt < DS_MAX_BUFFER; cnt++)
	{
		if((s_ptr + cnt)->ovf.datasource != NULL) ov_clear(&(s_ptr + cnt)->ovf);
		(s_ptr + cnt)->init();
	}
}

//--------------------------------------------------------------------------------------------
//	Oggをデコードした後、PCMデータのサイズを調べます
//--------------------------------------------------------------------------------------------
static DWORD ovGetPcmSize(OggVorbis_File* ovf)
{
	vorbis_info* info;
	ogg_int64_t sampleNum;
	DWORD dwPCMBytes;

	//開いているoggファイルの総サンプル数とチャンネル数を調べ
	//必要なバイト数を取得する
	info		= ov_info(ovf, -1);
	sampleNum	= ov_pcm_total(ovf, -1);
	dwPCMBytes  = (DWORD)sampleNum * OV_PCM_BITRATE_B * info->channels;
	return dwPCMBytes;
}
static DWORD ovGetPcmSize(const char* filename)
{
	OggVorbis_File ovf;
	int error;
	DWORD dwPCMBytes;

	error = ov_fopen(filename, &ovf);
	if(error != OV_SUCCESSFUL)
	{
		return 0;
	}
	dwPCMBytes = ovGetPcmSize(&ovf);
	ov_clear(&ovf);
	return dwPCMBytes;
}

//--------------------------------------------------------------------------------------------
//	チャンクがOggかどうかを調べる
//--------------------------------------------------------------------------------------------
bool ovIsOgg(const std::string& filename)
{
	FILE* fp = NULL;
	char c[3];

	fopen_s(&fp, filename.c_str(), "rb");
	if(fp == NULL) return false;
	fread_s(c, sizeof(c), 3, 1, fp);
	fclose(fp);

	return  (c[0] == 'O') &&
			(c[1] == 'g') &&
			(c[2] == 'g');
}

//--------------------------------------------------------------------------------------------
//	Windows用読み込み関数
//--------------------------------------------------------------------------------------------
static long ovRead(OggVorbis_File* ovf_ptr, char* b_ptr,int* bs_ptr)
{
	return ov_read(ovf_ptr, b_ptr, OV_BUFFER_SIZE, 
				   0,	// リトルエンディアン、ビッグなら1を指定
				   2,	// WORD型のByteを指定、Windowsは2Byte
				   1,	// 符号有り
				   bs_ptr);
}

//--------------------------------------------------------------------------------------------
//	ogg形式をPCM形式へデコードし、DSのバッファに書き込みます
//--------------------------------------------------------------------------------------------
static int ovCreateBuffer(OggVorbis_File* ovf_ptr, int bufNumber, const char* filename, DWORD dwSize)
{
	int errorNo;

//	ファイルを開き、oggでなければエラー番号を返す
	ovInitOggEnv(ovf_ptr);
	errorNo = ov_fopen(filename, ovf_ptr);
	if(errorNo != OV_SUCCESSFUL)	return errorNo;

//	PCMの形式をある程度調べる
//	サンプリングレートとチャンネル数のみ？
	vorbis_info *info = ov_info( ovf_ptr, -1 );
	ogg_int64_t t = ov_pcm_total(ovf_ptr, -1);

//	DirectSoundBuffer設定
	WAVEFORMATEX wf;
	//さて、デコードしたPCMのビットレート16を自動的に求めるには…
	dsSetWaveFormat(&wf, info->rate, info->channels, OV_PCM_BITRATE);
	dwSize = dwSize == 0 ? ovGetPcmSize(ovf_ptr): dwSize;
	dsCreateSoundBuffer(bufNumber, &wf, dwSize);

	return OV_SUCCESSFUL;
}

//--------------------------------------------------------------------------------------------
//	ogg形式をPCM形式へデコードし、DirectSoundバッファにすべて書き込みます
//--------------------------------------------------------------------------------------------
int ovLoadOggSE(int bufNumber, const std::string& filename)
{
  OggVorbis_File ovf = {};
	char buffer[OV_BUFFER_SIZE];
	int readSize = 0;
	int readTotalSize = 0;

//	oggをPCMに変換したサイズをDirectSoundバッファにロードする
	ovCreateBuffer(&ovf, bufNumber, filename.c_str(), 0);
	do
	{
		readTotalSize += readSize;
		readSize = ovRead( &ovf, buffer, NULL);
		dsWriteSoundData(bufNumber, (BYTE*)buffer, readTotalSize, readSize);
	}while(readSize != 0);
	ov_clear(&ovf);
  return eERRORDS_NOTHING;
}


//--------------------------------------------------------------------------------------------
//	ogg形式をPCM形式へデコードし、DSのバッファに書き込みます
//--------------------------------------------------------------------------------------------
MYERROR_DS ovLoadOggStream(int bufNumber, DWORD iWriteOffset, MYSOUNDINFO* s_ptr)
{
	char buffer[OV_BUFFER_SIZE];
	DWORD readSize = 0;
	DWORD readTotalSize = 0;
	int dsbufferSize = dsGetSoundBufferSize();
	int bufferSize   = dsbufferSize + OV_BUFFER_SIZE;
	BYTE* img = new BYTE[bufferSize];
	MYERROR_DS ret = eERRORDS_NOTHING;

	//バッファのサイズになるまで読み込む
	while(readTotalSize <= (dsbufferSize - s_ptr->dwFrac))
	{
		readSize = ovRead( &s_ptr->ovf, buffer, NULL);
		if(readSize == 0)
		{
			if(dsIsLoop(bufNumber))
			{
				ov_pcm_seek(&s_ptr->ovf, 0);
			}
			else
			{
				ZeroMemory(img + readTotalSize, bufferSize - readTotalSize);
				readTotalSize = dsbufferSize - s_ptr->dwFrac;
				s_ptr->effectflag = eEFFECTDS_STOPREADY;
				ret = eERRORDS_STREAMEND;
				break;
			}
		}
		memcpy_s(img + readTotalSize, bufferSize, buffer, readSize);
		readTotalSize += readSize;
	}
	//規定量を超えたので、DirectSoundのバッファに書き込む
	dsWriteSoundData(bufNumber, img, iWriteOffset + s_ptr->dwFrac, readTotalSize);
	//次回、端数の計算
	s_ptr->dwFrac = (readTotalSize + s_ptr->dwFrac) - dsbufferSize;
	delete[] img;
	return ret;
}
//--------------------------------------------------------------------------------------------
//	ogg形式をPCM形式へデコードし、DirectSoundのバッファに書き込みます
//	
//--------------------------------------------------------------------------------------------
MYERROR_DS ovLoadOggBGM(int bufNumber, const std::string& filename, MYSOUNDINFO* s_ptr)
{
	MYERROR_DS ret = eERRORDS_NOTHING;

	if(OV_SUCCESSFUL != ovCreateBuffer(&s_ptr->ovf, bufNumber,
									   filename.c_str(), dsGetSoundBufferSize() * 2))
	{
		return eERRORDS_CSB;
	}
	ret = ovLoadOggStream(bufNumber, 0, s_ptr);
	s_ptr->format = eFORMAT_OGG;
	s_ptr->bStream = true;
	return ret;
}

}

#endif