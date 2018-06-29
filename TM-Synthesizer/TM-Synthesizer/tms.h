#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <thread>
#include "wav.h"
#include "json.h"
#define PIT_OFFSET 60

using namespace std;

/*
1��ʹ�ù̶���������ʵ������ = pitch + 60
2��ʹ�þ�������
3����������ʼ�ͽ������⣬���������¼����绬��������˥���ȣ�����
4���Զ�����ϳ�����Ҫ���Ƿ���Ҫ��������ؼ���Duang~�������뵭���ȣ�����
*/

struct tms_output_settings
{
	int frequency;		//ͨ����44100Hz

	int bitsPerSample;	//ͨ����16

	int channels;		//0��ʾ��������1��ʾ˫����

	int samples;		//PCM��������
};

struct tms_output
{

	BYTE **waves = nullptr;

	int numberOfWaves;

	BYTE *outwave = nullptr;

	tms_output_settings settings;

	~tms_output()
	{
		for (int i = 0; i < numberOfWaves; ++i)
			delete waves[i];
		delete[] waves;
		delete outwave;
	}
};

class tms
{

	string jsonfp;

	string sffp;

	string wavefp;

	tms_input *input = nullptr;

	instrumentIndex *instru = nullptr;

	//tsf *tiniSF = nullptr; //����tsf_load_filename���ص���һ��ָ�룬�ƺ�tsf�ṹ���Ǵ����ڶ��ڴ��еġ�

	bool *mutex = nullptr;

	bool multiThreads;

	//...

	tms_output output;

	bool singleSF;


	bool mixer();

public:

	void synthesizer_track(int track_No);

	tms() = default;

	tms(const string& jsonfp, const string& sffp, const string& wavefp)
	{
		this->setjson(jsonfp);
		this->setsf(sffp);
		this->setwavef(wavefp);
	}

	tms(const tms&) = delete;

	tms& operator=(const tms&) = delete;

	~tms()
	{
		delete input;
		delete instru;
		delete mutex;
		//tsf_close(tiniSF);
	}

	bool setjson(string jsonfp)
	{
		this->jsonfp = jsonfp;
		return true;
	}

	bool setsf(string sffp)
	{
		this->sffp = sffp;
		return true;
	}

	bool setwavef(string wavefp)
	{
		this->wavefp = wavefp;
		return true;
	}

	bool jsonInput()//�ɹ�����true��ʧ�ܷ���false
	{
		return cJsonMusic(this->input, this->jsonfp.c_str());
	}

	bool jsonInput(string jsonfp)
	{
		if (!this->setjson(jsonfp)) return false;
		else return this->jsonInput();
	}

	bool sfInput()
	{
		//tiniSF = tsf_load_filename(sffp.c_str());
		this->singleSF = true;
		//if (tiniSF) return true;
		//else return false;
		return true;
	}

	bool sfInput(string sffp)
	{
		this->sffp = sffp;
		return sfInput();
	}

	bool sfInput2()
	{
		instru = new instrumentIndex(this->sffp.c_str());
		this->singleSF = false;
		return instru->fail();
	}

	bool sfInput2(string sffp)
	{
		this->sffp = sffp;
		return sfInput2();
	}

	void setOutputDefault()
	{
		this->output.settings.bitsPerSample = 16;
		this->output.settings.channels = 1;
		this->output.settings.frequency = 44100;
	}

	bool synthesizer();

	bool waveOutput()
	{
		//wavfile wav((BYTE*)output.outwave, output.settings.samples * sizeof(float));
		//return wav.save(wavefp.c_str());
		//wav_output("test_track0.wav", this->output.waves[0], this->output.settings.samples * this->output.settings.bitsPerSample / 8);
		//wav_output("test_track1.wav", this->output.waves[1], this->output.settings.samples * this->output.settings.bitsPerSample / 8);
		return wav_output(this->wavefp.c_str(), this->output.outwave, this->output.settings.samples * this->output.settings.bitsPerSample / 8);
	}

	bool waveOutput(string wavefp)
	{
		if (!setwavef(wavefp)) return false;
		else return waveOutput();
	}



	bool operation(string jsonfp, string sffp, string wavefp, bool multiThreads = true)
	{
		bool correct = true;
		this->multiThreads = multiThreads;
		setOutputDefault();
		correct = correct && jsonInput(jsonfp);
		correct = correct && sfInput2(sffp);
		correct = correct && synthesizer();
		correct = correct && waveOutput(wavefp);
		return correct;
	}

	bool operator()(string jsonfp, string sffp, string wavefp, bool multiThreads = true)
	{
		return operation(jsonfp, sffp, wavefp, multiThreads);
	}
};
