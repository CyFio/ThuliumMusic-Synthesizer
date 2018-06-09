#pragma once
//�������ɲ�����������wav�Ĳ���
/*�궨��*/

/*���ļ�����*/
#include<windows.h>
#include<iostream>
#include<fstream>
#include<string>
/*һ��ͷ�ļ�����*/

/*ȫ��������*/

/*�ṹ������*/

/*������*/
class wavfile;
/*ȫ�ֺ�������*/
bool wav_output
(
	LPCSTR FileName,//Path of the File.wav
	BYTE* wavedata, //Music data
	DWORD bytecount, //Size of the data in bytes
	WORD nChannels = 0x0002,//Number of channels(default = 2)
	DWORD nSamplesPerSec =44100,//Samples per second(default = 44.1kHz 
	WORD  wBitsPerSample = 16, //bits per sample
	WORD  wFormatTag = 0x0001,//PCM
	LPCSTR lpAppendix = "Produced by Thulium"
);
/*�ṹ�嶨��*/
struct RIFF_Header
{
	LPCSTR lpRiffID = "RIFF";
	DWORD dwRiffSize;
	LPSTR lpRiffFormat;
};

/*�ඨ��*/
class wavfile
{
private:
	WAVEFORMATEX wavheader;
	BYTE* wavedata;
	bool state;
public:
	wavfile();
	wavfile(LPCSTR filename);
	wavfile(WAVEFORMATEX& header, BYTE* wavedata, size_t bytesize);
	wavfile(BYTE* wavedata, size_t bytecount);
	bool setFormat(WAVEFORMATEX& header);
	//bool setHeader(tms_output_settings& settings);
	bool setFormat
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