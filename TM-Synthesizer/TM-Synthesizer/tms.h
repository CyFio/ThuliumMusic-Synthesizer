#pragma once

using namespace std;

/*
1、使用固定唱名法，实际音高 = pitch + 60
2、使用绝对音量
3、除音符开始和结束以外，其他音符事件（如滑音、音量衰减等）待定
4、对多音轨合成暂无要求，是否需要对音轨加特技（Duang~）（淡入淡出等）待定
*/

struct tms_note
{
	int pit;				//音高

	double vol;				//音量

	double time_start;		//音符开始时刻

	double time_duration;	//音符持续时长
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
	int frequency;		//通常是44100Hz

	int bitsPerSample;	//通常是16

	int channels;		//0表示单声道，1表示双声道

	int samples;		//PCM样本数量
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

