#include"wav.h"

bool write(std::ofstream& wave, const void*data, size_t bitsize)
{
	wave.write((LPCSTR)data, bitsize);
	if (wave.bad())return false;
	return true;
}

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
	PCMWAVEFORMAT wavfmt;
	wavfmt.wf.wFormatTag = wFormatTag;
	wavfmt.wf.nChannels = nChannels;
	wavfmt.wf.nSamplesPerSec = nSamplesPerSec;
	wavfmt.wf.nBlockAlign = nChannels * wBitsPerSample >> 3;
	wavfmt.wf.nAvgBytesPerSec = wavfmt.wf.nBlockAlign * nSamplesPerSec;
	wavfmt.wBitsPerSample = wBitsPerSample;
	DWORD dwFmtSize = sizeof(PCMWAVEFORMAT) + 2;
	DWORD dwRiffSize = 9 * sizeof(DWORD) + 4 * sizeof(WORD) + bytecount;
	/*write RIFF chunk*/
	if (!write(wave, "RIFF", 4))return false;
	if (!write(wave, &dwRiffSize, sizeof(DWORD)))return false;
	if (!write(wave, "WAVE", 4))return false;
	/*write fmt chunk*/
	if (!write(wave, "fmt ", 4))return false;
	if (!write(wave, &dwFmtSize, sizeof(DWORD)))return false;
	if (!write(wave, &wavfmt.wf.wFormatTag, sizeof(WORD)))return false;
	if (!write(wave, &wavfmt.wf.nChannels, sizeof(WORD)))return false;
	if (!write(wave, &wavfmt.wf.nSamplesPerSec, sizeof(DWORD)))return false;
	if (!write(wave, &wavfmt.wf.nAvgBytesPerSec, sizeof(DWORD)))return false;
	if (!write(wave, &wavfmt.wf.nBlockAlign, sizeof(WORD)))return false;
	if (!write(wave, &wavfmt.wBitsPerSample, sizeof(WORD)))return false;
	/*write data chunk*/
	if (!write(wave, "data", 4))return false;
	if (!write(wave, &bytecount, sizeof(DWORD)))return false;
	if (!write(wave, wavedata, bytecount))return false;
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
	if (wave.fail())
	{
		wave.close();
		return false;
	}
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
	if (!write(wave, "RIFF", 4)) { wave.close(); return false; }
	if (!write(wave, &dwRiffSize, sizeof(DWORD))) {wave.close();return false;}
	if (!write(wave, "WAVE", 4)) { wave.close();return false;}
	/*write fmt chunk*/
	if (!write(wave, "fmt ", 4)) { wave.close();return false;}
	if (!write(wave, &dwFmtSize, sizeof(DWORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.wFormatTag, sizeof(WORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.nChannels, sizeof(WORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.nSamplesPerSec, sizeof(DWORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.nAvgBytesPerSec, sizeof(DWORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.nBlockAlign, sizeof(WORD))) { wave.close();return false;}
	if (!write(wave, &wavfmt.wBitsPerSample, sizeof(WORD))) { wave.close();return false;}
	if (!wave.write((char*)&wavfmt.cbSize, sizeof(WORD))) { wave.close();return false;}
	/*write fact chunk*/
	if (!write(wave, "fact", 4)) { wave.close();return false;}
	if (!write(wave, &dwFactSize, sizeof(DWORD))) { wave.close();return false;}
	if (!write(wave, lpFact, dwFactSize)) { wave.close();return false;}
	/*write data chunk*/
	if (!write(wave, "data", 4)) { wave.close();return false;}
	if (!write(wave, &bytecount, sizeof(DWORD))) { wave.close();return false;}
	if (!write(wave, wavedata, bytecount)) { wave.close();return false;}
	wave.close();
	return true;
}

bool read(std::ifstream& wave, void*data, size_t bitsize)
{
	wave.read((LPSTR)data, bitsize);
	if (wave.bad())return false;
	return true;
}

/*读取检测文件头，如果符合wave规范就返回文件长度 - 8，否则返回0*/
DWORD read_wav_riff(std::ifstream& wave)
{
	RIFF_Header Riff;
	if (!read(wave, &Riff, sizeof(RIFF_Header)))return 0;
//	if (Riff.dwRiffID != 0x52494646)return 0;
//	if (Riff.dwRiffFormat != 0x57475645)return 0;
	return Riff.dwRiffSize;
}

/*读取wav信息头*/
bool read_wav_fmt(std::ifstream& wave, tWAVEFORMATEX* wfx)
{
	DWORD dwFmtID;
	DWORD dwFmtSize;
//	wave.seekg(sizeof(RIFF_Header),wave.beg());
	if (!read(wave, &dwFmtID, sizeof(DWORD)))return false;
//	if (dwFmtID != 0x666D7420)return false;
	if (!read(wave, &dwFmtSize, sizeof(DWORD)))return false;
	if (dwFmtSize == 18)
	{
		if (!read(wave, wfx, sizeof(WAVEFORMATEX)))return false;
		return true;
	}
	else
		if (dwFmtSize == 16)
		{
			if(!read(wave,&(wfx->wFormatTag),sizeof(WORD)))return false;
			if(!read(wave,&(wfx->nChannels),sizeof(WORD)))return false;
			if(!read(wave,&(wfx->nSamplesPerSec),sizeof(DWORD)))return false;
			if(!read(wave,&(wfx->nAvgBytesPerSec),sizeof(DWORD)))return false;
			if(!read(wave,&(wfx->nBlockAlign),sizeof(WORD)))return false;
			if(!read(wave,&(wfx->wBitsPerSample),sizeof(WORD)))return false;
			wfx->cbSize = 0;
			return true;
		}
		else
			return false;
}

bool read_wav_data(std::ifstream& wave, BYTE* wavedata, DWORD*dwDataSize)
{
	DWORD dwDataID;
	if (!read(wave, &dwDataID, sizeof(DWORD)))return false;
//	if (dwDataID != 0x64617461)return false;
	if (!read(wave, dwDataSize, sizeof(DWORD)))return false;
	if (dwDataSize <= 0)return false;
	wavedata = new BYTE[*dwDataSize];
	if (!read(wave, wavedata, (size_t)dwDataSize))
	{
		delete[]wavedata;
		*dwDataSize = 0;
		return false;
	}
	return true;
}

wavfile::wavfile()
	:wavheader(new WAVEFORMATEX),wavedata(nullptr), lpFact(nullptr), state(false), dwRiffSize(0), dwDataSize(0)
{
}

wavfile::wavfile(LPCSTR filename)
	:wavheader(new WAVEFORMATEX), wavedata(nullptr), lpFact(nullptr), state(false), dwDataSize(0)
{
	std::ifstream wave(filename, std::ios::binary);
	if (wave.fail()) 
	{ 
		wave.close(); 
		return; 
	}//open failed
	dwRiffSize = read_wav_riff(wave);
	if (dwRiffSize < 20) 
	{ 
		wave.close(); 
		return; 
	}
	if (!read_wav_fmt(wave, wavheader)) 
	{ 
		wave.close();
		return;
	}
	if (wavheader->cbSize != 0)
	{
		lpFact = new char[wavheader->cbSize];
		{
			DWORD something;
			wave.read((char*)&something, sizeof(DWORD));
			wave.read((char*)&something, sizeof(DWORD));
		}
		if (!read(wave, lpFact, wavheader->cbSize))
		{
			delete[]lpFact;
			wave.close(); 
			return;
		}
	}
	if (!read_wav_data(wave, wavedata, &dwDataSize)) 
	{ 
		wave.close(); 
		return;
	}
	wave.close();
	state = true;
	return;
}

wavfile::wavfile(WAVEFORMATEX* header, BYTE* wavedata, DWORD bytecount)
	:wavheader(new WAVEFORMATEX), wavedata(new BYTE[bytecount]), lpFact(nullptr), state(false), dwDataSize(bytecount)
{
	*wavheader = *header;
	for (int i = 0; i < bytecount; ++i) 
	{
		wavfile::wavedata[i] = wavedata[i];
	}
}

wavfile::wavfile(BYTE* wavedata, DWORD bytecount)
	: wavheader(new WAVEFORMATEX), wavedata(new BYTE[bytecount]), lpFact(nullptr), state(false), dwDataSize(bytecount)
{
	wavheader->wFormatTag = 0x0001;
	wavheader->nChannels = 0x0002;
	wavheader->nSamplesPerSec = 44100;
	wavheader->wBitsPerSample = 16;
	wavheader->nBlockAlign = wavheader->nChannels * wavheader->wBitsPerSample << 3;
	wavheader->nAvgBytesPerSec = (DWORD)wavheader->nBlockAlign * wavheader->nSamplesPerSec;
	wavheader->cbSize = 0;
}

bool wavfile::load(const char*filename)
{
	std::ifstream wave(filename, std::ios::binary);
	if (wave.fail()) 
	{ 
		wave.close();
		return false; 
	}//open failed
	dwRiffSize = read_wav_riff(wave);
	if (dwRiffSize < 20)
	{
		wave.close();
		return false;
	}
	/*delete prev*/
	delete wavheader;
	delete[]wavedata;
	delete[]lpFact;
	state = false;
	/*read new*/
	if (!read_wav_fmt(wave, wavheader)) 
	{ 
		wave.close();
		return false;
	}
	if (wavheader->cbSize != 0)
	{
		lpFact = new char[wavheader->cbSize];
		{
			DWORD something;
			wave.read((char*)&something, sizeof(DWORD));
			wave.read((char*)&something, sizeof(DWORD));
		}
		if (!read(wave, lpFact, wavheader->cbSize))
		{
			delete[]lpFact;
			wave.close();
			return false;
		}
	}
	if (!read_wav_data(wave, wavedata, &dwDataSize))
	{
		wave.close();
		return false;
	}
	wave.close();
	state = true;
	return true;
}
bool wavfile::save(const char*filename)
{
	if (state == false)return false;
	if(lpFact!=nullptr)
		return wav_output(filename, wavedata, dwDataSize, lpFact, wavheader->nChannels, wavheader->nSamplesPerSec, wavheader->wBitsPerSample, wavheader->wFormatTag);
	else
		return wav_output(filename, wavedata, dwDataSize, wavheader->nChannels, wavheader->nSamplesPerSec, wavheader->wBitsPerSample, wavheader->wFormatTag);
}
wavfile::~wavfile()
{
	delete wavheader;
	delete[]wavedata;
	delete[]lpFact;
}
