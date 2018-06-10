#pragma once
//这里是由波形数据生成wav的部分
/*宏定义*/

/*库文件引用*/
#include<windows.h>
#include<iostream>
#include<fstream>
#include<string>
/*一般头文件引用*/

/*全局量声明*/

/*结构体声明*/

/*类声明*/
class wavfile;
/*全局函数声明*/
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
/*结构体定义*/
struct RIFF_Header
{
	DWORD		dwRiffID;
	DWORD		dwRiffSize;
	DWORD		dwRiffFormat;
};

/*类定义*/
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
	wavfile(WAVEFORMATEX* header, BYTE* wavedata, DWORD bytecount);
	wavfile(BYTE* wavedata, DWORD bytecount);
	bool load(const char*filename);
	bool save(const char*filename);
	~wavfile();
};