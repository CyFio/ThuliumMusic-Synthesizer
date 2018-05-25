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

	ofstream wavef;

	tms_input input;

	tsf *tiniSF = nullptr; //函数tsf_load_filename返回的是一个指针，似乎tsf结构体是储存在堆内存中的。

	//...

	tms_output output;

	bool synthesizer_track(int track_No);

	bool mixer();

public:

	tms() = default;

	tms(string jsonfp, string sffp)
	{
		this->setjson(jsonfp);
		this->setsf(sffp);
	}

	tms(const tms&) = delete;
	tms& operator=(const tms&) = delete;

	~tms()
	{
		tsf_close(tiniSF);
	}

	bool setjson(string jsonfp)
	{
		this->jsonf.open(jsonfp);
		if (jsonf.is_open()) return true;
		else return false;
	}

	bool setsf(string sffp)
	{
		this->sff.open(sffp);
		if (sff.is_open()) return true;
		else return false;
	}

	bool setwavef(string wavefp)
	{
		this->wavef.open(wavefp);
		if (wavef.is_open()) return true;
		else return false;
	}

	bool jsonInput();//成功返回true，失败返回false

	bool jsonInput(string jsonfp)
	{
		if (!this->setjson(jsonfp)) return false;
		else return this->jsonInput();
	}

	bool sfInput(string sffp)
	{

		tiniSF = tsf_load_filename(sffp.c_str());
		if (tiniSF) return true;
		else return false;
	}

	bool synthesizer()
	{
		bool correct = true;
		for (int track_No = 0; track_No < input.numberOfTracks; ++track_No)
			correct = correct && synthesizer_track(track_No);
		correct = correct && mixer();
		return correct;
	}

	bool wave_output();

	bool wave_output(string wavefp)
	{
		if (!setwavef(wavefp)) return false;
		else return wave_output();
	}

	bool operation(string jsonfp, string sffp, string wavefp)
	{
		bool correct = true;
		correct = correct && jsonInput(jsonfp);
		correct = correct && sfInput(sffp);
		correct = correct && synthesizer();
		correct = correct && wave_output(wavefp);
		return correct;
	}

	bool operator()(string jsonfp, string sffp, string wavefp)
	{
		return operation(jsonfp, sffp, wavefp);
	}
};

