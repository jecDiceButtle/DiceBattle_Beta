//--------------------------------------------------------------------------------------------
//	ファイル名	：	ds.h
//	アプリ名	：	汎用的なファイルなので、実行ファイルは不特定
//	作成者		：	植山沙欧
//	作成日		：	2002.05.16
//	機能		：	DirectSound8で使用する汎用的関数群です。
//					DirectSoundの隠蔽を行います。
//					サウンド再生に必要な関数群です。
//	使用方法	：	dsInitDirectSound()を呼ぶことにより、DirectSound環境が初期化されます。
//					バッファにアクセスするには、すべて添え字でアクセスするものとします。
//					dsLoadWave()でメモリにWAVEファイルを展開し、
//					dsPlay_*()で再生することができます。
//					dsStop()で停止させることができます。
//					dsCheck_Stream()は毎フレーム呼び出すことが必要になります。
//					終了時には、dsRelease()を使用してメモリ解放してください。
//					ロードするWAVEファイルは未圧縮形式にしてください（ADPCMなどは不可）
//	対応ファイル：	ds.cpp
//	使用ヘッダ	：	dsound.h
//	マクロ		：	libSafeRelease				安全に参照カウントを減らします（Releaseメソッドを使用）
//					kMAX_SEGMENT		セグメントの最大数（未使用）
//					DS_MAX_BUFFER			サウンドバッファの最大数
//										この数だけWAVEファイルを登録することができます
//					DS_STANDERDSIZE		読み込み基準サイズ（バッファサイズの半分に等しい）
//					DS_VOLUME_MIN			DSBVOLUME_MINが小さすぎるため、自分で管理
//										本来は環境によって変化させるのが望ましい
//					DS_VALUE_FADE			フェードイン、アウトする一回あたりのボリューム量
//	構造体		：	MYDS				DirectMusic,Soundのオブジェクト
//					MYSOUNDINFO			ストリーミングで必要な情報
//	列挙型		：	MYERROR_DS			エラー戻り値
//					MYEFFECTFLAG		サウンドのエフェクトフラグ
//	宣言関数	：	dsInitDirectSound	DirectSound環境を初期化します
//					dsLoadWave			WAVEファイルをロードします
//					dsCheck_Stream		ストリーミング再生を監視（毎フレーム呼び出してください）
//					dsSetFrequency		サウンドの再生周波数を変更します
//										100-10000Hzまで。OSによっては100000Hzまでサポートしています
//					dsSetPan			パン（左右のバランス）を調整します
//					dsSetVolume			ボリュームを調整します
//					dsGetVolume			現在のボリューム値を取得します
//					dsPlay_Normal		ロードしたWAVEを再生します
//					dsPlay_FadeIn		ロードしたWAVEをフェードインで再生します
//					dsStop				ロードしたWAVEを停止させます
//					dsFadeOut			ロードしたWAVEをフェードアウトさせます
//					dsSetLoopInfo		ループ情報を設定します
//					dsRelease			DirectSound環境を解放します
//					dsGetDeviceCaps		デバイス情報の取得
//					dsGetStatus			バッファの状態を取得
//	更新履歴	：	2002.05.16	Ver1.00		植山沙欧	基本的な機能の実装
//					2003.06.06	Ver1.01		植山沙欧	DirectMusic補助関数を削除
//					2003.10.27	Ver1.02		植山沙欧	ストリーミング再生の強化
//					2003.11.12	Ver1.03		植山沙欧	バッファの作成を初期化時に行えるようにした
//														デバイスの情報を取得できるようにした
//					2006.05.01	Ver2.00		植山沙欧	DirectMusicを削除した
//					2007.02.09	Ver2.01		植山沙欧	ループ再生でストリーミングを意識させないようにした
//					2008.10.23	Ver2.02		植山沙欧	dsGetErrorMessage引数の変更
//					2011.03.10	Ver3.00		植山沙欧	Ogg形式のサポート（要ライブラリ）
//					2014.01.20	Ver3.01		植山沙欧	Ogg形式でstop->Playで再生位置が保持されるバグを修正
//													それに伴い、dsPauseとdsResumeを追加
//--------------------------------------------------------------------------------------------
#pragma comment(lib,"dxguid")
#pragma once

#include <windows.h>
#include <string>
#include <dsound.h>
#include <assert.h>

//OggVorbisライブラリが無い場合は、OGG_ENABLEDをOffにしておくこと
#define DS_OGG_ENABLED

namespace ci_ext
{


enum MYDSFORMAT
{
	eFORMAT_UNKNOWN,
	eFORMAT_WAVE,
	eFORMAT_OGG
};

//	環境設定
const int DS_MAX_BUFFER		= 100;		//	最大バッファ数
const int DS_VALUE_FADE		= 20;		//	フェードイン、アウトする一回あたりのボリューム量を指定
const int DS_STANDERDSIZE	= 88200;	//	基準サイズ（バッファサイズの半分に等しい）
const int DS_VOLUME_MIN		= (DSBVOLUME_MIN+5000);		//	DSBVOLUME_MINが小さすぎるため、自分で管理

//	エラー番号
enum MYERROR_DS
{
	eERRORDS_STREAMEND,	//	ストリーム再生時、EOFまで来た。ループ不可のストリーミングで必要

	eERRORDS_WRG,		//	ファイルが異常です
	eERRORDS_CCIM,		//	DirectMusicの作成に失敗
	eERRORDS_CCIS,		//	DirectSoundの作成に失敗
	eERRORDS_INIT,		//	Initializeに失敗
	eERRORDS_SCL,		//	SetCooperativeLevelに失敗
	eERRORDS_CSB,		//	バッファ作成に失敗
	eERRORDS_QI,		//	QueryInterfaceに失敗
	eERRORDS_SFMT,		//	フォーマット設定に失敗
	eERRORDS_IA,		//	InitAudioに失敗
	eERRORDS_CCILOADER,	//	ローダー作成に失敗
	eERRORDS_MBTWC,		//	unicode変換に失敗
	eERRORDS_SSD,		//	ディレクトリ設定に失敗
	eERRORDS_PSE,		//	セグメントデータを再生できない
	eERRORDS_INVALID,	//	ファイルが読み込めない
	eERRORDS_DL,		//	ダウンロードできない
	eERRORDS_UNLD,		//	アンロードできない
	eERRORDS_INVOPEN,	//	ファイルが開けない
	eERRORDS_RIFF,		//	RIFFチャンクが読み込めない
	eERRORDS_FMT,		//	fmtチャンクが読み込めない
	eERRORDS_NOTPCM,	//	読み込み対象のWAVEはサポートされていない形式
	eERRORDS_DATA,		//	dataチャンクは存在しない
	eERRORDS_READERROR,	//	読み込み失敗
	eERRORDS_UNSUPPORT,	//	サポートしていない形式
	eERRORDS_STOP,		//	ストリーム終端（ループはfalse）
	eERRORDS_NOTHING	//	エラーナシ
};

//	エフェクトフラグ番号
enum MYEFFECTFLAG	
{
	eEFFECTDS_NOTHING = 0,	//	エフェクトはなし
	eEFFECTDS_FADEIN,		//	フェードイン
	eEFFECTDS_FADEOUT,		//	フェードアウト
	eEFFECTDS_STOPREADY,	//	次ストリーム時に停止
	eEFFECTDS_STOP,			//	停止中
};

//	DirectMusic構造体
struct MYDS	
{
//	DirectSound
	IDirectSound8*			ds_ptr;			//	DirectSoundオブジェクト
	IDirectSoundBuffer8*	sec_ptr[DS_MAX_BUFFER];	//	セカンダリ（メモリ上の）バッファ
	IDirectSoundBuffer*		pri_ptr;	//	プライマリ（可聴）バッファ
//	サウンド環境情報
	DSCAPS					caps;			//	デバイス能力を格納
	DWORD					dwBufferSize;	//	用意するバッファのサイズ
};

#ifdef DS_OGG_ENABLED
	#include "vorbis/vorbisfile.h"
#endif

//	ストリーミングサウンド情報
struct MYSOUNDINFO
{
//	ストリーム情報
	MYDSFORMAT format;				//	対象ファイルのフォーマット
#ifdef DS_OGG_ENABLED
	OggVorbis_File	ovf;
	DWORD			dwFrac;	//バッファに余分に書き込んだ端数バイトを格納
#endif
	std::string	filename;	//	再生対象のファイル名
	DWORD	dwReadPosition;			//	読みこんでいるファイルポジション
	DWORD	dwHeaderSize;			//	読みこんでいるWAVEヘッダのサイズ
	int		iStreamPos;				//	書き込みの対象を表す（0なら後半、1なら前半）
	bool	bStream;				//	ストリーミング再生を許可するかどうか
//	ループ情報
	bool	bLoop;					//	ループさせるかどうか
	DWORD	dwReStart;				//	ループさせるとき、どこまで戻るか
	DWORD	dwLoop;					//	どこまできたらループさせるか
	DWORD	dwCnt;					//	何回ループを繰り返すか
//	フェードインアウト処理
	MYEFFECTFLAG	effectflag;		//	エフェクトの種類
	int				iFade;			//	フェード量
	long int		iVolume;		//	ボリューム
  bool fadeout_stop;//フェードアウトが終わったらtrue終了false音量0で再生続ける
	void init()
	{
		format = eFORMAT_UNKNOWN;
	#ifdef DS_OGG_ENABLED
		{
			OggVorbis_File temp = {};
			ovf = temp;
		}
		dwFrac = 0;
	#endif
		filename = "";
		dwReadPosition = 0;
		dwHeaderSize = 0;
		iStreamPos = 0;
		bStream = false;
		bLoop = false;
		dwReStart = 0;
		dwLoop = 0;
		dwCnt = 0;
		effectflag = eEFFECTDS_NOTHING;
		iFade = 0;
		iVolume = 0;
    fadeout_stop = true;
	}
	MYSOUNDINFO()
	{
		init();
	}};

//--------------------------------------------------------------------------------------------
//	環境設定
//--------------------------------------------------------------------------------------------
MYERROR_DS dsInitDirectSound(HWND, DWORD dwBufferSize = DS_STANDERDSIZE);	//	サウンド環境を初期化します
void dsCheck_Stream(void);			//	ストリーミング再生を監視します(毎フレーム必ず呼ぶ)
void dsReleaseBuffer(int iNumber);	//	指定されたサウンドバッファを解放します
void dsRelease(void);				//	サウンド環境を解放します

//--------------------------------------------------------------------------------------------
//	ロード関数
//--------------------------------------------------------------------------------------------
int dsLoadBGM(const std::string& filename);	//	WAVEファイルをストリーミング再生できる状態にします
int dsLoadSE(const std::string& filename);	//	WAVEファイルをメモリにロードします

//--------------------------------------------------------------------------------------------
//	再生関数
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_Normal(int no, bool loop);		//	バッファ内のデータを再生します
HRESULT dsPlay_FadeIn(int no, DWORD, int, bool restart=true);	//	バッファ内のデータをフェードインして再生します
HRESULT dsStop(int no);						//	再生中のバッファを停止させます
HRESULT dsPause(int no);						//	再生中のバッファを一時停止させます
HRESULT dsResume(int no);						//	一時停止のバッファを再生させます
void dsFadeOut(int no, int, bool fadeout_stop = true);	//	フェードアウトさせます
void dsSetLoopInfo(int no, int, int iLoopCnt=0);	//	ループ情報を設定します

//--------------------------------------------------------------------------------------------
//	再生パラメータ変更関数
//--------------------------------------------------------------------------------------------
HRESULT dsSetFrequency(int, DWORD);		//	周波数を変更します
HRESULT dsSetPan(int, int);				//	パン設定を行います
HRESULT dsSetVolume(int,long);			//	ボリューム調整を行います

//--------------------------------------------------------------------------------------------
//	パラメータ取得関数
//--------------------------------------------------------------------------------------------
int dsGetVolume(int);					//	現在のボリューム値を取得します
DWORD dsGetStatus(int);					//	バッファの状態を取得します
void dsGetDeviceCaps(DSCAPS*);			//	デバイスの情報を取得します
DWORD dsGetCurrentPosition(int iNumber);
DWORD dsGetSoundBufferSize(void);
bool dsIsLoop(int iNumber);


//--------------------------------------------------------------------------------------------
//	その他
//--------------------------------------------------------------------------------------------
MYERROR_DS dsGetWaveFormat(const std::string& filename, WAVEFORMATEX* wf_ptr);
void dsSetWaveFormat(WAVEFORMATEX* wf_ptr,DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample);


std::string dsGetErrorMessage(MYERROR_DS number);// エラー番号から文字列を取得します



#ifdef DS_OGG_ENABLED
	#include "OggVorbis.h"
	#define dsoggIsOgg			ovIsOgg
	#define dsoggLoadOggSE		(MYERROR_DS)ovLoadOggSE
	#define dsoggLoadOggBGM		(MYERROR_DS)ovLoadOggBGM
	#define dsoggLoadOggStream	ovLoadOggStream
	#define dsoggInitOggEnv		ovInitOggEnvAll
	#define dsoggSeekBegin		ovSeekBegin
	//	外部ライブラリからサウンドバッファ作成が必要なとき用
	extern MYERROR_DS dsCreateSoundBuffer(int iNumber, WAVEFORMATEX* wf_ptr, DWORD dwBufbyte);
	extern void dsWriteSoundData(int iNumber, BYTE* data_ptr, int iOffset, DWORD dwWriteSize);
#else
	#define dsoggIsOgg			false;__noop
	#define dsoggLoadOggSE		eERRORDS_UNSUPPORT;__noop
	#define dsoggLoadOggBGM		eERRORDS_UNSUPPORT;__noop
	#define dsoggLoadStream		eERRORDS_UNSUPPORT;__noop
	#define dsoggLoadOggStream	__noop
	#define dsoggInitOggEnv		__noop
	#define dsoggSeekBegin		__noop
#endif

}