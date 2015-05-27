//--------------------------------------------------------------------------------------------
//	ファイル名	：	ds2008.cpp
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
//	変数		：	MYDS		g_ds		DirectSound情報
//					MYSOUNDINFO g_sinfo		各サウンドバッファのストリーミング情報
//	使用ヘッダ	：	ds2008.h
//	更新履歴	：	2002.05.16	Ver1.00		植山沙欧	基本的な機能の実装
//					2003.06.13	Ver1.01		植山沙欧	dsLoadWaveStreamFirst()が不正処理で
//														落ちるバグを修正
//					2003.10.27	Ver1.02		植山沙欧	ストリーミングさせないサウンドをサポート
//					2003.11.12	Ver1.03		植山沙欧	バッファの作成を初期化時に行えるようにした
//														デバイスの情報を取得できるようにした
//					2004.09.15	Ver1.04		植山沙欧	サウンドバッファサイズが異常になっているバグを修正
//					2006.05.01	Ver2.00		植山沙欧	DirectMusicの削除、および引数の削減
//					2008.10.23	Ver2.01		植山沙欧	以下の不具合を変更
//														①　後処理がうまくいかない不具合を解消
//														②　dsGetErrorMessage引数の変更
//					2014.01.20	Ver2.02		植山沙欧	dsPause dsResumeの追加
//--------------------------------------------------------------------------------------------

#include "ds2008.h"
#include <iostream>


namespace ci_ext
{

static MYDS g_ds;
static MYSOUNDINFO* g_sinfo;

const WORD DS_EXTRAFORMAT_NOUSE = 0;

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
DWORD dsGetSoundBufferSize(void)
{
	return g_ds.dwBufferSize;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
bool dsIsLoop(int numBuffer)
{
	return g_sinfo[numBuffer].bLoop;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void dsSetWaveFormat(WAVEFORMATEX* wf_ptr,DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample)
{
	wf_ptr->cbSize			= DS_EXTRAFORMAT_NOUSE;
	wf_ptr->wFormatTag		= WAVE_FORMAT_PCM;
	wf_ptr->nChannels		= wChannels;
	wf_ptr->nSamplesPerSec	= nSamplesPerSec;
	wf_ptr->wBitsPerSample	= wBitsPerSample;
	wf_ptr->nBlockAlign		= wf_ptr->wBitsPerSample * wf_ptr->nChannels / 8;	//BitからByteへ変換
	wf_ptr->nAvgBytesPerSec = wf_ptr->nSamplesPerSec * wf_ptr->nBlockAlign;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsWriteMute
//	作成者		：	植山沙欧
//	作成日		：	2007.06.24
//	機能		：	バッファの内容を無音にします
//	機能効果	：	「numBuffer」バッファの「iSize」バイトを無音にします
//	引数		：	<入力>	numBuffer		バッファ番号
//					<入力>	dwFreq		無音にするサイズ
//	使用変数	：	LPVOID	lpSoundData1, 	ロックされた部分へのポインタを受け取る変数のアドレス
//							lpSoundData2	リングバッファであるため、受け取り変数が二つ必要になる
//					DWORD	size1, size2		ロックされたデータのサイズ	
//	使用関数	：	IDirectSoundBuffer8::Lock	指定されたバッファへ書き込みを許可します
//					memset						サウンドデータをバッファへコピーします
//					IDirectSoundBuffer8::Unlock	バッファのロックを解除します
//	戻り値		：	ありません
//	更新履歴	：	2007.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
static void dsWriteMute(int numBuffer, int iSize)
{
	LPVOID lpSoundData1 = NULL,	lpSoundData2 = NULL;
	DWORD size1,  size2;

	g_ds.sec_ptr[numBuffer]->Lock(0, iSize, &lpSoundData1, &size1, 
		&lpSoundData2, &size2, DSBLOCK_ENTIREBUFFER);
	// サウンドデータ読み込み
	memset(lpSoundData1, 0, iSize);
	// ロック解除
	g_ds.sec_ptr[numBuffer]->Unlock(lpSoundData1, size1, lpSoundData2, size2);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsSetFrequency
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	バッファの周波数を変化させます
//					100から100000（OSによっては100000）まで設定できます
//	機能効果	：	「numBuffer」バッファの周波数を「dwFreq」に設定します
//	引数		：	<入力>	numBuffer		周波数を変更させるバッファ番号
//					<入力>	dwFreq		設定する周波数
//										100から100000まで（OSによっては100000まで設定可能）
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::SetFrequency		周波数を変更します
//	戻り値		：	IDirectSoundBuffer8::SetFrequencyの戻り値です
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsSetFrequency(int numBuffer, DWORD dwFreq)
{
	return g_ds.sec_ptr[numBuffer]->SetFrequency(dwFreq);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsSetPan
//	作成者		：	植山沙欧
// 
//	作成日		：	2002.06.24
//	機能		：	パンを変更します。
//					パンとは、左右のボリュームのバランスを表します
//					-10000なら左のみ、10000なら右のみで再生します
//	機能効果	：	「numBuffer」バッファのパンを「iPan」に設定します
//	引数		：	<入力>	numBuffer		パンを変更させるバッファ番号
//					<入力>	iPan		設定するパンのdb（-10000から100000まで）
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::SetPan		パンを変更します
//	戻り値		：	IDirectSoundBuffer8::SetPanの戻り値です
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsSetPan(int numBuffer, int iPan)
{
	return g_ds.sec_ptr[numBuffer]->SetPan(iPan);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsSetVolume
//	作成者		：	植山沙欧
// 
//	作成日		：	2002.06.24
//	機能		：	ボリュームを変更します
//					(減衰なし)0から-10000(無音)まで設定できます 
//	機能効果	：	「numBuffer」バッファのボリュームを「iVolume」に設定します
//	引数		：	<入力>	numBuffer		パンを変更させるバッファ番号
//					<入力>	iVolume		設定するボリューム（0から-100000まで）
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::SetVolume		ボリュームを変更します
//	戻り値		：	IDirectSoundBuffer8::SetVolumeの戻り値です
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsSetVolume(int numBuffer, long int iVolume)
{
	return g_ds.sec_ptr[numBuffer]->SetVolume(iVolume);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsGetVolume
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	現在のボリューム値を取得します。
//	機能効果	：	「numBuffer」バッファのボリューム値を取得します。
//	引数		：	<入力>	numBuffer		取得するバッファ番号
//	使用変数	：	long	iVolume		取得するボリューム値格納先
//	使用関数	：	IDirectSoundBuffer8::GetVolume	ボリューム値を取得します
//	戻り値		：	ボリューム値を返します
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
int dsGetVolume(int numBuffer)
{
	long iVolume = 0;
	
	g_ds.sec_ptr[numBuffer]->GetVolume(&iVolume);
	return iVolume;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsStop
//	作成者		：	植山沙欧 
//	作成日		：	2002.06.24
//	機能		：	再生中のバッファを停止します
//	機能効果	：	「numBuffer」バッファのサウンドを停止します
//	引数		：	<入力>	numBuffer		停止させたいバッファ番号
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::Stop
//	戻り値		：	IDirectSoundBuffer8::Stopの戻り値が返ります
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsStop(int numBuffer)
{
	if(!g_ds.sec_ptr[numBuffer])
	{
		return HRESULT(0);
	}
//	g_sinfo[numBuffer].bStream = false;
  if (g_sinfo[numBuffer].bStream)
  {
    dsWriteMute(numBuffer, g_ds.dwBufferSize);
    g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwHeaderSize;			//	読みこんでいるファイルポジション
    g_sinfo[numBuffer].iStreamPos = 0;
#ifdef DS_OGG_ENABLED
    dsoggSeekBegin(&g_sinfo[numBuffer].ovf);
#endif
  }
	return g_ds.sec_ptr[numBuffer]->Stop();
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsPause
//	作成者		：	植山沙欧 
//	作成日		：	2014.01.20
//	機能		：	再生中のバッファを一時停止させます
//	機能効果	：	「numBuffer」バッファのサウンドを一時停止します
//	引数		：	<入力>	numBuffer		一時停止させたいバッファ番号
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::Stop
//	戻り値	：	IDirectSoundBuffer8::Stopの戻り値が返ります
//				バッファが生成されていない場合は0が返ります
//	更新履歴	：	2014.01.20	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsPause(int numBuffer)
{
	if(!g_ds.sec_ptr[numBuffer])
	{
		return HRESULT(0);
	}
	return g_ds.sec_ptr[numBuffer]->Stop();
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsResume
//	作成者		：	植山沙欧 
//	作成日		：	2014.01.20
//	機能		：	一時停止のバッファを再生させます
//	機能効果	：	「numBuffer」バッファのサウンドを再生します
//	引数		：	<入力>	numBuffer		再生させたいバッファ番号
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::Play
//	戻り値	：	IDirectSoundBuffer8::Playの戻り値が返ります
//	更新履歴	：	2014.01.20	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
HRESULT dsResume(int numBuffer)
{
	HRESULT hRet;
	DWORD dwPlayFlag = 0;

//	初期化されていない場合は再生しない
	if(g_ds.sec_ptr[numBuffer]==nullptr)
	{
		return DSERR_INVALIDCALL;
	}
//	再生
	if(g_sinfo[numBuffer].bLoop || g_sinfo[numBuffer].bStream)
	{
		dwPlayFlag = DSBPLAY_LOOPING;
	}
	hRet = g_ds.sec_ptr[numBuffer]->Play(0, 0, dwPlayFlag);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsFadeOut
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	再生中サウンドのフェードアウトを開始します
//	機能効果	：	「numBuffer」バッファのフェードアウトを開始します
//	引数		：	<入力>	numBuffer		
//	使用変数	：	ありません
//	使用関数	：	ありません
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
void dsFadeOut(int numBuffer, int iValue, bool fadeout_stop)
{
	g_sinfo[numBuffer].effectflag = eEFFECTDS_FADEOUT;
	g_sinfo[numBuffer].iFade = iValue;
  g_sinfo[numBuffer].fadeout_stop = fadeout_stop;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsSetLoopInfo
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	ストリーミング時のループの設定を行います。
//	機能効果	：	「numBuffer」のループ時、リスタート地点を「iReStartPos」に設定します。
//	引数		：	<入力>	numBuffer			を変更させるバッファ番号
//					<入力>	iReStartPos		設定するボリューム（現在実装していません）
//					<入力>	iLoopCnt		ループカウント数を決定（現在実装していません）
//	使用変数	：	ありません
//	使用関数	：	ありません
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver0.50		植山沙欧	一部の機能を実現
//--------------------------------------------------------------------------------------------
void dsSetLoopInfo(int numBuffer, int iReStartPos, int iLoopCnt)
{
	g_sinfo[numBuffer].dwReStart = iReStartPos;
	g_sinfo[numBuffer].dwCnt = iLoopCnt;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
MYERROR_DS dsGetWaveFormat(const std::string& filename, WAVEFORMATEX* wf_ptr)
{
	HANDLE hFile = NULL;	//	Waveファイルハンドル
	MYERROR_DS hRet = eERRORDS_NOTHING;	//	戻り値を格納します
	DWORD dwResult;			//	ReadFileの結果を格納する
	DWORD dwRiffSize;		//	RIFFチャンクの大きさを取得
	DWORD dwFmtSize;		//	fmtチャンクの大きさを取得
	DWORD dwDataSize;		//	dataチャンクの大きさを取得
	char szChunk[4] = "";	//	チャンクの文字列を格納します（\0は保存しません）
	bool bLoop;

//	ファイルを開く
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	RIFFチャンクの読み込み
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	読み込み失敗、もしくはRIFFではなかった場合
	if((dwResult != 4) || (memcmp("RIFF", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
	ReadFile(hFile, &dwRiffSize, sizeof(DWORD), &dwResult, NULL);
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	読み込み失敗、もしくはWAVEではなかった場合
	if((dwResult != 4) || (memcmp("WAVE", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
//	Foramtチャンク
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	読み込み失敗、もしくは不正なデータ形式だった場合
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	目的のチャンクだった場合
		if(memcmp("fmt", szChunk, 3) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	チャンク違い
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
	//	fmtデータサイズ
	ReadFile(hFile, &dwFmtSize, sizeof(DWORD), &dwResult, NULL);
	//	フォーマット情報取得
	ReadFile(hFile, wf_ptr, dwFmtSize, &dwResult, NULL);
	if((wf_ptr->wFormatTag != WAVE_FORMAT_PCM))
	{
		hRet = eERRORDS_NOTPCM;
		goto end;
	}
	wf_ptr->cbSize = 0;
end:
	if(hFile != NULL)
		CloseHandle(hFile);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadWaveStreamFirst
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	WAVEファイルをヘッダ情報を解析して読み込みます。
//	機能効果	：	「filename」のファイルデータを「data_ptr」へコピーします。
//					「data_ptr」は
//					ループ時、リスタート地点を「iReStartPos」に設定します。
//					「dwSize」に0を設定すると、データすべてを読み込みます。
//	引数		：	＜入力＞numBuffer		ロードするバッファ番号
//					＜入力＞filename	ロードするWAVEファイルへのパス
//					＜出力＞data_ptr	データを格納するアドレスへのポインタ
//					＜入力＞dwSize		一回に読み込むサイズ
//					＜出力＞wf_ptr		WAVEファイルのフォーマットデータの格納先
//	使用変数	：	HANDLE		hFile			Waveファイルハンドル
//					MYERROR_DS	hRet			戻り値を格納します
//					DWORD		dwResult		ReadFileの結果を格納する
//					DWORD		dwRiffSize		RIFFチャンクの大きさを取得
//					DWORD		dwFmtSize		fmtチャンクの大きさを取得
//					DWORD		dwDataSize		dataチャンクの大きさを取得
//					char		szChunk[4]		チャンクの文字列を格納します（\0は保存しません）
//					bool		bLoop			dataチャンクの検索でループが必要かどうか
//	使用関数	：	CreateFile		WAVEファイルを開きます
//					ReadFile		指定サイズ分、ファイルからメモリへロードします
//					memcmp			メモリの比較を行ないます
//					SetFilePointer	前回読み込み終了した場所までファイルポインタを移動させます
//					libCloseHandle	WAVEファイルを閉じます
//	戻り値		：	eERRORDS_INVOPEN	ファイルが開けません
//					eERRORDS_RIFF		指定したファイルはRIFFフォーマットではありません
//					eERRORDS_FMT		fmtチャンクの読み込みに失敗しました
//					eERRORDS_NOTPCM		このデータはPCM形式ではありません
//					eERRORDS_DATA		DATAチャンクの読み込みに失敗しました
//					eERRORDS_READERROR	チャンク構成が不正です
//					eERRORDS_NOTHING	エラーはありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2003.06.13	Ver1.01		植山沙欧	特定のWAVEファイルを開くと落ちるバグを修正
//					2003.10.27	Ver1.02		植山沙欧	データ全体を読み込む処理をサポート
//					2006.05.01	Ver1.03		植山沙欧	使っていない引数を削除
//--------------------------------------------------------------------------------------------
static MYERROR_DS dsLoadWaveStreamFirst(int numBuffer, const std::string& filename, 
	BYTE** data_ptr, DWORD dwSize, WAVEFORMATEX* wf_ptr)
{
	HANDLE hFile = NULL;	//	Waveファイルハンドル
	MYERROR_DS hRet = eERRORDS_NOTHING;	//	戻り値を格納します
	DWORD dwResult;			//	ReadFileの結果を格納する
	DWORD dwRiffSize;		//	RIFFチャンクの大きさを取得
	DWORD dwFmtSize;		//	fmtチャンクの大きさを取得
	DWORD dwDataSize;		//	dataチャンクの大きさを取得
	char szChunk[4] = "";	//	チャンクの文字列を格納します（\0は保存しません）
	bool bLoop;

//	ファイルを開く
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	RIFFチャンクの読み込み
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	読み込み失敗、もしくはRIFFではなかった場合
	if((dwResult != 4) || (memcmp("RIFF", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
	ReadFile(hFile, &dwRiffSize, sizeof(DWORD), &dwResult, NULL);
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	読み込み失敗、もしくはWAVEではなかった場合
if((dwResult != 4) || (memcmp("WAVE", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
//	Foramtチャンク
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	読み込み失敗、もしくは不正なデータ形式だった場合
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	目的のチャンクだった場合
		if(memcmp("fmt", szChunk, 3) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	チャンク違い
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
	//	fmtデータサイズ
	ReadFile(hFile, &dwFmtSize, sizeof(DWORD), &dwResult, NULL);
	//	フォーマット情報取得
	ReadFile(hFile, wf_ptr, dwFmtSize, &dwResult, NULL);
	if((wf_ptr->wFormatTag != WAVE_FORMAT_PCM))
	{
		hRet = eERRORDS_NOTPCM;
		goto end;
	}
	wf_ptr->cbSize = 0;
//	Dataチャンク検索
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	読み込み失敗、もしくは不正なデータ形式だった場合
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	Dataチャンクがみつかった
		if(memcmp("data", szChunk, 4) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	チャンク違い
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
//	データサイズ読み込み
	ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
	//	WAVヘッダサイズを保存しておく（予期しないチャンクデータ対策）
	g_sinfo[numBuffer].dwHeaderSize = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
//	WAVEデータを読み込む
	if(dwSize == 0)
	{
		dwSize = dwDataSize;
	}
	(*data_ptr) = new BYTE[dwSize];
//	SetFilePointer(hFile, g_sinfo[numBuffer].dwHeaderSize+dwOffset, NULL, FILE_BEGIN);
	ReadFile(hFile, *data_ptr, dwSize, &dwResult, NULL);
//	指定したサイズと読み込んだサイズが違うとき
	if(dwResult != dwSize)
	{
		hRet = eERRORDS_READERROR;
		goto end;
	}
	//	ファイルのカレントポジションを取得しておく
	g_sinfo[numBuffer].dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
end:
	if(hFile != NULL)
		CloseHandle(hFile);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsWriteSoundData
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	バッファをロックし、データを書きこむ処理を行ないます
//	機能効果	：	「numBuffer」バッファに「data_ptr」の内容を
//					「iOffset」を先頭アドレスとして「iSize」分書き込みます
//	引数		：	＜入力＞numBuffer		書き込み先のバッファ番号
//					＜入力＞data_ptr	書き込むデータ
//					＜入力＞iOffset		書き込みバッファのオフセット
//					＜入力＞iSize		書き込むサイズ
//	使用変数	：	LPVOID	lpSoundData1, 	ロックされた部分へのポインタを受け取る変数のアドレス
//							lpSoundData2	リングバッファであるため、受け取り変数が二つ必要になる
//					DWORD	size1, size2		ロックされたデータのサイズ	
//	使用関数	：	IDirectSoundBuffer8::Lock	指定されたバッファへ書き込みを許可します
//					memcpy						サウンドデータをバッファへコピーします
//					IDirectSoundBuffer8::Unlock	バッファのロックを解除します
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//				：	2011.03.03	Ver1.01		植山沙欧	バッファを越えて指定したときのバグを修正
//--------------------------------------------------------------------------------------------
void dsWriteSoundData(int numBuffer, BYTE* data_ptr, int iOffset, DWORD dwWriteSize)
{
	LPVOID lpSoundData1 = NULL,	lpSoundData2 = NULL;
	DWORD dwSize1,  dwSize2;
	HRESULT hRet;
	
	hRet = g_ds.sec_ptr[numBuffer]->Lock(iOffset, dwWriteSize,
									   &lpSoundData1, &dwSize1,	//通常はここだけで大丈夫
									   &lpSoundData2, &dwSize2,	//終端を超えたら、先頭のアドレスが返ってくる
									   DSBLOCK_ENTIREBUFFER);
	if(hRet == S_OK)
	{
		//書き込み要求が終端を跨っているサイズであれば回避する
		if(dwWriteSize <= dwSize1)
		{
			memcpy(lpSoundData1, data_ptr, dwWriteSize);
		}
		else
		{
			memcpy(lpSoundData1, data_ptr, dwSize1);
			DWORD dwRemainSize = (dwWriteSize - dwSize1) > dwSize2? dwSize2: dwWriteSize - dwSize1; 
			memcpy(lpSoundData2, data_ptr+dwSize1, dwRemainSize);
		}
	}
	// ロック解除
	g_ds.sec_ptr[numBuffer]->Unlock(lpSoundData1, dwSize1, lpSoundData2, dwSize2);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsStreamStop
//	作成者		：	植山沙欧
//	作成日		：	2011.03.07
//	機能		：	ストリーミング再生の停止制御を行います
//	引数		：	＜入力＞numBuffer			書き込むバッファ番号
//					＜入力＞data_ptr		データ領域
//					＜入力＞dwSize			読み込むサイズ
//					＜入力＞iWriteOffset	書き込みバッファのオフセット
//	更新履歴	：	2011.03.07	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
static bool dsStreamStop(int numBuffer, BYTE* data_ptr, DWORD dwSize, int iWriteOffset)
{
	bool bRet = false;

	if(g_sinfo[numBuffer].effectflag == eEFFECTDS_STOPREADY)
	{
		ZeroMemory(data_ptr, dwSize);
		dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwSize);
		g_sinfo[numBuffer].effectflag = eEFFECTDS_STOP;
		bRet = true;
	}
	else if(g_sinfo[numBuffer].effectflag == eEFFECTDS_STOP)
	{
		dsStop(numBuffer);
		g_sinfo[numBuffer].effectflag = eEFFECTDS_NOTHING;
		bRet = true;
	}
	return bRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsWriteWaveStream
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	ストリーミング再生のバッファ書き込みを行ないます
//	機能効果	：	「numBuffer」バッファに「filename」のWAVEファイルを
//					「iOffset」のファイルポジションから「iWriteOffset」を先頭アドレスとして
//					「iSize」分書き込みます
//	引数		：	＜入力＞numBuffer			書き込むバッファ番号
//					＜入力＞filename		ロードするWAVEファイル名
//					＜入力＞data_ptr		データ領域
//					＜入力＞iOffset			読み込みを開始するファイルポジション
//					＜入力＞dwSize			読み込むサイズ
//					＜入力＞iWriteOffset	書き込みバッファのオフセット
//	使用変数	：	HANDLE		hFile		WAVEファイルのハンドル
//					DWORD		dwResult	関数結果を入します
//					DWORD		dwRead		実際にファイルを読み込んだバイト数
//					MYERROR_DS	hRet		関数の戻り値を格納します
//	使用関数	：	CreateFile			WAVEファイルを開きます
//					SetFilePointer		ファイルポインタを移動させます
//					ReadFile				WAVEファイルをメモリにコピーします
//					dsStop				サウンドを停止します
//					libCloseHandle		WAVEファイルを閉じます
//					dsWriteSoundData		バッファにサウンドデータをコピーします
//					dsWriteWaveStream	バッファにストリームデータをコピーします
//	戻り値		：	eERRORDS_INVOPEN		ファイルが開けない
//					eERRORDS_NOTHING		成功
//	更新履歴	：	2002.06.24	Ver0.50		植山沙欧	機能の一部実現
//					2003.11.12	Ver0.51		植山沙欧	ストリーミング終了時のノイズを削除
//--------------------------------------------------------------------------------------------
static MYERROR_DS dsWriteWaveStream(int numBuffer, const std::string& filename, 
		BYTE* data_ptr, int iOffset, DWORD dwSize, int iWriteOffset)
{
	HANDLE hFile = NULL;
	DWORD dwResult;
	DWORD dwRead;	//	実際にファイルを読み込んだバイト数
	MYERROR_DS hRet = eERRORDS_NOTHING;

	if(dsStreamStop(numBuffer, data_ptr, dwSize, iWriteOffset))
	{
		return eERRORDS_STOP;
	}

//	ファイルを開く
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	データを読み込む
	SetFilePointer(hFile, iOffset, NULL, FILE_BEGIN);
	ReadFile(hFile, data_ptr, dwSize, &dwResult, NULL);
//	指定したサイズと読み込んだサイズが違うとき
	if(dwResult != dwSize)
	{
	//	EOFだったと仮定し、処理を行う
		//	読み込み先のポイントをデータの先頭に設定する
		dwRead = dwSize - dwResult;//SetFilePointer(hFile, 0, NULL, FILE_CURRENT) - iOffset;
		if(!g_sinfo[numBuffer].bLoop)
		{
			ZeroMemory(&data_ptr[dwResult], dwSize-dwResult);
			dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwSize);
			g_sinfo[numBuffer].effectflag = eEFFECTDS_STOPREADY;
			goto end;
		}
	//	読み込みポジションを設定する
		if(g_sinfo[numBuffer].dwReStart != 0)	//	リスタート地点が設定されているならば
		{
			g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwReStart + g_sinfo[numBuffer].dwHeaderSize;
		}
		else
		{
			g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwHeaderSize;
		}
	//	バッファにデータを書きこむ
		if(hFile != NULL)	CloseHandle(hFile);
		dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwResult);
		dsWriteWaveStream(numBuffer, filename, &data_ptr[dwResult], 
			g_sinfo[numBuffer].dwReadPosition, dwSize-dwResult, iWriteOffset+dwResult);
	}
	else
	{
	//	ファイルのカレントポジションを取得しておく
		g_sinfo[numBuffer].dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwSize);
	}
end:
	if(hFile != NULL)	CloseHandle(hFile);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsCreateSoundBuffer
//	作成者		：	植山沙欧
//	作成日		：	2004.6.8
//	機能		：	バッファを作成します
//	機能効果	：	「numBuffer」バッファを「wf_ptr」フォーマットで「dwBufbyte」バイト作成します
//	引数		：	＜入力＞numBuffer		作成するバッファ番号
//					＜入力＞wf_ptr		WAVEフォーマット
//					＜入力＞dwBufbyte	バッファのバイト数
//	使用変数	：	DSBUFFERDESC		desc		バッファ作成に必要な情報を格納します
//					LPDIRECTSOUNDBUFFER lpTempBuff	IDirectSoundBuffer8インターフェース取得に必要
//					MYERROR_DS			hRet		関数戻り値の格納
//	使用関数	：	ZeroMemory				メモリ0クリア
//					IDirectSoundBuffer::CreateSoundBuffer	サウンドバッファの作成を行ないます
//					IDirectSoundBuffer::QueryInterface		上位のインターフェースを取得します
//	戻り値		：	eERRORDS_CSB			バッファ作成に失敗
//					eERRORDS_QI				QueryInterfaceに失敗
//					eERRORDS_NOTHING		成功
//	更新履歴	：	2004.6.8	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
MYERROR_DS dsCreateSoundBuffer(int numBuffer, WAVEFORMATEX* wf_ptr, DWORD dwBufbyte)
{
	DSBUFFERDESC desc;
	LPDIRECTSOUNDBUFFER lpTempBuff = NULL;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	関数戻り値

//	サウンドデータ
	if(!wf_ptr)
	{
		return eERRORDS_SFMT;
	}
//	サウンドバッファの設定
	ZeroMemory(&desc, sizeof(desc));
	desc.lpwfxFormat = wf_ptr;
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_STATIC|DSBCAPS_LOCDEFER|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN;
	desc.dwBufferBytes = dwBufbyte;	//	バッファサイズ
//	DirectSoundBuffer生成
	if(FAILED(g_ds.ds_ptr->CreateSoundBuffer(&desc, &lpTempBuff, NULL)))
	{
		hRet = eERRORDS_CSB;
		goto end;
	}
//	DirectSoundBuffer8取得
	if(FAILED(lpTempBuff->QueryInterface(IID_IDirectSoundBuffer8, (void**)&g_ds.sec_ptr[numBuffer])))
	{
		hRet = eERRORDS_QI;
		goto end;
	}
end:
	if(lpTempBuff != NULL) lpTempBuff->Release();
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadWaveSE
//	作成者		：	植山沙欧
//	作成日		：	2003.11.12
//	機能		：	バッファにWAVEファイルデータをすべてコピーします
//	機能効果	：	「numBuffer」バッファに「filename」のデータをコピーします
//	引数		：	＜入力＞numBuffer		書き込むバッファ番号
//					＜入力＞filename	ロードするファイルへのパス
//	使用変数	：	BYTE*				data_ptr	書き込むサウンドデータ
//					WAVEFORMATEX		wf			WAVE情報の格納
//					MYERROR_DS			hRet		関数戻り値の格納
//	使用関数	：	ZeroMemory				メモリ0クリア
//					dsLoadWaveStreamFirst	ストリーミングに必要な情報のロード
//					dsCreateSoundBuffer		サウンドバッファの作成を行ないます
//					IDirectSoundBuffer::QueryInterface		上位のインターフェースを取得します
//					dsWriteSoundData			サウンドデータをバッファへ書き込みます
//					strcpy_s				文字列のコピーを行ないます
//	戻り値		：	eERRORDS_WRG			ファイルが異常です
//					eERRORDS_CSB			バッファ作成に失敗
//					eERRORDS_QI				QueryInterfaceに失敗
//					eERRORDS_NOTHING		成功
//	更新履歴	：	2003.11.12	Ver1.00		植山沙欧	機能の実現
//					2004.06.08	Ver1.01		植山沙欧	関数の追加（dsCreateSoundBuffer）
//					2004.09.15	Ver1.02		植山沙欧	サウンドバッファサイズが異常になっているバグを修正
//					2011.03.03	Ver1.03		植山沙欧	関数名変更
//--------------------------------------------------------------------------------------------
MYERROR_DS dsLoadWaveSE(int numBuffer, const std::string& filename)
{
	WAVEFORMATEX wf;
	BYTE* data_ptr = nullptr;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	関数戻り値

	dsReleaseBuffer(numBuffer);
	ZeroMemory(&wf, sizeof(WAVEFORMATEX));
//	DirectSoundBuffer設定
	if(eERRORDS_NOTHING != dsLoadWaveStreamFirst(numBuffer, filename, &data_ptr, 0, &wf))
	{
		hRet = eERRORDS_WRG;
		goto end;
	}
//	バッファの生成
	hRet = dsCreateSoundBuffer(numBuffer, &wf, g_sinfo[numBuffer].dwReadPosition-g_sinfo[numBuffer].dwHeaderSize);
	if(eERRORDS_NOTHING!=hRet)
	{
		goto end;
	}
//	バッファロック
	dsWriteSoundData(numBuffer, data_ptr, 0, g_sinfo[numBuffer].dwReadPosition-g_sinfo[numBuffer].dwHeaderSize);
//	サウンド情報を格納
	g_sinfo[numBuffer].filename =  filename;	//	サウンドファイル名を格納
	g_sinfo[numBuffer].iStreamPos = 0;
	g_sinfo[numBuffer].bLoop = false;
	g_sinfo[numBuffer].bStream = false;
end:
	if(data_ptr != nullptr) delete[] data_ptr; 
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsIsOgg_CheckSupported
//	作成者		：	植山沙欧
//	作成日		：	2010.03.03
//	機能		：	Waveかoggかを自動的に判別します。
//	引数		：	＜入力＞filename	ロードするファイルへのパス
//	戻り値		：	true	oggはサポートされており、読み込み対象ファイルはoggです
//					false	oggはサポートされていない、もしくは読み込み対象ファイルはoggではない
//	更新履歴	：	2010.03.03	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
static bool dsIsOgg_CheckSupported(const std::string& filename)
{
	return dsoggIsOgg(filename);
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadSE
//	作成者		：	植山沙欧
//	作成日		：	2003.11.12
//	機能		：	Waveかoggかを自動的に判別します。
//	引数		：	＜入力＞numBuffer		書き込むバッファ番号
//					＜入力＞filename	ロードするファイルへのパス
//	使用変数	：	MYERROR_DS			hRet		関数戻り値の格納
//	使用関数	：	dsoggIsOgg			oggかどうかをヘッダを見て調べます				
//					dsLoadWaveSE		waveのイメージをバッファに書き込みます
//					dsoggLoadOggSE		oggのイメージをPCMにデコードしてバッファに書き込みます	
//	戻り値		：	eERRORDS_NOTHING		成功
//	更新履歴	：	2003.11.12	Ver1.00		植山沙欧	機能の実現
//					2004.06.08	Ver1.01		植山沙欧	関数の追加（dsCreateSoundBuffer）
//					2004.09.15	Ver1.02		植山沙欧	サウンドバッファサイズが異常になっているバグを修正
//--------------------------------------------------------------------------------------------
int dsLoadSE(const std::string& filename)
{
	int i;
	//既に読み込まれているパスなら、その番号を返す
	for(i = 0 ; i < DS_MAX_BUFFER; i ++)
	{
		if(g_sinfo[i].filename == filename)	return i;
	}
	//開いている場所を探す
	for(i = 0 ; i < DS_MAX_BUFFER; i ++)
	{
		if(g_ds.sec_ptr[i] == nullptr)
			break;
	}
	if(i == DS_MAX_BUFFER)
	{
		assert(!"これ以上SEを読み込むことができません。DS_MAX_BUFFERの値を増やしてください");
		i = -1;
	}

	//読み込み開始
	MYERROR_DS ret;
	if(dsIsOgg_CheckSupported(filename) == false)
	{
		ret = dsLoadWaveSE(i, filename);
	}
	else
	{
		ret = dsoggLoadOggSE(i, filename);
	}

	//エラーかどうか
	if(ret != eERRORDS_NOTHING)
	{
		assert(!"SEを読み込むことができません。dsLoadSEの引数確認及び対象のファイルフォーマットをチェックしてください");
		i = -1;
	}

	return i;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadWaveBGM
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	バッファにWAVEファイルデータをコピーします
//	機能効果	：	「numBuffer」バッファに「filename」のデータをコピーします
//					ループさせたい場合は「bLoop」にtrue、したくない場合はfalseを代入します
//	引数		：	＜入力＞numBuffer		書き込むバッファ番号
//					＜入力＞filename	ロードするファイルへのパス
//					＜入力＞bLoop		再生するサウンドはループさせるかどうか
//	使用変数	：	BYTE*				data_ptr	書き込むサウンドデータ
//					WAVEFORMATEX		wf			WAVE情報の格納
//					MYERROR_DS			hRet		関数戻り値の格納
//	使用関数	：	ZeroMemory				メモリ0クリア
//					dsLoadWaveStreamFirst	ストリーミングに必要な情報のロード
//					dsCreateSoundBuffer	サウンドバッファの作成を行ないます
//					IDirectSoundBuffer::QueryInterface		上位のインターフェースを取得します
//					dsWriteSoundData			サウンドデータをバッファへ書き込みます
//					strcpy_s				文字列のコピーを行ないます
//	戻り値		：	eERRORDS_WRG			ファイルが異常です
//					eERRORDS_CSB			バッファ作成に失敗
//					eERRORDS_QI				QueryInterfaceに失敗
//					eERRORDS_NOTHING		成功
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2003.10.27	Ver1.01		植山沙欧	ストリーミング再生を判定するフラグを設定した
//					2004.06.08	Ver1.02		植山沙欧	関数の追加（dsCreateSoundBuffer）
//					2007.02.09	Ver1.10		植山沙欧	引数の削除、メモリ解放の変更
//--------------------------------------------------------------------------------------------
MYERROR_DS dsLoadWaveBGM(int numBuffer, const std::string& filename)
{
	BYTE* data_ptr = NULL;
	WAVEFORMATEX wf;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	関数戻り値

	dsReleaseBuffer(numBuffer);
//	DirectSoundBuffer設定
	if(eERRORDS_NOTHING != dsLoadWaveStreamFirst(numBuffer, filename, &data_ptr, g_ds.dwBufferSize, &wf))
	{
		hRet = eERRORDS_WRG;
		goto end;
	}
//	バッファの生成
	hRet = dsCreateSoundBuffer(numBuffer, &wf, g_ds.dwBufferSize*2);
	if(eERRORDS_NOTHING != hRet)
	{
		goto end;
	}
//	バッファロック
	dsWriteSoundData(numBuffer, data_ptr, 0, g_ds.dwBufferSize);
//	サウンド情報を格納
	g_sinfo[numBuffer].filename = filename;	//	サウンドファイル名を格納
	g_sinfo[numBuffer].format = eFORMAT_WAVE;
	g_sinfo[numBuffer].iStreamPos = 0;
	g_sinfo[numBuffer].bStream = true;
end:
	if(data_ptr != nullptr) delete[] data_ptr; 
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadBGM
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	バッファにWAVEファイルデータをコピーします
//	機能効果	：	「numBuffer」バッファに「filename」のデータをコピーします
//					ループさせたい場合は「bLoop」にtrue、したくない場合はfalseを代入します
//	引数		：	＜入力＞numBuffer		書き込むバッファ番号
//					＜入力＞filename	ロードするファイルへのパス
//					＜入力＞bLoop		再生するサウンドはループさせるかどうか
//	使用変数	：	BYTE*				data_ptr	書き込むサウンドデータ
//					WAVEFORMATEX		wf			WAVE情報の格納
//					MYERROR_DS			hRet		関数戻り値の格納
//	使用関数	：	ZeroMemory				メモリ0クリア
//					dsLoadWaveStreamFirst	ストリーミングに必要な情報のロード
//					dsCreateSoundBuffer	サウンドバッファの作成を行ないます
//					IDirectSoundBuffer::QueryInterface		上位のインターフェースを取得します
//					dsWriteSoundData			サウンドデータをバッファへ書き込みます
//					strcpy_s				文字列のコピーを行ないます
//	戻り値		：	eERRORDS_WRG			ファイルが異常です
//					eERRORDS_CSB			バッファ作成に失敗
//					eERRORDS_QI				QueryInterfaceに失敗
//					eERRORDS_NOTHING		成功
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2003.10.27	Ver1.01		植山沙欧	ストリーミング再生を判定するフラグを設定した
//					2004.06.08	Ver1.02		植山沙欧	関数の追加（dsCreateSoundBuffer）
//					2007.02.09	Ver1.10		植山沙欧	引数の削除、メモリ解放の変更
//--------------------------------------------------------------------------------------------
int dsLoadBGM(const std::string& filename)
{
	int i;
	//既に読み込まれているパスなら、その番号を返す
	for(i = 0; i < DS_MAX_BUFFER; ++i)
	{
		if(g_sinfo[i].filename == filename) return i;
	}
	//開いている場所を探す
	for(i = 0; i < DS_MAX_BUFFER; ++i)
	{
		if(g_ds.sec_ptr[i] == NULL) break;
	}
	if(i == DS_MAX_BUFFER)
	{
		assert(!"これ以上BGMを読み込むことができません。DS_MAX_BUFFERの値を増やしてください");
		i = -1;
	}

	//読み込み開始
	MYERROR_DS ret;
	if(dsIsOgg_CheckSupported(filename) == false)
	{
		ret = dsLoadWaveBGM(i, filename);
	}
	else
	{
		ret = dsoggLoadOggBGM(i, filename, &g_sinfo[i]);
	}

	//エラーかどうか
	if(ret != eERRORDS_NOTHING)
	{
		assert(!"BGMを読み込むことができません。dsLoadBGMの引数確認及び対象のファイルフォーマットをチェックしてください");
		i = -1;
	}
	return i;
}

//--------------------------------------------------------------------------------------------
//	指定バッファのフォーマットを返します
//--------------------------------------------------------------------------------------------
MYDSFORMAT dsCheckFormat(int numBuffer)
{
	return g_sinfo[numBuffer].format;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsLoadStream
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	ストリーミングを行ないます
//	機能効果	：	「numBuffer」バッファのデータを「iWriteOffset」の位置に書き込みます
//					利便上「iStreamPos」でブロックナンバーをあらわしてます
//	引数		：	＜入力＞numBuffer			書き込むバッファ番号
//					＜入力＞iWriteOffset	書き込むバッファのオフセット
//					＜入力＞iStreamPos		書き込むバッファのブロック番号
//	使用変数	：	BYTE*	data_ptr	ロードするWAVEファイルデータの一時格納先
//	使用関数	：	dsWriteWaveStream	ストリーミングを行ないます
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2007.02.09	Ver1.01		植山沙欧	メモリ確保の方法を変更
//--------------------------------------------------------------------------------------------
static void dsLoadStream(int numBuffer, int iWriteOffset, int iStreamPos)
{
	BYTE* data_ptr = nullptr;	//	サウンドデータ

	data_ptr = new BYTE[g_ds.dwBufferSize];
	switch(dsCheckFormat(numBuffer))
	{
		case eFORMAT_WAVE:
			dsWriteWaveStream(numBuffer, g_sinfo[numBuffer].filename, data_ptr, 
									   g_sinfo[numBuffer].dwReadPosition,
									   g_ds.dwBufferSize, iWriteOffset);
			break;
		case eFORMAT_OGG:
			if(!dsStreamStop(numBuffer, data_ptr, g_ds.dwBufferSize, iWriteOffset))
			{
				if( eERRORDS_STREAMEND == dsoggLoadOggStream(numBuffer, iWriteOffset, &g_sinfo[numBuffer]))
				{
					
				}
			}
			break;
	}
	g_sinfo[numBuffer].iStreamPos = iStreamPos;
	if(data_ptr != nullptr) delete[] data_ptr; 
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsPlay_Normal
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	ロードしたバッファを再生します
//	機能効果	：	「numBuffer」バッファを「dwPlayFlag」の形式で再生します
//	引数		：	＜入力＞numBuffer		ロードするバッファ番号
//					＜入力＞bLoop		ループ再生するかどうか
//	使用変数	：	HRESULT hRet		戻り値を格納します
//	使用関数	：	dsLoadStream					ストリーミングを行ないます
//					IDirectSoundBuffer8::Play	バッファデータを再生します
//					dsSetVolume					ボリュームを調整します
//	戻り値		：	IDirectSoundBuffer8::Playの戻り値が返ります
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2003.10.27	Ver1.01		植山沙欧	初期化されていない場合に呼び出すと
//														不正処理になるバグを解消
//					2007.02.09	Ver1.02		植山沙欧	再生フラグ設定方法の変更
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_Normal(int numBuffer, bool bLoop)
{
	HRESULT hRet;
	DWORD dwPlayFlag = 0;

//	初期化されていない場合は再生しない
	if(g_ds.sec_ptr[numBuffer]==NULL)
	{
		return DSERR_INVALIDCALL;
	}
//	再生
	g_ds.sec_ptr[numBuffer]->SetCurrentPosition(0);
	if(bLoop || g_sinfo[numBuffer].bStream)
	{
		dwPlayFlag = DSBPLAY_LOOPING;
	}
	hRet = g_ds.sec_ptr[numBuffer]->Play(0, 0, dwPlayFlag);
//	ボリュームやエフェクトの初期化
	dsSetVolume(numBuffer, DSBVOLUME_MAX);
	g_sinfo[numBuffer].bLoop = bLoop;
	g_sinfo[numBuffer].iVolume = DSBVOLUME_MAX;
	g_sinfo[numBuffer].effectflag = eEFFECTDS_NOTHING;
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsPlay_FadeIn
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	フェードインで再生を開始します
//	機能効果	：	「numBuffer」バッファを「dwPlayFlag」の形式で再生します
//	引数		：	＜入力＞numBuffer		ロードするバッファ番号
//					＜入力＞dwPlayFlag	IDirectSoundBuffer8::Playの第三引数を指定します
//	使用変数	：	HRESULT hRet		戻り値を格納します
//	使用関数	：	IDirectSoundBuffer8::Play	バッファデータを再生します
//					dsSetVolume					ボリュームを調整します
//	戻り値		：	IDirectSoundBuffer8::Playの戻り値が返ります
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2003.10.27	Ver1.01		植山沙欧	初期化されていない場合に呼び出すと
//														不正処理になるバグを解消
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_FadeIn(int numBuffer, DWORD dwPlayFlag, int iValue, bool restart)
{
	HRESULT hRet = S_OK;

//	初期化されていない場合は再生しない
	if(g_ds.sec_ptr[numBuffer] == NULL)
	{
		return DSERR_INVALIDCALL;
	}
  if (restart)
  {
    hRet = g_ds.sec_ptr[numBuffer]->Play(0, 0, dwPlayFlag);
    dsSetVolume(numBuffer, DS_VOLUME_MIN);
    g_sinfo[numBuffer].iVolume = DS_VOLUME_MIN;
  }
	g_sinfo[numBuffer].effectflag = eEFFECTDS_FADEIN;
	g_sinfo[numBuffer].iFade = iValue;
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsCheck_Stream
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	ストリーミングをチェックします
//					必ず毎フレーム呼び出してください
//	引数		：	ありません
//	使用変数	：	int		iCnt		ループカウンタ
//					DWORD	dwStatus	再生フラグを格納します
//					DWORD	dwPos		再生ポジションを格納します
//	使用関数	：	IDirectSoundBuffer8::GetStatus	バッファの状態を取得します
//					IDirectSoundBuffer8::GetCurrentPosition	現在のバッファの再生ポジションを取得します
//					dsLoadStream		ストリームングを行ないます
//					dsStop			再生中のバッファを停止させます
//					dsSetVolume		ボリュームを調整します（フェードイン、アウト用）
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2002.10.27	Ver1.01		植山沙欧	ストリーミングしないサウンドをサポート
//--------------------------------------------------------------------------------------------
void dsCheck_Stream(void)
{
	int iCnt;		//	ループカウンタ
	DWORD dwStatus;	//	再生フラグを格納
	DWORD dwPos;	//	再生ポジションを格納

//	読み込みタイミングを調べる
	for(iCnt=0;iCnt<DS_MAX_BUFFER;iCnt++)
	{
	//	初期化が終わっていない場合
		if(!g_ds.sec_ptr[iCnt])
		{
			continue;
		}
	//	エフェクトを管理
		if(g_sinfo[iCnt].effectflag == eEFFECTDS_FADEOUT)
		{
			g_sinfo[iCnt].iVolume -= g_sinfo[iCnt].iFade;//DS_VALUE_FADE;
			if(g_sinfo[iCnt].iVolume < DS_VOLUME_MIN)
			{
        if (g_sinfo[iCnt].fadeout_stop)
          dsStop(iCnt);
				g_sinfo[iCnt].effectflag = eEFFECTDS_NOTHING;
			}
			dsSetVolume(iCnt, g_sinfo[iCnt].iVolume);
		}
		if(g_sinfo[iCnt].effectflag == eEFFECTDS_FADEIN)
		{
			g_sinfo[iCnt].iVolume += g_sinfo[iCnt].iFade;//DS_VALUE_FADE;
			if(g_sinfo[iCnt].iVolume > DSBVOLUME_MAX)
			{
				g_sinfo[iCnt].iVolume = DSBVOLUME_MAX;
				g_sinfo[iCnt].effectflag = eEFFECTDS_NOTHING;
			}
			dsSetVolume(iCnt, g_sinfo[iCnt].iVolume);
		}
	//	ストリーミング再生が許可されていない
		if(!g_sinfo[iCnt].bStream)
		{
			continue;
		}
	//	バッファは再生中かどうかを調べる
		g_ds.sec_ptr[iCnt]->GetStatus(&dwStatus);
		if(!(dwStatus & DSBSTATUS_PLAYING))
		{
			continue;
		}
	//	読み込みタイミングかどうかをチェック
		g_ds.sec_ptr[iCnt]->GetCurrentPosition(&dwPos, NULL);
	//	再生ポジションフラグが0で、かつ0-11025なら22050-44100のデータを更新する。
		if((g_sinfo[iCnt].iStreamPos == 0) && (dwPos > 0) && (dwPos < (g_ds.dwBufferSize / 2)))
		{
 			dsLoadStream(iCnt, g_ds.dwBufferSize, 1);
		}
	//	再生ポジションフラグが1で、かつ22050-33075なら0-22050のデータを更新。
		if((g_sinfo[iCnt].iStreamPos == 1) && (dwPos > g_ds.dwBufferSize))
		{
			dsLoadStream(iCnt, 0, 0);
		}
	}
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsRelease
//	作成者		：	植山沙欧
//	作成日		：	2011.03.03
//	機能		：	DirectSoundBufferを解放します。
//	引数		：	＜入力＞numBuffer			消去するバッファ番号
//	戻り値		：	ありません
//	更新履歴	：	2011.03.03	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
void dsReleaseBuffer(int numBuffer)
{
	if(g_ds.sec_ptr[numBuffer] != NULL)
	{
		g_ds.sec_ptr[numBuffer]->Release();
		g_ds.sec_ptr[numBuffer] = NULL;
	}
	if(g_sinfo != nullptr)
		g_sinfo[numBuffer].init();
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsRelease
//	作成者		：	植山沙欧
//	作成日		：	2002.05.16
//	機能		：	DirectSound環境を解放します。
//	引数		：	ありません
//	使用変数	：	int	iCnt				バッファを解放するためのカウンタ
//	使用関数	：	IDirectMusicPerformance8::Stop		音を止めます
//					IDirectMusicPerformance8::CloseDown	アンロードされていない音源を解放します
//	戻り値		：	ありません
//	更新履歴	：	2002.05.16	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
void dsRelease(void)
{
	int iCnt;

	dsoggInitOggEnv(g_sinfo);
//	DirectSound
	for(iCnt = 0; iCnt < DS_MAX_BUFFER; iCnt++)
	{
		dsReleaseBuffer(iCnt);
	}
	if(g_ds.pri_ptr != NULL)
	{
		g_ds.pri_ptr->Release();
		g_ds.pri_ptr = NULL;
	}
	if(g_ds.ds_ptr != NULL)
	{
		g_ds.ds_ptr->Release();
		g_ds.ds_ptr = NULL;
	}
	delete[] g_sinfo;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsInitDirectSound
//	作成者		：	植山沙欧
//	作成日		：	2002.05.16
//	機能		：	DirectSound環境を初期化します。
//	機能説明	：	「iChannel」でステレオ、モノラルの設定を行います
//					「iSamples」で周波数を設定します
//					「iBit」でビットレートを設定します。
//	引数		：	<入力>	hWnd		サウンドを発するウィンドウのハンドル
//					<入力>	iChannel	ステレオなら2、モノラルなら1を選択
//					<入力>	iSamples	サンプリングレートを選択します
//					<入力>	iBit		ビットレートを選択します
//	使用関数	：	ZeroMemory								メモリ0クリア
//					CoCreateInstance							インターフェース取得
//					IDirectSound8::Initialize				デバイスを初期化する
//					IDirectSound8::SetCooperativeLevel		協調レベルの設定
//					IDirectSound8::CreateSoundBuffer			可聴バッファの作成
//					IDirectMusicPerformance8::InitAudio		サウンド環境を初期化
//					GetCurrentDirectory						現在のディレクトリを取得
//					IDirectMusicLoader8::MultiByteToWideChar	ANSIコードからunicodeに変換
//					IDirectMusicLoader8::SetSearchDirectory	オーディオのあるパスを設定
//	戻り値		：	eERRORDS_CCIM		DirectMusicの作成に失敗
//					eERRORDS_CCIS		DirectSoundの作成に失敗
//					eERRORDS_INIT		Initializeに失敗
//					eERRORDS_SCL		SetCooperativeLevelに失敗
//					eERRORDS_CSB		バッファ作成に失敗
//					eERRORDS_IA			InitAudioに失敗
//					eERRORDS_CCILOADER	ローダー作成に失敗
//					eERRORDS_MBTWC		unicode変換に失敗
//					eERRORDS_SSD		ディレクトリ設定に失敗
//					eERRORDS_NOTHING	成功
//	更新履歴	：	2002.05.16	Ver1.00		植山沙欧	機能の実現
//					2003.10.27	Ver1.01		植山沙欧	引数を削減
//--------------------------------------------------------------------------------------------
MYERROR_DS dsInitDirectSound(HWND hWnd, DWORD dwBufferSize)
{
	ZeroMemory(&g_ds, sizeof(MYDS));
	g_sinfo = new MYSOUNDINFO[DS_MAX_BUFFER];
//	DirectSoundの初期化
	if(FAILED(CoCreateInstance(CLSID_DirectSound8 , NULL, CLSCTX_INPROC, 
			IID_IDirectSound8, (void **)&g_ds.ds_ptr)))
	{
		return eERRORDS_CCIS;
	}
	if(FAILED(g_ds.ds_ptr->Initialize(NULL)))
	{
		return eERRORDS_INIT;
	}
	if(FAILED(g_ds.ds_ptr->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return eERRORDS_SCL;
	}
//	プライマリサウンドバッファを作成
	DSBUFFERDESC desc =
	{
		sizeof(DSBUFFERDESC)
	};

	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if(FAILED(g_ds.ds_ptr->CreateSoundBuffer(&desc, &g_ds.pri_ptr, NULL)))
	{
		return eERRORDS_CSB;
	}
//	情報の初期化
	g_ds.ds_ptr->GetCaps(&g_ds.caps);
	g_ds.dwBufferSize = dwBufferSize;
	dsoggInitOggEnv(g_sinfo);

	return eERRORDS_NOTHING;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsGetDeviceCaps
//	作成者		：	植山沙欧
//	作成日		：	2003.11.12
//	機能		：	デバイス情報を取得します
//	機能効果	：	「caps_ptr」にデバイス情報をコピーします
//	引数		：	＜出力＞caps_ptr	デバイス情報の出力先
//	使用変数	：	ありません
//	使用関数	：	memcpy			メモリコピー
//	戻り値		：	ありません
//	更新履歴	：	2003.11.12	Ver1.00		植山沙欧	機能の実現
//--------------------------------------------------------------------------------------------
void dsGetDeviceCaps(DSCAPS* caps_ptr)
{
	memcpy(caps_ptr, &g_ds.caps, sizeof(DSCAPS));
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsGetStatus
//	作成者		：	植山沙欧
//	作成日		：	2003.11.12
//	機能		：	バッファの状態を取得します
//	機能効果	：	「numBuffer」のバッファの状態を取得します
//	引数		：	＜入力＞numBuffer		バッファ番号
//	使用変数	：	ありません
//	使用関数	：	IDirectSoundBuffer8::GetStatus	サウンドバッファの状態を取得します
//	戻り値		：	ありません
//	更新履歴	：	2003.11.12	Ver1.00		植山沙欧	機能の実現
//	戻り値		：	DSBSTATUS_BUFFERLOST	バッファは失われており、再生やロックを行う前に
//											復元しなければならない。  
//					DSBSTATUS_LOOPING		バッファはループしている。この値が設定されていない
//											場合、バッファはサウンド データの最後に達すると停止する。
//											この値は、DSBSTATUS_PLAYING との組み合わせでのみ返される。 
//					DSBSTATUS_PLAYING		バッファは再生中である。
//											この値が設定されていない場合、バッファは停止している。  
//					DSBSTATUS_LOCSOFTWARE	バッファはソフトウェアで再生中である。
//											DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。 
//					DSBSTATUS_LOCHARDWARE	バッファはハードウェアで再生中である。
//											DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。 
//					DSBSTATUS_TERMINATED	ボイス管理がバッファを早めに終了し、バッファは停止している。
//											DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。 
//--------------------------------------------------------------------------------------------
DWORD dsGetStatus(int numBuffer)
{
	DWORD dwStatus;

	g_ds.sec_ptr[numBuffer]->GetStatus(&dwStatus);
	return dwStatus;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
DWORD dsGetCurrentPosition(int numBuffer)
{
//	return g_sinfo[numBuffer].dwReadPosition;
	DWORD r, w;
	g_ds.sec_ptr[numBuffer]->GetCurrentPosition(&r, &w);
	return r;
}

//--------------------------------------------------------------------------------------------
//	関数名		：	dsGetErrorMessage
//	作成者		：	植山沙欧
//	作成日		：	2002.06.24
//	機能		：	エラーメッセージを取得します
//	機能効果	：	「number」に対応する文字列を「szMsg」にコピーします
//					「iLength」にはコピー先のバッファサイズを指定してください
//	引数		：	＜入力＞number		参照するエラーナンバー
//					＜入力＞szMsg		メッセージの格納先
//					＜入力＞iLength		格納先の長さ
//	使用変数	：	char szWork		文字列の位置時格納先
//	使用関数	：	strcpy_s	文字列コピー
//					memcpy			メモリコピー
//	戻り値		：	ありません
//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
//					2007.02.09	Ver1.01		植山沙欧	コピー方法の変更
//--------------------------------------------------------------------------------------------

std::string dsGetErrorMessage(MYERROR_DS number)
{
	std::string szMsg;

	switch(number)
	{
		case eERRORDS_CCIM: szMsg = "DirectMusicの作成に失敗"; break;
		case eERRORDS_CCIS: szMsg = "DirectSoundの作成に失敗"; break;
		case eERRORDS_INIT: szMsg = "Initializeに失敗"; break;
		case eERRORDS_SCL: szMsg = "SetCooperativeLevelに失敗"; break;
		case eERRORDS_CSB: szMsg = "バッファ作成に失敗"; break;
		case eERRORDS_QI: szMsg = "QueryInterfaceに失敗"; break;
		case eERRORDS_SFMT: szMsg = "フォーマット設定に失敗"; break;
		case eERRORDS_IA: szMsg = "InitAudioに失敗"; break;
		case eERRORDS_CCILOADER: szMsg = "ローダー作成に失敗"; break;
		case eERRORDS_MBTWC: szMsg = "unicode変換に失敗"; break;
		case eERRORDS_SSD: szMsg = "ディレクトリ設定に失敗"; break;
		case eERRORDS_PSE: szMsg = "セグメントデータを再生できない"; break;
		case eERRORDS_INVALID: szMsg = "ファイルが読み込めない"; break;
		case eERRORDS_DL: szMsg = "ダウンロードできない"; break;
		case eERRORDS_UNLD: szMsg = "アンロードできない"; break;
		case eERRORDS_INVOPEN: szMsg = "ファイルが開けない"; break;
		case eERRORDS_RIFF: szMsg = "RIFFチャンクが読み込めない"; break;
		case eERRORDS_FMT: szMsg = "fmtチャンクが読み込めない"; break;
		case eERRORDS_NOTPCM: szMsg = "読み込み対象のWAVEはサポートされていない形式"; break;
		case eERRORDS_DATA: szMsg = "dataチャンクは存在しない"; break;
		case eERRORDS_READERROR: szMsg = "読み込み失敗"; break;
		case eERRORDS_UNSUPPORT: szMsg = "サポートしていない形式"; break;
		case eERRORDS_NOTHING: szMsg = "エラーはありません"; break;
		default: szMsg = "定義されていないエラー"; break;
	}
	return szMsg;
}

}