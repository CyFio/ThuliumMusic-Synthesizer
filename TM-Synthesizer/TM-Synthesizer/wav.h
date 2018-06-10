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
bool wav_output//output wavefile with no extra infomation
(
	LPCSTR FileName,//Path of the File.wav
	BYTE* wavedata, //Music data
	DWORD bytecount, //Size of the data in bytes
	WORD nChannels = 0x0002,//Number of channels(default = 2)
	DWORD nSamplesPerSec =44100,//Samples per second(default = 44.1kHz 
	WORD  wBitsPerSample = 16, //bits per sample
	WORD  wFormatTag = 0x0001//PCM
);
bool wav_output//output wavefile with no extra infomation
(
	LPCSTR FileName,//Path of the File.wav
	BYTE* wavedata, //Music data
	DWORD bytecount, //Size of the data in bytes
	LPCSTR lpAppendix, //infomation
	WORD nChannels = 0x0002,//Number of channels(default = 2)
	DWORD nSamplesPerSec = 44100,//Samples per second(default = 44.1kHz 
	WORD  wBitsPerSample = 16, //bits per sample
	WORD  wFormatTag = 0x0001//PCM
);
/*�ṹ�嶨��*/
struct RIFF_Header
{
	DWORD		dwRiffID;
	DWORD		dwRiffSize;
	DWORD		dwRiffFormat;
};

/*�ඨ��*/
class wavfile
{
private:
	WAVEFORMATEX* wavheader;
	BYTE* wavedata;
	LPSTR lpFact;
	DWORD dwRiffSize;
	DWORD dwDataSize;
	bool state;
public:
	wavfile();
	wavfile(LPCSTR filename);
	wavfile(WAVEFORMATEX* header, BYTE* wavedata, size_t bytesize);
	wavfile(BYTE* wavedata, size_t bytecount);
	bool load(const char*filename);
	bool save(const char*filename);
	~wavfile();
};