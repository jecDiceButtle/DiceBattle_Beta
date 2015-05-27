//--------------------------------------------------------------------------------------------
//	�t�@�C����	�F	ds2008.cpp
//	�A�v����	�F	�ėp�I�ȃt�@�C���Ȃ̂ŁA���s�t�@�C���͕s����
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.05.16
//	�@�\		�F	DirectSound8�Ŏg�p����ėp�I�֐��Q�ł��B
//					DirectSound�̉B�����s���܂��B
//					�T�E���h�Đ��ɕK�v�Ȋ֐��Q�ł��B
//	�g�p���@	�F	dsInitDirectSound()���ĂԂ��Ƃɂ��ADirectSound��������������܂��B
//					�o�b�t�@�ɃA�N�Z�X����ɂ́A���ׂēY�����ŃA�N�Z�X������̂Ƃ��܂��B
//					dsLoadWave()�Ń�������WAVE�t�@�C����W�J���A
//					dsPlay_*()�ōĐ����邱�Ƃ��ł��܂��B
//					dsStop()�Œ�~�����邱�Ƃ��ł��܂��B
//					dsCheck_Stream()�͖��t���[���Ăяo�����Ƃ��K�v�ɂȂ�܂��B
//					�I�����ɂ́AdsRelease()���g�p���ă�����������Ă��������B
//					���[�h����WAVE�t�@�C���͖����k�`���ɂ��Ă��������iADPCM�Ȃǂ͕s�j
//	�ϐ�		�F	MYDS		g_ds		DirectSound���
//					MYSOUNDINFO g_sinfo		�e�T�E���h�o�b�t�@�̃X�g���[�~���O���
//	�g�p�w�b�_	�F	ds2008.h
//	�X�V����	�F	2002.05.16	Ver1.00		�A�R����	��{�I�ȋ@�\�̎���
//					2003.06.13	Ver1.01		�A�R����	dsLoadWaveStreamFirst()���s��������
//														������o�O���C��
//					2003.10.27	Ver1.02		�A�R����	�X�g���[�~���O�����Ȃ��T�E���h���T�|�[�g
//					2003.11.12	Ver1.03		�A�R����	�o�b�t�@�̍쐬�����������ɍs����悤�ɂ���
//														�f�o�C�X�̏����擾�ł���悤�ɂ���
//					2004.09.15	Ver1.04		�A�R����	�T�E���h�o�b�t�@�T�C�Y���ُ�ɂȂ��Ă���o�O���C��
//					2006.05.01	Ver2.00		�A�R����	DirectMusic�̍폜�A����ш����̍팸
//					2008.10.23	Ver2.01		�A�R����	�ȉ��̕s���ύX
//														�@�@�㏈�������܂������Ȃ��s�������
//														�A�@dsGetErrorMessage�����̕ύX
//					2014.01.20	Ver2.02		�A�R����	dsPause dsResume�̒ǉ�
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
	wf_ptr->nBlockAlign		= wf_ptr->wBitsPerSample * wf_ptr->nChannels / 8;	//Bit����Byte�֕ϊ�
	wf_ptr->nAvgBytesPerSec = wf_ptr->nSamplesPerSec * wf_ptr->nBlockAlign;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsWriteMute
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2007.06.24
//	�@�\		�F	�o�b�t�@�̓��e�𖳉��ɂ��܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�́uiSize�v�o�C�g�𖳉��ɂ��܂�
//	����		�F	<����>	numBuffer		�o�b�t�@�ԍ�
//					<����>	dwFreq		�����ɂ���T�C�Y
//	�g�p�ϐ�	�F	LPVOID	lpSoundData1, 	���b�N���ꂽ�����ւ̃|�C���^���󂯎��ϐ��̃A�h���X
//							lpSoundData2	�����O�o�b�t�@�ł��邽�߁A�󂯎��ϐ�����K�v�ɂȂ�
//					DWORD	size1, size2		���b�N���ꂽ�f�[�^�̃T�C�Y	
//	�g�p�֐�	�F	IDirectSoundBuffer8::Lock	�w�肳�ꂽ�o�b�t�@�֏������݂������܂�
//					memset						�T�E���h�f�[�^���o�b�t�@�փR�s�[���܂�
//					IDirectSoundBuffer8::Unlock	�o�b�t�@�̃��b�N���������܂�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2007.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
static void dsWriteMute(int numBuffer, int iSize)
{
	LPVOID lpSoundData1 = NULL,	lpSoundData2 = NULL;
	DWORD size1,  size2;

	g_ds.sec_ptr[numBuffer]->Lock(0, iSize, &lpSoundData1, &size1, 
		&lpSoundData2, &size2, DSBLOCK_ENTIREBUFFER);
	// �T�E���h�f�[�^�ǂݍ���
	memset(lpSoundData1, 0, iSize);
	// ���b�N����
	g_ds.sec_ptr[numBuffer]->Unlock(lpSoundData1, size1, lpSoundData2, size2);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsSetFrequency
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�o�b�t�@�̎��g����ω������܂�
//					100����100000�iOS�ɂ���Ă�100000�j�܂Őݒ�ł��܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̎��g�����udwFreq�v�ɐݒ肵�܂�
//	����		�F	<����>	numBuffer		���g����ύX������o�b�t�@�ԍ�
//					<����>	dwFreq		�ݒ肷����g��
//										100����100000�܂ŁiOS�ɂ���Ă�100000�܂Őݒ�\�j
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::SetFrequency		���g����ύX���܂�
//	�߂�l		�F	IDirectSoundBuffer8::SetFrequency�̖߂�l�ł�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
HRESULT dsSetFrequency(int numBuffer, DWORD dwFreq)
{
	return g_ds.sec_ptr[numBuffer]->SetFrequency(dwFreq);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsSetPan
//	�쐬��		�F	�A�R����
// 
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�p����ύX���܂��B
//					�p���Ƃ́A���E�̃{�����[���̃o�����X��\���܂�
//					-10000�Ȃ獶�̂݁A10000�Ȃ�E�݂̂ōĐ����܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃p�����uiPan�v�ɐݒ肵�܂�
//	����		�F	<����>	numBuffer		�p����ύX������o�b�t�@�ԍ�
//					<����>	iPan		�ݒ肷��p����db�i-10000����100000�܂Łj
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::SetPan		�p����ύX���܂�
//	�߂�l		�F	IDirectSoundBuffer8::SetPan�̖߂�l�ł�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
HRESULT dsSetPan(int numBuffer, int iPan)
{
	return g_ds.sec_ptr[numBuffer]->SetPan(iPan);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsSetVolume
//	�쐬��		�F	�A�R����
// 
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�{�����[����ύX���܂�
//					(�����Ȃ�)0����-10000(����)�܂Őݒ�ł��܂� 
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃{�����[�����uiVolume�v�ɐݒ肵�܂�
//	����		�F	<����>	numBuffer		�p����ύX������o�b�t�@�ԍ�
//					<����>	iVolume		�ݒ肷��{�����[���i0����-100000�܂Łj
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::SetVolume		�{�����[����ύX���܂�
//	�߂�l		�F	IDirectSoundBuffer8::SetVolume�̖߂�l�ł�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
HRESULT dsSetVolume(int numBuffer, long int iVolume)
{
	return g_ds.sec_ptr[numBuffer]->SetVolume(iVolume);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsGetVolume
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	���݂̃{�����[���l���擾���܂��B
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃{�����[���l���擾���܂��B
//	����		�F	<����>	numBuffer		�擾����o�b�t�@�ԍ�
//	�g�p�ϐ�	�F	long	iVolume		�擾����{�����[���l�i�[��
//	�g�p�֐�	�F	IDirectSoundBuffer8::GetVolume	�{�����[���l���擾���܂�
//	�߂�l		�F	�{�����[���l��Ԃ��܂�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
int dsGetVolume(int numBuffer)
{
	long iVolume = 0;
	
	g_ds.sec_ptr[numBuffer]->GetVolume(&iVolume);
	return iVolume;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsStop
//	�쐬��		�F	�A�R���� 
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�Đ����̃o�b�t�@���~���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃T�E���h���~���܂�
//	����		�F	<����>	numBuffer		��~���������o�b�t�@�ԍ�
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::Stop
//	�߂�l		�F	IDirectSoundBuffer8::Stop�̖߂�l���Ԃ�܂�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
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
    g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwHeaderSize;			//	�ǂ݂���ł���t�@�C���|�W�V����
    g_sinfo[numBuffer].iStreamPos = 0;
#ifdef DS_OGG_ENABLED
    dsoggSeekBegin(&g_sinfo[numBuffer].ovf);
#endif
  }
	return g_ds.sec_ptr[numBuffer]->Stop();
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsPause
//	�쐬��		�F	�A�R���� 
//	�쐬��		�F	2014.01.20
//	�@�\		�F	�Đ����̃o�b�t�@���ꎞ��~�����܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃T�E���h���ꎞ��~���܂�
//	����		�F	<����>	numBuffer		�ꎞ��~���������o�b�t�@�ԍ�
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::Stop
//	�߂�l	�F	IDirectSoundBuffer8::Stop�̖߂�l���Ԃ�܂�
//				�o�b�t�@����������Ă��Ȃ��ꍇ��0���Ԃ�܂�
//	�X�V����	�F	2014.01.20	Ver1.00		�A�R����	�@�\�̎���
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
//	�֐���		�F	dsResume
//	�쐬��		�F	�A�R���� 
//	�쐬��		�F	2014.01.20
//	�@�\		�F	�ꎞ��~�̃o�b�t�@���Đ������܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃T�E���h���Đ����܂�
//	����		�F	<����>	numBuffer		�Đ����������o�b�t�@�ԍ�
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::Play
//	�߂�l	�F	IDirectSoundBuffer8::Play�̖߂�l���Ԃ�܂�
//	�X�V����	�F	2014.01.20	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
HRESULT dsResume(int numBuffer)
{
	HRESULT hRet;
	DWORD dwPlayFlag = 0;

//	����������Ă��Ȃ��ꍇ�͍Đ����Ȃ�
	if(g_ds.sec_ptr[numBuffer]==nullptr)
	{
		return DSERR_INVALIDCALL;
	}
//	�Đ�
	if(g_sinfo[numBuffer].bLoop || g_sinfo[numBuffer].bStream)
	{
		dwPlayFlag = DSBPLAY_LOOPING;
	}
	hRet = g_ds.sec_ptr[numBuffer]->Play(0, 0, dwPlayFlag);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsFadeOut
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�Đ����T�E���h�̃t�F�[�h�A�E�g���J�n���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃t�F�[�h�A�E�g���J�n���܂�
//	����		�F	<����>	numBuffer		
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	����܂���
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
void dsFadeOut(int numBuffer, int iValue, bool fadeout_stop)
{
	g_sinfo[numBuffer].effectflag = eEFFECTDS_FADEOUT;
	g_sinfo[numBuffer].iFade = iValue;
  g_sinfo[numBuffer].fadeout_stop = fadeout_stop;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsSetLoopInfo
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�X�g���[�~���O���̃��[�v�̐ݒ���s���܂��B
//	�@�\����	�F	�unumBuffer�v�̃��[�v���A���X�^�[�g�n�_���uiReStartPos�v�ɐݒ肵�܂��B
//	����		�F	<����>	numBuffer			��ύX������o�b�t�@�ԍ�
//					<����>	iReStartPos		�ݒ肷��{�����[���i���ݎ������Ă��܂���j
//					<����>	iLoopCnt		���[�v�J�E���g��������i���ݎ������Ă��܂���j
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	����܂���
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver0.50		�A�R����	�ꕔ�̋@�\������
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
	HANDLE hFile = NULL;	//	Wave�t�@�C���n���h��
	MYERROR_DS hRet = eERRORDS_NOTHING;	//	�߂�l���i�[���܂�
	DWORD dwResult;			//	ReadFile�̌��ʂ��i�[����
	DWORD dwRiffSize;		//	RIFF�`�����N�̑傫�����擾
	DWORD dwFmtSize;		//	fmt�`�����N�̑傫�����擾
	DWORD dwDataSize;		//	data�`�����N�̑傫�����擾
	char szChunk[4] = "";	//	�`�����N�̕�������i�[���܂��i\0�͕ۑ����܂���j
	bool bLoop;

//	�t�@�C�����J��
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	RIFF�`�����N�̓ǂݍ���
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	�ǂݍ��ݎ��s�A��������RIFF�ł͂Ȃ������ꍇ
	if((dwResult != 4) || (memcmp("RIFF", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
	ReadFile(hFile, &dwRiffSize, sizeof(DWORD), &dwResult, NULL);
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	�ǂݍ��ݎ��s�A��������WAVE�ł͂Ȃ������ꍇ
	if((dwResult != 4) || (memcmp("WAVE", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
//	Foramt�`�����N
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	�ǂݍ��ݎ��s�A�������͕s���ȃf�[�^�`���������ꍇ
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	�ړI�̃`�����N�������ꍇ
		if(memcmp("fmt", szChunk, 3) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	�`�����N�Ⴂ
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
	//	fmt�f�[�^�T�C�Y
	ReadFile(hFile, &dwFmtSize, sizeof(DWORD), &dwResult, NULL);
	//	�t�H�[�}�b�g���擾
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
//	�֐���		�F	dsLoadWaveStreamFirst
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	WAVE�t�@�C�����w�b�_������͂��ēǂݍ��݂܂��B
//	�@�\����	�F	�ufilename�v�̃t�@�C���f�[�^���udata_ptr�v�փR�s�[���܂��B
//					�udata_ptr�v��
//					���[�v���A���X�^�[�g�n�_���uiReStartPos�v�ɐݒ肵�܂��B
//					�udwSize�v��0��ݒ肷��ƁA�f�[�^���ׂĂ�ǂݍ��݂܂��B
//	����		�F	�����́�numBuffer		���[�h����o�b�t�@�ԍ�
//					�����́�filename	���[�h����WAVE�t�@�C���ւ̃p�X
//					���o�́�data_ptr	�f�[�^���i�[����A�h���X�ւ̃|�C���^
//					�����́�dwSize		���ɓǂݍ��ރT�C�Y
//					���o�́�wf_ptr		WAVE�t�@�C���̃t�H�[�}�b�g�f�[�^�̊i�[��
//	�g�p�ϐ�	�F	HANDLE		hFile			Wave�t�@�C���n���h��
//					MYERROR_DS	hRet			�߂�l���i�[���܂�
//					DWORD		dwResult		ReadFile�̌��ʂ��i�[����
//					DWORD		dwRiffSize		RIFF�`�����N�̑傫�����擾
//					DWORD		dwFmtSize		fmt�`�����N�̑傫�����擾
//					DWORD		dwDataSize		data�`�����N�̑傫�����擾
//					char		szChunk[4]		�`�����N�̕�������i�[���܂��i\0�͕ۑ����܂���j
//					bool		bLoop			data�`�����N�̌����Ń��[�v���K�v���ǂ���
//	�g�p�֐�	�F	CreateFile		WAVE�t�@�C�����J���܂�
//					ReadFile		�w��T�C�Y���A�t�@�C�����烁�����փ��[�h���܂�
//					memcmp			�������̔�r���s�Ȃ��܂�
//					SetFilePointer	�O��ǂݍ��ݏI�������ꏊ�܂Ńt�@�C���|�C���^���ړ������܂�
//					libCloseHandle	WAVE�t�@�C������܂�
//	�߂�l		�F	eERRORDS_INVOPEN	�t�@�C�����J���܂���
//					eERRORDS_RIFF		�w�肵���t�@�C����RIFF�t�H�[�}�b�g�ł͂���܂���
//					eERRORDS_FMT		fmt�`�����N�̓ǂݍ��݂Ɏ��s���܂���
//					eERRORDS_NOTPCM		���̃f�[�^��PCM�`���ł͂���܂���
//					eERRORDS_DATA		DATA�`�����N�̓ǂݍ��݂Ɏ��s���܂���
//					eERRORDS_READERROR	�`�����N�\�����s���ł�
//					eERRORDS_NOTHING	�G���[�͂���܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2003.06.13	Ver1.01		�A�R����	�����WAVE�t�@�C�����J���Ɨ�����o�O���C��
//					2003.10.27	Ver1.02		�A�R����	�f�[�^�S�̂�ǂݍ��ޏ������T�|�[�g
//					2006.05.01	Ver1.03		�A�R����	�g���Ă��Ȃ��������폜
//--------------------------------------------------------------------------------------------
static MYERROR_DS dsLoadWaveStreamFirst(int numBuffer, const std::string& filename, 
	BYTE** data_ptr, DWORD dwSize, WAVEFORMATEX* wf_ptr)
{
	HANDLE hFile = NULL;	//	Wave�t�@�C���n���h��
	MYERROR_DS hRet = eERRORDS_NOTHING;	//	�߂�l���i�[���܂�
	DWORD dwResult;			//	ReadFile�̌��ʂ��i�[����
	DWORD dwRiffSize;		//	RIFF�`�����N�̑傫�����擾
	DWORD dwFmtSize;		//	fmt�`�����N�̑傫�����擾
	DWORD dwDataSize;		//	data�`�����N�̑傫�����擾
	char szChunk[4] = "";	//	�`�����N�̕�������i�[���܂��i\0�͕ۑ����܂���j
	bool bLoop;

//	�t�@�C�����J��
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	RIFF�`�����N�̓ǂݍ���
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	�ǂݍ��ݎ��s�A��������RIFF�ł͂Ȃ������ꍇ
	if((dwResult != 4) || (memcmp("RIFF", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
	ReadFile(hFile, &dwRiffSize, sizeof(DWORD), &dwResult, NULL);
	ReadFile(hFile, szChunk, 4, &dwResult, NULL);
	//	�ǂݍ��ݎ��s�A��������WAVE�ł͂Ȃ������ꍇ
if((dwResult != 4) || (memcmp("WAVE", szChunk, 4)))
	{
		hRet = eERRORDS_RIFF;
		goto end;
	}
//	Foramt�`�����N
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	�ǂݍ��ݎ��s�A�������͕s���ȃf�[�^�`���������ꍇ
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	�ړI�̃`�����N�������ꍇ
		if(memcmp("fmt", szChunk, 3) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	�`�����N�Ⴂ
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
	//	fmt�f�[�^�T�C�Y
	ReadFile(hFile, &dwFmtSize, sizeof(DWORD), &dwResult, NULL);
	//	�t�H�[�}�b�g���擾
	ReadFile(hFile, wf_ptr, dwFmtSize, &dwResult, NULL);
	if((wf_ptr->wFormatTag != WAVE_FORMAT_PCM))
	{
		hRet = eERRORDS_NOTPCM;
		goto end;
	}
	wf_ptr->cbSize = 0;
//	Data�`�����N����
	bLoop = true;
	while(bLoop)
	{
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	�ǂݍ��ݎ��s�A�������͕s���ȃf�[�^�`���������ꍇ
		if(dwResult != 4)
		{
			hRet = eERRORDS_DATA;
			goto end;
		}
		//	Data�`�����N���݂�����
		if(memcmp("data", szChunk, 4) == 0)
		{
			bLoop = false;
		}
		else
		{ 
		//	�`�����N�Ⴂ
			ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
			SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
		}
	}
//	�f�[�^�T�C�Y�ǂݍ���
	ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
	//	WAV�w�b�_�T�C�Y��ۑ����Ă����i�\�����Ȃ��`�����N�f�[�^�΍�j
	g_sinfo[numBuffer].dwHeaderSize = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
//	WAVE�f�[�^��ǂݍ���
	if(dwSize == 0)
	{
		dwSize = dwDataSize;
	}
	(*data_ptr) = new BYTE[dwSize];
//	SetFilePointer(hFile, g_sinfo[numBuffer].dwHeaderSize+dwOffset, NULL, FILE_BEGIN);
	ReadFile(hFile, *data_ptr, dwSize, &dwResult, NULL);
//	�w�肵���T�C�Y�Ɠǂݍ��񂾃T�C�Y���Ⴄ�Ƃ�
	if(dwResult != dwSize)
	{
		hRet = eERRORDS_READERROR;
		goto end;
	}
	//	�t�@�C���̃J�����g�|�W�V�������擾���Ă���
	g_sinfo[numBuffer].dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
end:
	if(hFile != NULL)
		CloseHandle(hFile);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsWriteSoundData
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�o�b�t�@�����b�N���A�f�[�^���������ޏ������s�Ȃ��܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�Ɂudata_ptr�v�̓��e��
//					�uiOffset�v��擪�A�h���X�Ƃ��āuiSize�v���������݂܂�
//	����		�F	�����́�numBuffer		�������ݐ�̃o�b�t�@�ԍ�
//					�����́�data_ptr	�������ރf�[�^
//					�����́�iOffset		�������݃o�b�t�@�̃I�t�Z�b�g
//					�����́�iSize		�������ރT�C�Y
//	�g�p�ϐ�	�F	LPVOID	lpSoundData1, 	���b�N���ꂽ�����ւ̃|�C���^���󂯎��ϐ��̃A�h���X
//							lpSoundData2	�����O�o�b�t�@�ł��邽�߁A�󂯎��ϐ�����K�v�ɂȂ�
//					DWORD	size1, size2		���b�N���ꂽ�f�[�^�̃T�C�Y	
//	�g�p�֐�	�F	IDirectSoundBuffer8::Lock	�w�肳�ꂽ�o�b�t�@�֏������݂������܂�
//					memcpy						�T�E���h�f�[�^���o�b�t�@�փR�s�[���܂�
//					IDirectSoundBuffer8::Unlock	�o�b�t�@�̃��b�N���������܂�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//				�F	2011.03.03	Ver1.01		�A�R����	�o�b�t�@���z���Ďw�肵���Ƃ��̃o�O���C��
//--------------------------------------------------------------------------------------------
void dsWriteSoundData(int numBuffer, BYTE* data_ptr, int iOffset, DWORD dwWriteSize)
{
	LPVOID lpSoundData1 = NULL,	lpSoundData2 = NULL;
	DWORD dwSize1,  dwSize2;
	HRESULT hRet;
	
	hRet = g_ds.sec_ptr[numBuffer]->Lock(iOffset, dwWriteSize,
									   &lpSoundData1, &dwSize1,	//�ʏ�͂��������ő��v
									   &lpSoundData2, &dwSize2,	//�I�[�𒴂�����A�擪�̃A�h���X���Ԃ��Ă���
									   DSBLOCK_ENTIREBUFFER);
	if(hRet == S_OK)
	{
		//�������ݗv�����I�[���ׂ��Ă���T�C�Y�ł���Ή������
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
	// ���b�N����
	g_ds.sec_ptr[numBuffer]->Unlock(lpSoundData1, dwSize1, lpSoundData2, dwSize2);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsStreamStop
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2011.03.07
//	�@�\		�F	�X�g���[�~���O�Đ��̒�~������s���܂�
//	����		�F	�����́�numBuffer			�������ރo�b�t�@�ԍ�
//					�����́�data_ptr		�f�[�^�̈�
//					�����́�dwSize			�ǂݍ��ރT�C�Y
//					�����́�iWriteOffset	�������݃o�b�t�@�̃I�t�Z�b�g
//	�X�V����	�F	2011.03.07	Ver1.00		�A�R����	�@�\�̎���
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
//	�֐���		�F	dsWriteWaveStream
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�X�g���[�~���O�Đ��̃o�b�t�@�������݂��s�Ȃ��܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�Ɂufilename�v��WAVE�t�@�C����
//					�uiOffset�v�̃t�@�C���|�W�V��������uiWriteOffset�v��擪�A�h���X�Ƃ���
//					�uiSize�v���������݂܂�
//	����		�F	�����́�numBuffer			�������ރo�b�t�@�ԍ�
//					�����́�filename		���[�h����WAVE�t�@�C����
//					�����́�data_ptr		�f�[�^�̈�
//					�����́�iOffset			�ǂݍ��݂��J�n����t�@�C���|�W�V����
//					�����́�dwSize			�ǂݍ��ރT�C�Y
//					�����́�iWriteOffset	�������݃o�b�t�@�̃I�t�Z�b�g
//	�g�p�ϐ�	�F	HANDLE		hFile		WAVE�t�@�C���̃n���h��
//					DWORD		dwResult	�֐����ʂ�����܂�
//					DWORD		dwRead		���ۂɃt�@�C����ǂݍ��񂾃o�C�g��
//					MYERROR_DS	hRet		�֐��̖߂�l���i�[���܂�
//	�g�p�֐�	�F	CreateFile			WAVE�t�@�C�����J���܂�
//					SetFilePointer		�t�@�C���|�C���^���ړ������܂�
//					ReadFile				WAVE�t�@�C�����������ɃR�s�[���܂�
//					dsStop				�T�E���h���~���܂�
//					libCloseHandle		WAVE�t�@�C������܂�
//					dsWriteSoundData		�o�b�t�@�ɃT�E���h�f�[�^���R�s�[���܂�
//					dsWriteWaveStream	�o�b�t�@�ɃX�g���[���f�[�^���R�s�[���܂�
//	�߂�l		�F	eERRORDS_INVOPEN		�t�@�C�����J���Ȃ�
//					eERRORDS_NOTHING		����
//	�X�V����	�F	2002.06.24	Ver0.50		�A�R����	�@�\�̈ꕔ����
//					2003.11.12	Ver0.51		�A�R����	�X�g���[�~���O�I�����̃m�C�Y���폜
//--------------------------------------------------------------------------------------------
static MYERROR_DS dsWriteWaveStream(int numBuffer, const std::string& filename, 
		BYTE* data_ptr, int iOffset, DWORD dwSize, int iWriteOffset)
{
	HANDLE hFile = NULL;
	DWORD dwResult;
	DWORD dwRead;	//	���ۂɃt�@�C����ǂݍ��񂾃o�C�g��
	MYERROR_DS hRet = eERRORDS_NOTHING;

	if(dsStreamStop(numBuffer, data_ptr, dwSize, iWriteOffset))
	{
		return eERRORDS_STOP;
	}

//	�t�@�C�����J��
	hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		hRet = eERRORDS_INVOPEN;
		goto end;
	}
//	�f�[�^��ǂݍ���
	SetFilePointer(hFile, iOffset, NULL, FILE_BEGIN);
	ReadFile(hFile, data_ptr, dwSize, &dwResult, NULL);
//	�w�肵���T�C�Y�Ɠǂݍ��񂾃T�C�Y���Ⴄ�Ƃ�
	if(dwResult != dwSize)
	{
	//	EOF�������Ɖ��肵�A�������s��
		//	�ǂݍ��ݐ�̃|�C���g���f�[�^�̐擪�ɐݒ肷��
		dwRead = dwSize - dwResult;//SetFilePointer(hFile, 0, NULL, FILE_CURRENT) - iOffset;
		if(!g_sinfo[numBuffer].bLoop)
		{
			ZeroMemory(&data_ptr[dwResult], dwSize-dwResult);
			dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwSize);
			g_sinfo[numBuffer].effectflag = eEFFECTDS_STOPREADY;
			goto end;
		}
	//	�ǂݍ��݃|�W�V������ݒ肷��
		if(g_sinfo[numBuffer].dwReStart != 0)	//	���X�^�[�g�n�_���ݒ肳��Ă���Ȃ��
		{
			g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwReStart + g_sinfo[numBuffer].dwHeaderSize;
		}
		else
		{
			g_sinfo[numBuffer].dwReadPosition = g_sinfo[numBuffer].dwHeaderSize;
		}
	//	�o�b�t�@�Ƀf�[�^����������
		if(hFile != NULL)	CloseHandle(hFile);
		dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwResult);
		dsWriteWaveStream(numBuffer, filename, &data_ptr[dwResult], 
			g_sinfo[numBuffer].dwReadPosition, dwSize-dwResult, iWriteOffset+dwResult);
	}
	else
	{
	//	�t�@�C���̃J�����g�|�W�V�������擾���Ă���
		g_sinfo[numBuffer].dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		dsWriteSoundData(numBuffer, data_ptr, iWriteOffset, dwSize);
	}
end:
	if(hFile != NULL)	CloseHandle(hFile);
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsCreateSoundBuffer
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2004.6.8
//	�@�\		�F	�o�b�t�@���쐬���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@���uwf_ptr�v�t�H�[�}�b�g�ŁudwBufbyte�v�o�C�g�쐬���܂�
//	����		�F	�����́�numBuffer		�쐬����o�b�t�@�ԍ�
//					�����́�wf_ptr		WAVE�t�H�[�}�b�g
//					�����́�dwBufbyte	�o�b�t�@�̃o�C�g��
//	�g�p�ϐ�	�F	DSBUFFERDESC		desc		�o�b�t�@�쐬�ɕK�v�ȏ����i�[���܂�
//					LPDIRECTSOUNDBUFFER lpTempBuff	IDirectSoundBuffer8�C���^�[�t�F�[�X�擾�ɕK�v
//					MYERROR_DS			hRet		�֐��߂�l�̊i�[
//	�g�p�֐�	�F	ZeroMemory				������0�N���A
//					IDirectSoundBuffer::CreateSoundBuffer	�T�E���h�o�b�t�@�̍쐬���s�Ȃ��܂�
//					IDirectSoundBuffer::QueryInterface		��ʂ̃C���^�[�t�F�[�X���擾���܂�
//	�߂�l		�F	eERRORDS_CSB			�o�b�t�@�쐬�Ɏ��s
//					eERRORDS_QI				QueryInterface�Ɏ��s
//					eERRORDS_NOTHING		����
//	�X�V����	�F	2004.6.8	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
MYERROR_DS dsCreateSoundBuffer(int numBuffer, WAVEFORMATEX* wf_ptr, DWORD dwBufbyte)
{
	DSBUFFERDESC desc;
	LPDIRECTSOUNDBUFFER lpTempBuff = NULL;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	�֐��߂�l

//	�T�E���h�f�[�^
	if(!wf_ptr)
	{
		return eERRORDS_SFMT;
	}
//	�T�E���h�o�b�t�@�̐ݒ�
	ZeroMemory(&desc, sizeof(desc));
	desc.lpwfxFormat = wf_ptr;
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_STATIC|DSBCAPS_LOCDEFER|
		DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN;
	desc.dwBufferBytes = dwBufbyte;	//	�o�b�t�@�T�C�Y
//	DirectSoundBuffer����
	if(FAILED(g_ds.ds_ptr->CreateSoundBuffer(&desc, &lpTempBuff, NULL)))
	{
		hRet = eERRORDS_CSB;
		goto end;
	}
//	DirectSoundBuffer8�擾
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
//	�֐���		�F	dsLoadWaveSE
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2003.11.12
//	�@�\		�F	�o�b�t�@��WAVE�t�@�C���f�[�^�����ׂăR�s�[���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�Ɂufilename�v�̃f�[�^���R�s�[���܂�
//	����		�F	�����́�numBuffer		�������ރo�b�t�@�ԍ�
//					�����́�filename	���[�h����t�@�C���ւ̃p�X
//	�g�p�ϐ�	�F	BYTE*				data_ptr	�������ރT�E���h�f�[�^
//					WAVEFORMATEX		wf			WAVE���̊i�[
//					MYERROR_DS			hRet		�֐��߂�l�̊i�[
//	�g�p�֐�	�F	ZeroMemory				������0�N���A
//					dsLoadWaveStreamFirst	�X�g���[�~���O�ɕK�v�ȏ��̃��[�h
//					dsCreateSoundBuffer		�T�E���h�o�b�t�@�̍쐬���s�Ȃ��܂�
//					IDirectSoundBuffer::QueryInterface		��ʂ̃C���^�[�t�F�[�X���擾���܂�
//					dsWriteSoundData			�T�E���h�f�[�^���o�b�t�@�֏������݂܂�
//					strcpy_s				������̃R�s�[���s�Ȃ��܂�
//	�߂�l		�F	eERRORDS_WRG			�t�@�C�����ُ�ł�
//					eERRORDS_CSB			�o�b�t�@�쐬�Ɏ��s
//					eERRORDS_QI				QueryInterface�Ɏ��s
//					eERRORDS_NOTHING		����
//	�X�V����	�F	2003.11.12	Ver1.00		�A�R����	�@�\�̎���
//					2004.06.08	Ver1.01		�A�R����	�֐��̒ǉ��idsCreateSoundBuffer�j
//					2004.09.15	Ver1.02		�A�R����	�T�E���h�o�b�t�@�T�C�Y���ُ�ɂȂ��Ă���o�O���C��
//					2011.03.03	Ver1.03		�A�R����	�֐����ύX
//--------------------------------------------------------------------------------------------
MYERROR_DS dsLoadWaveSE(int numBuffer, const std::string& filename)
{
	WAVEFORMATEX wf;
	BYTE* data_ptr = nullptr;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	�֐��߂�l

	dsReleaseBuffer(numBuffer);
	ZeroMemory(&wf, sizeof(WAVEFORMATEX));
//	DirectSoundBuffer�ݒ�
	if(eERRORDS_NOTHING != dsLoadWaveStreamFirst(numBuffer, filename, &data_ptr, 0, &wf))
	{
		hRet = eERRORDS_WRG;
		goto end;
	}
//	�o�b�t�@�̐���
	hRet = dsCreateSoundBuffer(numBuffer, &wf, g_sinfo[numBuffer].dwReadPosition-g_sinfo[numBuffer].dwHeaderSize);
	if(eERRORDS_NOTHING!=hRet)
	{
		goto end;
	}
//	�o�b�t�@���b�N
	dsWriteSoundData(numBuffer, data_ptr, 0, g_sinfo[numBuffer].dwReadPosition-g_sinfo[numBuffer].dwHeaderSize);
//	�T�E���h�����i�[
	g_sinfo[numBuffer].filename =  filename;	//	�T�E���h�t�@�C�������i�[
	g_sinfo[numBuffer].iStreamPos = 0;
	g_sinfo[numBuffer].bLoop = false;
	g_sinfo[numBuffer].bStream = false;
end:
	if(data_ptr != nullptr) delete[] data_ptr; 
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsIsOgg_CheckSupported
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2010.03.03
//	�@�\		�F	Wave��ogg���������I�ɔ��ʂ��܂��B
//	����		�F	�����́�filename	���[�h����t�@�C���ւ̃p�X
//	�߂�l		�F	true	ogg�̓T�|�[�g����Ă���A�ǂݍ��ݑΏۃt�@�C����ogg�ł�
//					false	ogg�̓T�|�[�g����Ă��Ȃ��A�������͓ǂݍ��ݑΏۃt�@�C����ogg�ł͂Ȃ�
//	�X�V����	�F	2010.03.03	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
static bool dsIsOgg_CheckSupported(const std::string& filename)
{
	return dsoggIsOgg(filename);
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsLoadSE
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2003.11.12
//	�@�\		�F	Wave��ogg���������I�ɔ��ʂ��܂��B
//	����		�F	�����́�numBuffer		�������ރo�b�t�@�ԍ�
//					�����́�filename	���[�h����t�@�C���ւ̃p�X
//	�g�p�ϐ�	�F	MYERROR_DS			hRet		�֐��߂�l�̊i�[
//	�g�p�֐�	�F	dsoggIsOgg			ogg���ǂ������w�b�_�����Ē��ׂ܂�				
//					dsLoadWaveSE		wave�̃C���[�W���o�b�t�@�ɏ������݂܂�
//					dsoggLoadOggSE		ogg�̃C���[�W��PCM�Ƀf�R�[�h���ăo�b�t�@�ɏ������݂܂�	
//	�߂�l		�F	eERRORDS_NOTHING		����
//	�X�V����	�F	2003.11.12	Ver1.00		�A�R����	�@�\�̎���
//					2004.06.08	Ver1.01		�A�R����	�֐��̒ǉ��idsCreateSoundBuffer�j
//					2004.09.15	Ver1.02		�A�R����	�T�E���h�o�b�t�@�T�C�Y���ُ�ɂȂ��Ă���o�O���C��
//--------------------------------------------------------------------------------------------
int dsLoadSE(const std::string& filename)
{
	int i;
	//���ɓǂݍ��܂�Ă���p�X�Ȃ�A���̔ԍ���Ԃ�
	for(i = 0 ; i < DS_MAX_BUFFER; i ++)
	{
		if(g_sinfo[i].filename == filename)	return i;
	}
	//�J���Ă���ꏊ��T��
	for(i = 0 ; i < DS_MAX_BUFFER; i ++)
	{
		if(g_ds.sec_ptr[i] == nullptr)
			break;
	}
	if(i == DS_MAX_BUFFER)
	{
		assert(!"����ȏ�SE��ǂݍ��ނ��Ƃ��ł��܂���BDS_MAX_BUFFER�̒l�𑝂₵�Ă�������");
		i = -1;
	}

	//�ǂݍ��݊J�n
	MYERROR_DS ret;
	if(dsIsOgg_CheckSupported(filename) == false)
	{
		ret = dsLoadWaveSE(i, filename);
	}
	else
	{
		ret = dsoggLoadOggSE(i, filename);
	}

	//�G���[���ǂ���
	if(ret != eERRORDS_NOTHING)
	{
		assert(!"SE��ǂݍ��ނ��Ƃ��ł��܂���BdsLoadSE�̈����m�F�y�ёΏۂ̃t�@�C���t�H�[�}�b�g���`�F�b�N���Ă�������");
		i = -1;
	}

	return i;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsLoadWaveBGM
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�o�b�t�@��WAVE�t�@�C���f�[�^���R�s�[���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�Ɂufilename�v�̃f�[�^���R�s�[���܂�
//					���[�v���������ꍇ�́ubLoop�v��true�A�������Ȃ��ꍇ��false�������܂�
//	����		�F	�����́�numBuffer		�������ރo�b�t�@�ԍ�
//					�����́�filename	���[�h����t�@�C���ւ̃p�X
//					�����́�bLoop		�Đ�����T�E���h�̓��[�v�����邩�ǂ���
//	�g�p�ϐ�	�F	BYTE*				data_ptr	�������ރT�E���h�f�[�^
//					WAVEFORMATEX		wf			WAVE���̊i�[
//					MYERROR_DS			hRet		�֐��߂�l�̊i�[
//	�g�p�֐�	�F	ZeroMemory				������0�N���A
//					dsLoadWaveStreamFirst	�X�g���[�~���O�ɕK�v�ȏ��̃��[�h
//					dsCreateSoundBuffer	�T�E���h�o�b�t�@�̍쐬���s�Ȃ��܂�
//					IDirectSoundBuffer::QueryInterface		��ʂ̃C���^�[�t�F�[�X���擾���܂�
//					dsWriteSoundData			�T�E���h�f�[�^���o�b�t�@�֏������݂܂�
//					strcpy_s				������̃R�s�[���s�Ȃ��܂�
//	�߂�l		�F	eERRORDS_WRG			�t�@�C�����ُ�ł�
//					eERRORDS_CSB			�o�b�t�@�쐬�Ɏ��s
//					eERRORDS_QI				QueryInterface�Ɏ��s
//					eERRORDS_NOTHING		����
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2003.10.27	Ver1.01		�A�R����	�X�g���[�~���O�Đ��𔻒肷��t���O��ݒ肵��
//					2004.06.08	Ver1.02		�A�R����	�֐��̒ǉ��idsCreateSoundBuffer�j
//					2007.02.09	Ver1.10		�A�R����	�����̍폜�A����������̕ύX
//--------------------------------------------------------------------------------------------
MYERROR_DS dsLoadWaveBGM(int numBuffer, const std::string& filename)
{
	BYTE* data_ptr = NULL;
	WAVEFORMATEX wf;
	MYERROR_DS hRet = eERRORDS_NOTHING;		//	�֐��߂�l

	dsReleaseBuffer(numBuffer);
//	DirectSoundBuffer�ݒ�
	if(eERRORDS_NOTHING != dsLoadWaveStreamFirst(numBuffer, filename, &data_ptr, g_ds.dwBufferSize, &wf))
	{
		hRet = eERRORDS_WRG;
		goto end;
	}
//	�o�b�t�@�̐���
	hRet = dsCreateSoundBuffer(numBuffer, &wf, g_ds.dwBufferSize*2);
	if(eERRORDS_NOTHING != hRet)
	{
		goto end;
	}
//	�o�b�t�@���b�N
	dsWriteSoundData(numBuffer, data_ptr, 0, g_ds.dwBufferSize);
//	�T�E���h�����i�[
	g_sinfo[numBuffer].filename = filename;	//	�T�E���h�t�@�C�������i�[
	g_sinfo[numBuffer].format = eFORMAT_WAVE;
	g_sinfo[numBuffer].iStreamPos = 0;
	g_sinfo[numBuffer].bStream = true;
end:
	if(data_ptr != nullptr) delete[] data_ptr; 
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsLoadBGM
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�o�b�t�@��WAVE�t�@�C���f�[�^���R�s�[���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�Ɂufilename�v�̃f�[�^���R�s�[���܂�
//					���[�v���������ꍇ�́ubLoop�v��true�A�������Ȃ��ꍇ��false�������܂�
//	����		�F	�����́�numBuffer		�������ރo�b�t�@�ԍ�
//					�����́�filename	���[�h����t�@�C���ւ̃p�X
//					�����́�bLoop		�Đ�����T�E���h�̓��[�v�����邩�ǂ���
//	�g�p�ϐ�	�F	BYTE*				data_ptr	�������ރT�E���h�f�[�^
//					WAVEFORMATEX		wf			WAVE���̊i�[
//					MYERROR_DS			hRet		�֐��߂�l�̊i�[
//	�g�p�֐�	�F	ZeroMemory				������0�N���A
//					dsLoadWaveStreamFirst	�X�g���[�~���O�ɕK�v�ȏ��̃��[�h
//					dsCreateSoundBuffer	�T�E���h�o�b�t�@�̍쐬���s�Ȃ��܂�
//					IDirectSoundBuffer::QueryInterface		��ʂ̃C���^�[�t�F�[�X���擾���܂�
//					dsWriteSoundData			�T�E���h�f�[�^���o�b�t�@�֏������݂܂�
//					strcpy_s				������̃R�s�[���s�Ȃ��܂�
//	�߂�l		�F	eERRORDS_WRG			�t�@�C�����ُ�ł�
//					eERRORDS_CSB			�o�b�t�@�쐬�Ɏ��s
//					eERRORDS_QI				QueryInterface�Ɏ��s
//					eERRORDS_NOTHING		����
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2003.10.27	Ver1.01		�A�R����	�X�g���[�~���O�Đ��𔻒肷��t���O��ݒ肵��
//					2004.06.08	Ver1.02		�A�R����	�֐��̒ǉ��idsCreateSoundBuffer�j
//					2007.02.09	Ver1.10		�A�R����	�����̍폜�A����������̕ύX
//--------------------------------------------------------------------------------------------
int dsLoadBGM(const std::string& filename)
{
	int i;
	//���ɓǂݍ��܂�Ă���p�X�Ȃ�A���̔ԍ���Ԃ�
	for(i = 0; i < DS_MAX_BUFFER; ++i)
	{
		if(g_sinfo[i].filename == filename) return i;
	}
	//�J���Ă���ꏊ��T��
	for(i = 0; i < DS_MAX_BUFFER; ++i)
	{
		if(g_ds.sec_ptr[i] == NULL) break;
	}
	if(i == DS_MAX_BUFFER)
	{
		assert(!"����ȏ�BGM��ǂݍ��ނ��Ƃ��ł��܂���BDS_MAX_BUFFER�̒l�𑝂₵�Ă�������");
		i = -1;
	}

	//�ǂݍ��݊J�n
	MYERROR_DS ret;
	if(dsIsOgg_CheckSupported(filename) == false)
	{
		ret = dsLoadWaveBGM(i, filename);
	}
	else
	{
		ret = dsoggLoadOggBGM(i, filename, &g_sinfo[i]);
	}

	//�G���[���ǂ���
	if(ret != eERRORDS_NOTHING)
	{
		assert(!"BGM��ǂݍ��ނ��Ƃ��ł��܂���BdsLoadBGM�̈����m�F�y�ёΏۂ̃t�@�C���t�H�[�}�b�g���`�F�b�N���Ă�������");
		i = -1;
	}
	return i;
}

//--------------------------------------------------------------------------------------------
//	�w��o�b�t�@�̃t�H�[�}�b�g��Ԃ��܂�
//--------------------------------------------------------------------------------------------
MYDSFORMAT dsCheckFormat(int numBuffer)
{
	return g_sinfo[numBuffer].format;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsLoadStream
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�X�g���[�~���O���s�Ȃ��܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@�̃f�[�^���uiWriteOffset�v�̈ʒu�ɏ������݂܂�
//					���֏�uiStreamPos�v�Ńu���b�N�i���o�[������킵�Ă܂�
//	����		�F	�����́�numBuffer			�������ރo�b�t�@�ԍ�
//					�����́�iWriteOffset	�������ރo�b�t�@�̃I�t�Z�b�g
//					�����́�iStreamPos		�������ރo�b�t�@�̃u���b�N�ԍ�
//	�g�p�ϐ�	�F	BYTE*	data_ptr	���[�h����WAVE�t�@�C���f�[�^�̈ꎞ�i�[��
//	�g�p�֐�	�F	dsWriteWaveStream	�X�g���[�~���O���s�Ȃ��܂�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2007.02.09	Ver1.01		�A�R����	�������m�ۂ̕��@��ύX
//--------------------------------------------------------------------------------------------
static void dsLoadStream(int numBuffer, int iWriteOffset, int iStreamPos)
{
	BYTE* data_ptr = nullptr;	//	�T�E���h�f�[�^

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
//	�֐���		�F	dsPlay_Normal
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	���[�h�����o�b�t�@���Đ����܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@���udwPlayFlag�v�̌`���ōĐ����܂�
//	����		�F	�����́�numBuffer		���[�h����o�b�t�@�ԍ�
//					�����́�bLoop		���[�v�Đ����邩�ǂ���
//	�g�p�ϐ�	�F	HRESULT hRet		�߂�l���i�[���܂�
//	�g�p�֐�	�F	dsLoadStream					�X�g���[�~���O���s�Ȃ��܂�
//					IDirectSoundBuffer8::Play	�o�b�t�@�f�[�^���Đ����܂�
//					dsSetVolume					�{�����[���𒲐����܂�
//	�߂�l		�F	IDirectSoundBuffer8::Play�̖߂�l���Ԃ�܂�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2003.10.27	Ver1.01		�A�R����	����������Ă��Ȃ��ꍇ�ɌĂяo����
//														�s�������ɂȂ�o�O������
//					2007.02.09	Ver1.02		�A�R����	�Đ��t���O�ݒ���@�̕ύX
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_Normal(int numBuffer, bool bLoop)
{
	HRESULT hRet;
	DWORD dwPlayFlag = 0;

//	����������Ă��Ȃ��ꍇ�͍Đ����Ȃ�
	if(g_ds.sec_ptr[numBuffer]==NULL)
	{
		return DSERR_INVALIDCALL;
	}
//	�Đ�
	g_ds.sec_ptr[numBuffer]->SetCurrentPosition(0);
	if(bLoop || g_sinfo[numBuffer].bStream)
	{
		dwPlayFlag = DSBPLAY_LOOPING;
	}
	hRet = g_ds.sec_ptr[numBuffer]->Play(0, 0, dwPlayFlag);
//	�{�����[����G�t�F�N�g�̏�����
	dsSetVolume(numBuffer, DSBVOLUME_MAX);
	g_sinfo[numBuffer].bLoop = bLoop;
	g_sinfo[numBuffer].iVolume = DSBVOLUME_MAX;
	g_sinfo[numBuffer].effectflag = eEFFECTDS_NOTHING;
	return hRet;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsPlay_FadeIn
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�t�F�[�h�C���ōĐ����J�n���܂�
//	�@�\����	�F	�unumBuffer�v�o�b�t�@���udwPlayFlag�v�̌`���ōĐ����܂�
//	����		�F	�����́�numBuffer		���[�h����o�b�t�@�ԍ�
//					�����́�dwPlayFlag	IDirectSoundBuffer8::Play�̑�O�������w�肵�܂�
//	�g�p�ϐ�	�F	HRESULT hRet		�߂�l���i�[���܂�
//	�g�p�֐�	�F	IDirectSoundBuffer8::Play	�o�b�t�@�f�[�^���Đ����܂�
//					dsSetVolume					�{�����[���𒲐����܂�
//	�߂�l		�F	IDirectSoundBuffer8::Play�̖߂�l���Ԃ�܂�
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2003.10.27	Ver1.01		�A�R����	����������Ă��Ȃ��ꍇ�ɌĂяo����
//														�s�������ɂȂ�o�O������
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_FadeIn(int numBuffer, DWORD dwPlayFlag, int iValue, bool restart)
{
	HRESULT hRet = S_OK;

//	����������Ă��Ȃ��ꍇ�͍Đ����Ȃ�
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
//	�֐���		�F	dsCheck_Stream
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�X�g���[�~���O���`�F�b�N���܂�
//					�K�����t���[���Ăяo���Ă�������
//	����		�F	����܂���
//	�g�p�ϐ�	�F	int		iCnt		���[�v�J�E���^
//					DWORD	dwStatus	�Đ��t���O���i�[���܂�
//					DWORD	dwPos		�Đ��|�W�V�������i�[���܂�
//	�g�p�֐�	�F	IDirectSoundBuffer8::GetStatus	�o�b�t�@�̏�Ԃ��擾���܂�
//					IDirectSoundBuffer8::GetCurrentPosition	���݂̃o�b�t�@�̍Đ��|�W�V�������擾���܂�
//					dsLoadStream		�X�g���[�����O���s�Ȃ��܂�
//					dsStop			�Đ����̃o�b�t�@���~�����܂�
//					dsSetVolume		�{�����[���𒲐����܂��i�t�F�[�h�C���A�A�E�g�p�j
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2002.10.27	Ver1.01		�A�R����	�X�g���[�~���O���Ȃ��T�E���h���T�|�[�g
//--------------------------------------------------------------------------------------------
void dsCheck_Stream(void)
{
	int iCnt;		//	���[�v�J�E���^
	DWORD dwStatus;	//	�Đ��t���O���i�[
	DWORD dwPos;	//	�Đ��|�W�V�������i�[

//	�ǂݍ��݃^�C�~���O�𒲂ׂ�
	for(iCnt=0;iCnt<DS_MAX_BUFFER;iCnt++)
	{
	//	���������I����Ă��Ȃ��ꍇ
		if(!g_ds.sec_ptr[iCnt])
		{
			continue;
		}
	//	�G�t�F�N�g���Ǘ�
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
	//	�X�g���[�~���O�Đ���������Ă��Ȃ�
		if(!g_sinfo[iCnt].bStream)
		{
			continue;
		}
	//	�o�b�t�@�͍Đ������ǂ����𒲂ׂ�
		g_ds.sec_ptr[iCnt]->GetStatus(&dwStatus);
		if(!(dwStatus & DSBSTATUS_PLAYING))
		{
			continue;
		}
	//	�ǂݍ��݃^�C�~���O���ǂ������`�F�b�N
		g_ds.sec_ptr[iCnt]->GetCurrentPosition(&dwPos, NULL);
	//	�Đ��|�W�V�����t���O��0�ŁA����0-11025�Ȃ�22050-44100�̃f�[�^���X�V����B
		if((g_sinfo[iCnt].iStreamPos == 0) && (dwPos > 0) && (dwPos < (g_ds.dwBufferSize / 2)))
		{
 			dsLoadStream(iCnt, g_ds.dwBufferSize, 1);
		}
	//	�Đ��|�W�V�����t���O��1�ŁA����22050-33075�Ȃ�0-22050�̃f�[�^���X�V�B
		if((g_sinfo[iCnt].iStreamPos == 1) && (dwPos > g_ds.dwBufferSize))
		{
			dsLoadStream(iCnt, 0, 0);
		}
	}
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsRelease
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2011.03.03
//	�@�\		�F	DirectSoundBuffer��������܂��B
//	����		�F	�����́�numBuffer			��������o�b�t�@�ԍ�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2011.03.03	Ver1.00		�A�R����	�@�\�̎���
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
//	�֐���		�F	dsRelease
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.05.16
//	�@�\		�F	DirectSound����������܂��B
//	����		�F	����܂���
//	�g�p�ϐ�	�F	int	iCnt				�o�b�t�@��������邽�߂̃J�E���^
//	�g�p�֐�	�F	IDirectMusicPerformance8::Stop		�����~�߂܂�
//					IDirectMusicPerformance8::CloseDown	�A�����[�h����Ă��Ȃ�������������܂�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.05.16	Ver1.00		�A�R����	�@�\�̎���
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
//	�֐���		�F	dsInitDirectSound
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.05.16
//	�@�\		�F	DirectSound�������������܂��B
//	�@�\����	�F	�uiChannel�v�ŃX�e���I�A���m�����̐ݒ���s���܂�
//					�uiSamples�v�Ŏ��g����ݒ肵�܂�
//					�uiBit�v�Ńr�b�g���[�g��ݒ肵�܂��B
//	����		�F	<����>	hWnd		�T�E���h�𔭂���E�B���h�E�̃n���h��
//					<����>	iChannel	�X�e���I�Ȃ�2�A���m�����Ȃ�1��I��
//					<����>	iSamples	�T���v�����O���[�g��I�����܂�
//					<����>	iBit		�r�b�g���[�g��I�����܂�
//	�g�p�֐�	�F	ZeroMemory								������0�N���A
//					CoCreateInstance							�C���^�[�t�F�[�X�擾
//					IDirectSound8::Initialize				�f�o�C�X������������
//					IDirectSound8::SetCooperativeLevel		�������x���̐ݒ�
//					IDirectSound8::CreateSoundBuffer			���o�b�t�@�̍쐬
//					IDirectMusicPerformance8::InitAudio		�T�E���h����������
//					GetCurrentDirectory						���݂̃f�B���N�g�����擾
//					IDirectMusicLoader8::MultiByteToWideChar	ANSI�R�[�h����unicode�ɕϊ�
//					IDirectMusicLoader8::SetSearchDirectory	�I�[�f�B�I�̂���p�X��ݒ�
//	�߂�l		�F	eERRORDS_CCIM		DirectMusic�̍쐬�Ɏ��s
//					eERRORDS_CCIS		DirectSound�̍쐬�Ɏ��s
//					eERRORDS_INIT		Initialize�Ɏ��s
//					eERRORDS_SCL		SetCooperativeLevel�Ɏ��s
//					eERRORDS_CSB		�o�b�t�@�쐬�Ɏ��s
//					eERRORDS_IA			InitAudio�Ɏ��s
//					eERRORDS_CCILOADER	���[�_�[�쐬�Ɏ��s
//					eERRORDS_MBTWC		unicode�ϊ��Ɏ��s
//					eERRORDS_SSD		�f�B���N�g���ݒ�Ɏ��s
//					eERRORDS_NOTHING	����
//	�X�V����	�F	2002.05.16	Ver1.00		�A�R����	�@�\�̎���
//					2003.10.27	Ver1.01		�A�R����	�������팸
//--------------------------------------------------------------------------------------------
MYERROR_DS dsInitDirectSound(HWND hWnd, DWORD dwBufferSize)
{
	ZeroMemory(&g_ds, sizeof(MYDS));
	g_sinfo = new MYSOUNDINFO[DS_MAX_BUFFER];
//	DirectSound�̏�����
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
//	�v���C�}���T�E���h�o�b�t�@���쐬
	DSBUFFERDESC desc =
	{
		sizeof(DSBUFFERDESC)
	};

	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if(FAILED(g_ds.ds_ptr->CreateSoundBuffer(&desc, &g_ds.pri_ptr, NULL)))
	{
		return eERRORDS_CSB;
	}
//	���̏�����
	g_ds.ds_ptr->GetCaps(&g_ds.caps);
	g_ds.dwBufferSize = dwBufferSize;
	dsoggInitOggEnv(g_sinfo);

	return eERRORDS_NOTHING;
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsGetDeviceCaps
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2003.11.12
//	�@�\		�F	�f�o�C�X�����擾���܂�
//	�@�\����	�F	�ucaps_ptr�v�Ƀf�o�C�X�����R�s�[���܂�
//	����		�F	���o�́�caps_ptr	�f�o�C�X���̏o�͐�
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	memcpy			�������R�s�[
//	�߂�l		�F	����܂���
//	�X�V����	�F	2003.11.12	Ver1.00		�A�R����	�@�\�̎���
//--------------------------------------------------------------------------------------------
void dsGetDeviceCaps(DSCAPS* caps_ptr)
{
	memcpy(caps_ptr, &g_ds.caps, sizeof(DSCAPS));
}

//--------------------------------------------------------------------------------------------
//	�֐���		�F	dsGetStatus
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2003.11.12
//	�@�\		�F	�o�b�t�@�̏�Ԃ��擾���܂�
//	�@�\����	�F	�unumBuffer�v�̃o�b�t�@�̏�Ԃ��擾���܂�
//	����		�F	�����́�numBuffer		�o�b�t�@�ԍ�
//	�g�p�ϐ�	�F	����܂���
//	�g�p�֐�	�F	IDirectSoundBuffer8::GetStatus	�T�E���h�o�b�t�@�̏�Ԃ��擾���܂�
//	�߂�l		�F	����܂���
//	�X�V����	�F	2003.11.12	Ver1.00		�A�R����	�@�\�̎���
//	�߂�l		�F	DSBSTATUS_BUFFERLOST	�o�b�t�@�͎����Ă���A�Đ��⃍�b�N���s���O��
//											�������Ȃ���΂Ȃ�Ȃ��B  
//					DSBSTATUS_LOOPING		�o�b�t�@�̓��[�v���Ă���B���̒l���ݒ肳��Ă��Ȃ�
//											�ꍇ�A�o�b�t�@�̓T�E���h �f�[�^�̍Ō�ɒB����ƒ�~����B
//											���̒l�́ADSBSTATUS_PLAYING �Ƃ̑g�ݍ��킹�ł̂ݕԂ����B 
//					DSBSTATUS_PLAYING		�o�b�t�@�͍Đ����ł���B
//											���̒l���ݒ肳��Ă��Ȃ��ꍇ�A�o�b�t�@�͒�~���Ă���B  
//					DSBSTATUS_LOCSOFTWARE	�o�b�t�@�̓\�t�g�E�F�A�ōĐ����ł���B
//											DSBCAPS_LOCDEFER �t���O�ō쐬�����o�b�t�@�ɂ̂ݐݒ肳���B 
//					DSBSTATUS_LOCHARDWARE	�o�b�t�@�̓n�[�h�E�F�A�ōĐ����ł���B
//											DSBCAPS_LOCDEFER �t���O�ō쐬�����o�b�t�@�ɂ̂ݐݒ肳���B 
//					DSBSTATUS_TERMINATED	�{�C�X�Ǘ����o�b�t�@�𑁂߂ɏI�����A�o�b�t�@�͒�~���Ă���B
//											DSBCAPS_LOCDEFER �t���O�ō쐬�����o�b�t�@�ɂ̂ݐݒ肳���B 
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
//	�֐���		�F	dsGetErrorMessage
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2002.06.24
//	�@�\		�F	�G���[���b�Z�[�W���擾���܂�
//	�@�\����	�F	�unumber�v�ɑΉ����镶������uszMsg�v�ɃR�s�[���܂�
//					�uiLength�v�ɂ̓R�s�[��̃o�b�t�@�T�C�Y���w�肵�Ă�������
//	����		�F	�����́�number		�Q�Ƃ���G���[�i���o�[
//					�����́�szMsg		���b�Z�[�W�̊i�[��
//					�����́�iLength		�i�[��̒���
//	�g�p�ϐ�	�F	char szWork		������̈ʒu���i�[��
//	�g�p�֐�	�F	strcpy_s	������R�s�[
//					memcpy			�������R�s�[
//	�߂�l		�F	����܂���
//	�X�V����	�F	2002.06.24	Ver1.00		�A�R����	�@�\�̎���
//					2007.02.09	Ver1.01		�A�R����	�R�s�[���@�̕ύX
//--------------------------------------------------------------------------------------------

std::string dsGetErrorMessage(MYERROR_DS number)
{
	std::string szMsg;

	switch(number)
	{
		case eERRORDS_CCIM: szMsg = "DirectMusic�̍쐬�Ɏ��s"; break;
		case eERRORDS_CCIS: szMsg = "DirectSound�̍쐬�Ɏ��s"; break;
		case eERRORDS_INIT: szMsg = "Initialize�Ɏ��s"; break;
		case eERRORDS_SCL: szMsg = "SetCooperativeLevel�Ɏ��s"; break;
		case eERRORDS_CSB: szMsg = "�o�b�t�@�쐬�Ɏ��s"; break;
		case eERRORDS_QI: szMsg = "QueryInterface�Ɏ��s"; break;
		case eERRORDS_SFMT: szMsg = "�t�H�[�}�b�g�ݒ�Ɏ��s"; break;
		case eERRORDS_IA: szMsg = "InitAudio�Ɏ��s"; break;
		case eERRORDS_CCILOADER: szMsg = "���[�_�[�쐬�Ɏ��s"; break;
		case eERRORDS_MBTWC: szMsg = "unicode�ϊ��Ɏ��s"; break;
		case eERRORDS_SSD: szMsg = "�f�B���N�g���ݒ�Ɏ��s"; break;
		case eERRORDS_PSE: szMsg = "�Z�O�����g�f�[�^���Đ��ł��Ȃ�"; break;
		case eERRORDS_INVALID: szMsg = "�t�@�C�����ǂݍ��߂Ȃ�"; break;
		case eERRORDS_DL: szMsg = "�_�E�����[�h�ł��Ȃ�"; break;
		case eERRORDS_UNLD: szMsg = "�A�����[�h�ł��Ȃ�"; break;
		case eERRORDS_INVOPEN: szMsg = "�t�@�C�����J���Ȃ�"; break;
		case eERRORDS_RIFF: szMsg = "RIFF�`�����N���ǂݍ��߂Ȃ�"; break;
		case eERRORDS_FMT: szMsg = "fmt�`�����N���ǂݍ��߂Ȃ�"; break;
		case eERRORDS_NOTPCM: szMsg = "�ǂݍ��ݑΏۂ�WAVE�̓T�|�[�g����Ă��Ȃ��`��"; break;
		case eERRORDS_DATA: szMsg = "data�`�����N�͑��݂��Ȃ�"; break;
		case eERRORDS_READERROR: szMsg = "�ǂݍ��ݎ��s"; break;
		case eERRORDS_UNSUPPORT: szMsg = "�T�|�[�g���Ă��Ȃ��`��"; break;
		case eERRORDS_NOTHING: szMsg = "�G���[�͂���܂���"; break;
		default: szMsg = "��`����Ă��Ȃ��G���["; break;
	}
	return szMsg;
}

}