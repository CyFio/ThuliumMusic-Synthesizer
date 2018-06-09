#include"wav.h"

bool wav_output
(
	LPCSTR FileName,
	BYTE* wavedata,
	DWORD bytecount,
	WORD nChannels,
	DWORD nSamplesPerSec,
	WORD  wBitsPerSample,
	WORD  wFormatTag
)
{
	std::ofstream wave;
	wave.open(FileName,std::ios::binary);
	if (wave.fail())return false;
	WAVEFORMATEX wavfmt;
	wavfmt.wFormatTag = wFormatTag;
	wavfmt.nChannels = nChannels;
	wavfmt.nSamplesPerSec = nSamplesPerSec;
	wavfmt.nBlockAlign = nChannels * wBitsPerSample >> 3;
	wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * nSamplesPerSec;
	wavfmt.wBitsPerSample = wBitsPerSample;
	wavfmt.cbSize = 0;
	//wavfmt.cbSize = strlen(lpAppendix) + 1;
	DWORD dwFmtSize = sizeof(WAVEFORMATEX) - 2;
	DWORD dwRiffSize = 4 + sizeof(WAVEFORMATEX) + wavfmt.cbSize + 8 + bytecount;
	/*write RIFF Header*/
	wave.write("RIFF", 4);
	wave.write((char*)&dwRiffSize, sizeof(DWORD));
	wave.write("WAVE", 4);
	wave.write("fmt ", 4);
	wave.write((char*)&dwFmtSize, sizeof(DWORD));
	//wave.write((char*)&wavfmt, sizeof(WAVEFORMATEX));
	wave.write((char*)&wavfmt.wFormatTag, sizeof(WORD));
	wave.write((char*)&wavfmt.nChannels, sizeof(WORD));
	wave.write((char*)&wavfmt.nSamplesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nAvgBytesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nBlockAlign, sizeof(WORD));
	wave.write((char*)&wavfmt.wBitsPerSample, sizeof(WORD));
	//wave.write((char*)&wavfmt.cbSize, sizeof(WORD));
	//wave.write(lpAppendix, wavfmt.cbSize);
	wave.write("data", 4);
	wave.write((char*)&bytecount, sizeof(DWORD));
	wave.write((char*)wavedata, bytecount);
	wave.close();
	return true;
}