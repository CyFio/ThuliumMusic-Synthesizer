#pragma once
//�������ɲ�����������wav�Ĳ���
/*�궨��*/

/*���ļ�����*/
#include<windows.h>
#include<fstream>
/*һ��ͷ�ļ�����*/

/*ȫ��������*/

/*ȫ�ֺ�������*/

/*�ṹ�嶨��*/

/*�ඨ��*/
class wavfile
{
private:
	WAVEFORMATEX wavheader;
	BYTE* wavedata;
	bool state;
public:
	wavfile();
	wavfile(const char* filename);
	wavfile(WAVEFORMATEX& header, BYTE* wavedata, size_t bytesize);
	wavfile(BYTE* wavedata, size_t bytesize);
	bool setHeader(WAVEFORMATEX& header);
	//bool setHeader(tms_output_settings& settings);
	bool setHeader
	(
		WORD        wFormatTag,
		WORD        nChannels,         /* number of channels (i.e. mono, stereo...) */
		DWORD       nSamplesPerSec,     /* sample rate */
		DWORD       nAvgBytesPerSec,    /* for buffer estimation */
		WORD        nBlockAlign,        /* block size of data */
		WORD        wBitsPerSample,     /* number of bits per sample of mono data */
		WORD        cbSize
	);
	bool load(const char*filename);
	bool save(const char*filename);
	~wavfile();
};