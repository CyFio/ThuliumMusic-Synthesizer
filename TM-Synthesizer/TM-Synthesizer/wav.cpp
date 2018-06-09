#include"wav.h"

bool wav_output
(
	LPCSTR FileName,
	BYTE* wavedata,
	DWORD bytecount,
	WORD nChannels,
	DWORD nSamplesPerSec,
	WORD  wBitsPerSample,
	WORD  wFormatTag,
	LPCSTR lpAppendix
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
	wavfmt.cbSize = strlen(lpAppendix) + 1;
	DWORD dwRiffSize = 4 + sizeof(WAVEFORMATEX) + wavfmt.cbSize + 8 + bytecount;
	/*write RIFF Header*/
	wave.write("RIFF", 4);
	wave.write((char*)&dwRiffSize, sizeof(DWORD));
	wave.write("WAVE", 4);
	wave.write("fmt ", 4);
	wave.write((char*)&wavfmt, sizeof(WAVEFORMATEX));
	wave.write(lpAppendix, wavfmt.cbSize);
	wave.write("data ", 5);
	wave.write((char*)&bytecount, sizeof(DWORD));
	wave.write((char*)wavedata, bytecount);
	wave.close();
	return true;
}