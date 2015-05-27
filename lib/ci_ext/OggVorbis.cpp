//--------------------------------------------------------------------------------------------
//	�t�@�C����	�F	OggVorbis.cpp
//	�A�v����	�F	�ėp�I�ȃt�@�C���Ȃ̂ŁA���s�t�@�C���͕s����
//	�쐬��		�F	�A�R����
//	�쐬��		�F	2011.03.03
//	�@�\		�F	ds2008��Ogg�Đ���g�ݍ��ރ��b�s���O�֐��S�ł��B
//	�g�p���@	�F
//	�g�p�w�b�_	�F	OggVorbis.h
//	�X�V����	�F	2011.03.03	Ver1.00		�A�R����	��{�I�ȋ@�\�̎���
//				2014.01.20	Ver1.01		�A�R����	ovSeekBegin�̒ǉ�
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
//	Ogg�ǂݍ��ݏ������������܂�
//--------------------------------------------------------------------------------------------
void InitOggData(OggVorbis_File* s_ptr)
{
	if(s_ptr->datasource != NULL){
		ov_clear(s_ptr);
	}
}

//--------------------------------------------------------------------------------------------
//	Ogg�ǂݍ��ݏ������������܂�
//--------------------------------------------------------------------------------------------
static void ovInitOggEnv(OggVorbis_File* s_ptr)
{
	if(s_ptr->datasource != NULL)
	{
		ov_clear(s_ptr);
	}
}

//--------------------------------------------------------------------------------------------
//	Ogg�Đ���擪�֖߂��܂�
//--------------------------------------------------------------------------------------------
void ovSeekBegin(OggVorbis_File* s_ptr)
{
	ov_pcm_seek(s_ptr, 0);
}
//--------------------------------------------------------------------------------------------
//	Ogg�ǂݍ��ݏ������������܂�
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
//	Ogg���f�R�[�h������APCM�f�[�^�̃T�C�Y�𒲂ׂ܂�
//--------------------------------------------------------------------------------------------
static DWORD ovGetPcmSize(OggVorbis_File* ovf)
{
	vorbis_info* info;
	ogg_int64_t sampleNum;
	DWORD dwPCMBytes;

	//�J���Ă���ogg�t�@�C���̑��T���v�����ƃ`�����l�����𒲂�
	//�K�v�ȃo�C�g�����擾����
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
//	�`�����N��Ogg���ǂ����𒲂ׂ�
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
//	Windows�p�ǂݍ��݊֐�
//--------------------------------------------------------------------------------------------
static long ovRead(OggVorbis_File* ovf_ptr, char* b_ptr,int* bs_ptr)
{
	return ov_read(ovf_ptr, b_ptr, OV_BUFFER_SIZE, 
				   0,	// ���g���G���f�B�A���A�r�b�O�Ȃ�1���w��
				   2,	// WORD�^��Byte���w��AWindows��2Byte
				   1,	// �����L��
				   bs_ptr);
}

//--------------------------------------------------------------------------------------------
//	ogg�`����PCM�`���փf�R�[�h���ADS�̃o�b�t�@�ɏ������݂܂�
//--------------------------------------------------------------------------------------------
static int ovCreateBuffer(OggVorbis_File* ovf_ptr, int bufNumber, const char* filename, DWORD dwSize)
{
	int errorNo;

//	�t�@�C�����J���Aogg�łȂ���΃G���[�ԍ���Ԃ�
	ovInitOggEnv(ovf_ptr);
	errorNo = ov_fopen(filename, ovf_ptr);
	if(errorNo != OV_SUCCESSFUL)	return errorNo;

//	PCM�̌`����������x���ׂ�
//	�T���v�����O���[�g�ƃ`�����l�����̂݁H
	vorbis_info *info = ov_info( ovf_ptr, -1 );
	ogg_int64_t t = ov_pcm_total(ovf_ptr, -1);

//	DirectSoundBuffer�ݒ�
	WAVEFORMATEX wf;
	//���āA�f�R�[�h����PCM�̃r�b�g���[�g16�������I�ɋ��߂�ɂ́c
	dsSetWaveFormat(&wf, info->rate, info->channels, OV_PCM_BITRATE);
	dwSize = dwSize == 0 ? ovGetPcmSize(ovf_ptr): dwSize;
	dsCreateSoundBuffer(bufNumber, &wf, dwSize);

	return OV_SUCCESSFUL;
}

//--------------------------------------------------------------------------------------------
//	ogg�`����PCM�`���փf�R�[�h���ADirectSound�o�b�t�@�ɂ��ׂď������݂܂�
//--------------------------------------------------------------------------------------------
int ovLoadOggSE(int bufNumber, const std::string& filename)
{
  OggVorbis_File ovf = {};
	char buffer[OV_BUFFER_SIZE];
	int readSize = 0;
	int readTotalSize = 0;

//	ogg��PCM�ɕϊ������T�C�Y��DirectSound�o�b�t�@�Ƀ��[�h����
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
//	ogg�`����PCM�`���փf�R�[�h���ADS�̃o�b�t�@�ɏ������݂܂�
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

	//�o�b�t�@�̃T�C�Y�ɂȂ�܂œǂݍ���
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
	//�K��ʂ𒴂����̂ŁADirectSound�̃o�b�t�@�ɏ�������
	dsWriteSoundData(bufNumber, img, iWriteOffset + s_ptr->dwFrac, readTotalSize);
	//����A�[���̌v�Z
	s_ptr->dwFrac = (readTotalSize + s_ptr->dwFrac) - dsbufferSize;
	delete[] img;
	return ret;
}
//--------------------------------------------------------------------------------------------
//	ogg�`����PCM�`���փf�R�[�h���ADirectSound�̃o�b�t�@�ɏ������݂܂�
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