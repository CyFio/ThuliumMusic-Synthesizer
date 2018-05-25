#pragma once

using namespace std;

/*
1��ʹ�ù̶���������ʵ������ = pitch + 60
2��ʹ�þ�������
3����������ʼ�ͽ������⣬���������¼����绬��������˥���ȣ�����
4���Զ�����ϳ�����Ҫ���Ƿ���Ҫ��������ؼ���Duang~�������뵭���ȣ�����
*/

struct tms_note
{
	int pit;				//����

	double vol;				//����

	double time_start;		//������ʼʱ��

	double time_duration;	//��������ʱ��
};

struct tms_track_settings
{
	string instrument;
};

struct tms_track
{

	tms_note *notes = nullptr;

	int numberOfnotes;

	tms_track_settings settings;

	~tms_track()
	{
		delete[] notes;
	}
};

struct tms_input
{

	tms_track *tracks = nullptr;

	int numberOfTracks;

	~tms_input()
	{
		delete[] tracks;
	}
};

struct tms_output_settings
{
	int frequency;		//ͨ����44100Hz

	int bitsPerSample;	//ͨ����16

	int channels;		//0��ʾ��������1��ʾ˫����

	int samples;		//PCM��������
};

struct tms_wave
{
	float *samples = nullptr;

	int numberOfSamples;

	~tms_wave()
	{
		delete[] samples;
	}
};

struct tms_output
{

	tms_wave *waves = nullptr;

	int numberOfWaves;

	tms_wave *outwave = new tms_wave;

	tms_output_settings settings;

	~tms_output()
	{
		delete[] waves;
		delete outwave;
	}
};

class tms
{

	ifstream jsonf;

	ifstream sff;

	ifstream wavef;

	tms_input input;

	tsf tiniSF;

	//...

	tms_output output;

public:

	tms() = default;

	tms(string jsonfp, string sffp)
	{
		this->setjson(jsonfp);
		this->setsf(sffp);
	}

	bool setjson(string jsonfp)
	{
		this->jsonf.open(jsonfp);
		if (jsonf.is_open()) return 1;
		else return 0;
	}

	bool setsf(string sffp)
	{
		this->sff.open(sffp);
		if (sff.is_open()) return 1;
		else return 0;
	}

	bool jsonInput();

	bool jsonInput(string jsonfp)
	{
		if (!this->setjson(jsonfp)) return false;
		else return this->jsonInput();
	}

	void sfInput();

	//...

	string operator()(string jsonfp, string sffp, string wavefp);
};

