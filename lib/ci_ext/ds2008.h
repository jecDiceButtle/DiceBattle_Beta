//--------------------------------------------------------------------------------------------
//	�t�@�C����	�F	ds.h
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
//	�Ή��t�@�C���F	ds.cpp
//	�g�p�w�b�_	�F	dsound.h
//	�}�N��		�F	libSafeRelease				���S�ɎQ�ƃJ�E���g�����炵�܂��iRelease���\�b�h���g�p�j
//					kMAX_SEGMENT		�Z�O�����g�̍ő吔�i���g�p�j
//					DS_MAX_BUFFER			�T�E���h�o�b�t�@�̍ő吔
//										���̐�����WAVE�t�@�C����o�^���邱�Ƃ��ł��܂�
//					DS_STANDERDSIZE		�ǂݍ��݊�T�C�Y�i�o�b�t�@�T�C�Y�̔����ɓ������j
//					DS_VOLUME_MIN			DSBVOLUME_MIN�����������邽�߁A�����ŊǗ�
//										�{���͊��ɂ���ĕω�������̂��]�܂���
//					DS_VALUE_FADE			�t�F�[�h�C���A�A�E�g�����񂠂���̃{�����[����
//	�\����		�F	MYDS				DirectMusic,Sound�̃I�u�W�F�N�g
//					MYSOUNDINFO			�X�g���[�~���O�ŕK�v�ȏ��
//	�񋓌^		�F	MYERROR_DS			�G���[�߂�l
//					MYEFFECTFLAG		�T�E���h�̃G�t�F�N�g�t���O
//	�錾�֐�	�F	dsInitDirectSound	DirectSound�������������܂�
//					dsLoadWave			WAVE�t�@�C�������[�h���܂�
//					dsCheck_Stream		�X�g���[�~���O�Đ����Ď��i���t���[���Ăяo���Ă��������j
//					dsSetFrequency		�T�E���h�̍Đ����g����ύX���܂�
//										100-10000Hz�܂ŁBOS�ɂ���Ă�100000Hz�܂ŃT�|�[�g���Ă��܂�
//					dsSetPan			�p���i���E�̃o�����X�j�𒲐����܂�
//					dsSetVolume			�{�����[���𒲐����܂�
//					dsGetVolume			���݂̃{�����[���l���擾���܂�
//					dsPlay_Normal		���[�h����WAVE���Đ����܂�
//					dsPlay_FadeIn		���[�h����WAVE���t�F�[�h�C���ōĐ����܂�
//					dsStop				���[�h����WAVE���~�����܂�
//					dsFadeOut			���[�h����WAVE���t�F�[�h�A�E�g�����܂�
//					dsSetLoopInfo		���[�v����ݒ肵�܂�
//					dsRelease			DirectSound����������܂�
//					dsGetDeviceCaps		�f�o�C�X���̎擾
//					dsGetStatus			�o�b�t�@�̏�Ԃ��擾
//	�X�V����	�F	2002.05.16	Ver1.00		�A�R����	��{�I�ȋ@�\�̎���
//					2003.06.06	Ver1.01		�A�R����	DirectMusic�⏕�֐����폜
//					2003.10.27	Ver1.02		�A�R����	�X�g���[�~���O�Đ��̋���
//					2003.11.12	Ver1.03		�A�R����	�o�b�t�@�̍쐬�����������ɍs����悤�ɂ���
//														�f�o�C�X�̏����擾�ł���悤�ɂ���
//					2006.05.01	Ver2.00		�A�R����	DirectMusic���폜����
//					2007.02.09	Ver2.01		�A�R����	���[�v�Đ��ŃX�g���[�~���O���ӎ������Ȃ��悤�ɂ���
//					2008.10.23	Ver2.02		�A�R����	dsGetErrorMessage�����̕ύX
//					2011.03.10	Ver3.00		�A�R����	Ogg�`���̃T�|�[�g�i�v���C�u�����j
//					2014.01.20	Ver3.01		�A�R����	Ogg�`����stop->Play�ōĐ��ʒu���ێ������o�O���C��
//													����ɔ����AdsPause��dsResume��ǉ�
//--------------------------------------------------------------------------------------------
#pragma comment(lib,"dxguid")
#pragma once

#include <windows.h>
#include <string>
#include <dsound.h>
#include <assert.h>

//OggVorbis���C�u�����������ꍇ�́AOGG_ENABLED��Off�ɂ��Ă�������
#define DS_OGG_ENABLED

namespace ci_ext
{


enum MYDSFORMAT
{
	eFORMAT_UNKNOWN,
	eFORMAT_WAVE,
	eFORMAT_OGG
};

//	���ݒ�
const int DS_MAX_BUFFER		= 100;		//	�ő�o�b�t�@��
const int DS_VALUE_FADE		= 20;		//	�t�F�[�h�C���A�A�E�g�����񂠂���̃{�����[���ʂ��w��
const int DS_STANDERDSIZE	= 88200;	//	��T�C�Y�i�o�b�t�@�T�C�Y�̔����ɓ������j
const int DS_VOLUME_MIN		= (DSBVOLUME_MIN+5000);		//	DSBVOLUME_MIN�����������邽�߁A�����ŊǗ�

//	�G���[�ԍ�
enum MYERROR_DS
{
	eERRORDS_STREAMEND,	//	�X�g���[���Đ����AEOF�܂ŗ����B���[�v�s�̃X�g���[�~���O�ŕK�v

	eERRORDS_WRG,		//	�t�@�C�����ُ�ł�
	eERRORDS_CCIM,		//	DirectMusic�̍쐬�Ɏ��s
	eERRORDS_CCIS,		//	DirectSound�̍쐬�Ɏ��s
	eERRORDS_INIT,		//	Initialize�Ɏ��s
	eERRORDS_SCL,		//	SetCooperativeLevel�Ɏ��s
	eERRORDS_CSB,		//	�o�b�t�@�쐬�Ɏ��s
	eERRORDS_QI,		//	QueryInterface�Ɏ��s
	eERRORDS_SFMT,		//	�t�H�[�}�b�g�ݒ�Ɏ��s
	eERRORDS_IA,		//	InitAudio�Ɏ��s
	eERRORDS_CCILOADER,	//	���[�_�[�쐬�Ɏ��s
	eERRORDS_MBTWC,		//	unicode�ϊ��Ɏ��s
	eERRORDS_SSD,		//	�f�B���N�g���ݒ�Ɏ��s
	eERRORDS_PSE,		//	�Z�O�����g�f�[�^���Đ��ł��Ȃ�
	eERRORDS_INVALID,	//	�t�@�C�����ǂݍ��߂Ȃ�
	eERRORDS_DL,		//	�_�E�����[�h�ł��Ȃ�
	eERRORDS_UNLD,		//	�A�����[�h�ł��Ȃ�
	eERRORDS_INVOPEN,	//	�t�@�C�����J���Ȃ�
	eERRORDS_RIFF,		//	RIFF�`�����N���ǂݍ��߂Ȃ�
	eERRORDS_FMT,		//	fmt�`�����N���ǂݍ��߂Ȃ�
	eERRORDS_NOTPCM,	//	�ǂݍ��ݑΏۂ�WAVE�̓T�|�[�g����Ă��Ȃ��`��
	eERRORDS_DATA,		//	data�`�����N�͑��݂��Ȃ�
	eERRORDS_READERROR,	//	�ǂݍ��ݎ��s
	eERRORDS_UNSUPPORT,	//	�T�|�[�g���Ă��Ȃ��`��
	eERRORDS_STOP,		//	�X�g���[���I�[�i���[�v��false�j
	eERRORDS_NOTHING	//	�G���[�i�V
};

//	�G�t�F�N�g�t���O�ԍ�
enum MYEFFECTFLAG	
{
	eEFFECTDS_NOTHING = 0,	//	�G�t�F�N�g�͂Ȃ�
	eEFFECTDS_FADEIN,		//	�t�F�[�h�C��
	eEFFECTDS_FADEOUT,		//	�t�F�[�h�A�E�g
	eEFFECTDS_STOPREADY,	//	���X�g���[�����ɒ�~
	eEFFECTDS_STOP,			//	��~��
};

//	DirectMusic�\����
struct MYDS	
{
//	DirectSound
	IDirectSound8*			ds_ptr;			//	DirectSound�I�u�W�F�N�g
	IDirectSoundBuffer8*	sec_ptr[DS_MAX_BUFFER];	//	�Z�J���_���i��������́j�o�b�t�@
	IDirectSoundBuffer*		pri_ptr;	//	�v���C�}���i���j�o�b�t�@
//	�T�E���h�����
	DSCAPS					caps;			//	�f�o�C�X�\�͂��i�[
	DWORD					dwBufferSize;	//	�p�ӂ���o�b�t�@�̃T�C�Y
};

#ifdef DS_OGG_ENABLED
	#include "vorbis/vorbisfile.h"
#endif

//	�X�g���[�~���O�T�E���h���
struct MYSOUNDINFO
{
//	�X�g���[�����
	MYDSFORMAT format;				//	�Ώۃt�@�C���̃t�H�[�}�b�g
#ifdef DS_OGG_ENABLED
	OggVorbis_File	ovf;
	DWORD			dwFrac;	//�o�b�t�@�ɗ]���ɏ������񂾒[���o�C�g���i�[
#endif
	std::string	filename;	//	�Đ��Ώۂ̃t�@�C����
	DWORD	dwReadPosition;			//	�ǂ݂���ł���t�@�C���|�W�V����
	DWORD	dwHeaderSize;			//	�ǂ݂���ł���WAVE�w�b�_�̃T�C�Y
	int		iStreamPos;				//	�������݂̑Ώۂ�\���i0�Ȃ�㔼�A1�Ȃ�O���j
	bool	bStream;				//	�X�g���[�~���O�Đ��������邩�ǂ���
//	���[�v���
	bool	bLoop;					//	���[�v�����邩�ǂ���
	DWORD	dwReStart;				//	���[�v������Ƃ��A�ǂ��܂Ŗ߂邩
	DWORD	dwLoop;					//	�ǂ��܂ł����烋�[�v�����邩
	DWORD	dwCnt;					//	���񃋁[�v���J��Ԃ���
//	�t�F�[�h�C���A�E�g����
	MYEFFECTFLAG	effectflag;		//	�G�t�F�N�g�̎��
	int				iFade;			//	�t�F�[�h��
	long int		iVolume;		//	�{�����[��
  bool fadeout_stop;//�t�F�[�h�A�E�g���I�������true�I��false����0�ōĐ�������
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
//	���ݒ�
//--------------------------------------------------------------------------------------------
MYERROR_DS dsInitDirectSound(HWND, DWORD dwBufferSize = DS_STANDERDSIZE);	//	�T�E���h�������������܂�
void dsCheck_Stream(void);			//	�X�g���[�~���O�Đ����Ď����܂�(���t���[���K���Ă�)
void dsReleaseBuffer(int iNumber);	//	�w�肳�ꂽ�T�E���h�o�b�t�@��������܂�
void dsRelease(void);				//	�T�E���h����������܂�

//--------------------------------------------------------------------------------------------
//	���[�h�֐�
//--------------------------------------------------------------------------------------------
int dsLoadBGM(const std::string& filename);	//	WAVE�t�@�C�����X�g���[�~���O�Đ��ł����Ԃɂ��܂�
int dsLoadSE(const std::string& filename);	//	WAVE�t�@�C�����������Ƀ��[�h���܂�

//--------------------------------------------------------------------------------------------
//	�Đ��֐�
//--------------------------------------------------------------------------------------------
HRESULT dsPlay_Normal(int no, bool loop);		//	�o�b�t�@���̃f�[�^���Đ����܂�
HRESULT dsPlay_FadeIn(int no, DWORD, int, bool restart=true);	//	�o�b�t�@���̃f�[�^���t�F�[�h�C�����čĐ����܂�
HRESULT dsStop(int no);						//	�Đ����̃o�b�t�@���~�����܂�
HRESULT dsPause(int no);						//	�Đ����̃o�b�t�@���ꎞ��~�����܂�
HRESULT dsResume(int no);						//	�ꎞ��~�̃o�b�t�@���Đ������܂�
void dsFadeOut(int no, int, bool fadeout_stop = true);	//	�t�F�[�h�A�E�g�����܂�
void dsSetLoopInfo(int no, int, int iLoopCnt=0);	//	���[�v����ݒ肵�܂�

//--------------------------------------------------------------------------------------------
//	�Đ��p�����[�^�ύX�֐�
//--------------------------------------------------------------------------------------------
HRESULT dsSetFrequency(int, DWORD);		//	���g����ύX���܂�
HRESULT dsSetPan(int, int);				//	�p���ݒ���s���܂�
HRESULT dsSetVolume(int,long);			//	�{�����[���������s���܂�

//--------------------------------------------------------------------------------------------
//	�p�����[�^�擾�֐�
//--------------------------------------------------------------------------------------------
int dsGetVolume(int);					//	���݂̃{�����[���l���擾���܂�
DWORD dsGetStatus(int);					//	�o�b�t�@�̏�Ԃ��擾���܂�
void dsGetDeviceCaps(DSCAPS*);			//	�f�o�C�X�̏����擾���܂�
DWORD dsGetCurrentPosition(int iNumber);
DWORD dsGetSoundBufferSize(void);
bool dsIsLoop(int iNumber);


//--------------------------------------------------------------------------------------------
//	���̑�
//--------------------------------------------------------------------------------------------
MYERROR_DS dsGetWaveFormat(const std::string& filename, WAVEFORMATEX* wf_ptr);
void dsSetWaveFormat(WAVEFORMATEX* wf_ptr,DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample);


std::string dsGetErrorMessage(MYERROR_DS number);// �G���[�ԍ����當������擾���܂�



#ifdef DS_OGG_ENABLED
	#include "OggVorbis.h"
	#define dsoggIsOgg			ovIsOgg
	#define dsoggLoadOggSE		(MYERROR_DS)ovLoadOggSE
	#define dsoggLoadOggBGM		(MYERROR_DS)ovLoadOggBGM
	#define dsoggLoadOggStream	ovLoadOggStream
	#define dsoggInitOggEnv		ovInitOggEnvAll
	#define dsoggSeekBegin		ovSeekBegin
	//	�O�����C�u��������T�E���h�o�b�t�@�쐬���K�v�ȂƂ��p
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