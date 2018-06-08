#pragma once
//这里是由波形数据生成wav的部分
/*宏定义*/

/*库文件引用*/
#include<windows.h>
#include<fstream>
/*一般头文件引用*/

/*全局量声明*/

/*全局函数声明*/

/*结构体定义*/

/*类定义*/
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