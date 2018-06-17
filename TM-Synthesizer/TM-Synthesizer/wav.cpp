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
	DWORD dwFmtSize = sizeof(WAVEFORMATEX) - 2;
	DWORD dwRiffSize = 9 * sizeof(DWORD) + 4 * sizeof(WORD) + bytecount;
	/*write RIFF chunk*/
	wave.write("RIFF", 4);
	wave.write((char*)&dwRiffSize, sizeof(DWORD));
	wave.write("WAVE", 4);
	/*write fmt chunk*/
	wave.write("fmt ", 4);
	wave.write((char*)&dwFmtSize, sizeof(DWORD));
	wave.write((char*)&wavfmt.wFormatTag, sizeof(WORD));
	wave.write((char*)&wavfmt.nChannels, sizeof(WORD));
	wave.write((char*)&wavfmt.nSamplesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nAvgBytesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nBlockAlign, sizeof(WORD));
	wave.write((char*)&wavfmt.wBitsPerSample, sizeof(WORD));
	/*write data chunk*/
	wave.write("data", 4);
	wave.write((char*)&bytecount, sizeof(DWORD));
	wave.write((char*)wavedata, bytecount);
	wave.close();
	return true;
}

bool wav_output
(
	LPCSTR FileName,
	BYTE* wavedata,
	DWORD bytecount,
	LPCSTR lpFact, 
	WORD nChannels ,
	DWORD nSamplesPerSec,
	WORD  wBitsPerSample,
	WORD  wFormatTag
)

{
	std::ofstream wave;
	wave.open(FileName, std::ios::binary);
	if (wave.fail())return false;
	WAVEFORMATEX wavfmt;
	DWORD dwFactSize = strlen(lpFact) + 1;
	DWORD dwFmtSize = sizeof(WAVEFORMATEX);
	wavfmt.wFormatTag = wFormatTag;
	wavfmt.nChannels = nChannels;
	wavfmt.nSamplesPerSec = nSamplesPerSec;
	wavfmt.nBlockAlign = nChannels * wBitsPerSample >> 3;
	wavfmt.nAvgBytesPerSec = wavfmt.nBlockAlign * nSamplesPerSec;
	wavfmt.wBitsPerSample = wBitsPerSample;
	wavfmt.cbSize = 2*sizeof(DWORD) + dwFactSize;
	DWORD dwRiffSize = 11 * sizeof(DWORD) + 5 * sizeof(WORD) + dwFactSize + bytecount;
	/*write RIFF chunk*/
	wave.write("RIFF", 4);
	wave.write((char*)&dwRiffSize, sizeof(DWORD));
	wave.write("WAVE", 4);
	/*write fmt chunk*/
	wave.write("fmt ", 4);
	wave.write((char*)&dwFmtSize, sizeof(DWORD));
	wave.write((char*)&wavfmt.wFormatTag, sizeof(WORD));
	wave.write((char*)&wavfmt.nChannels, sizeof(WORD));
	wave.write((char*)&wavfmt.nSamplesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nAvgBytesPerSec, sizeof(DWORD));
	wave.write((char*)&wavfmt.nBlockAlign, sizeof(WORD));
	wave.write((char*)&wavfmt.wBitsPerSample, sizeof(WORD));
	wave.write((char*)&wavfmt.cbSize, sizeof(WORD));
	/*write fact chunk*/
	wave.write("fact", 4);
	wave.write((char*)&dwFactSize, sizeof(DWORD));
	wave.write(lpFact, dwFactSize);
	/*write data chunk*/
	wave.write("data", 4);
	wave.write((char*)&bytecount, sizeof(DWORD));
	wave.write((char*)wavedata, bytecount);
	wave.close();
	return true;
}