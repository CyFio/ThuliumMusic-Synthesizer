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

	ofstream wavef;

	tms_input input;

	tsf *tiniSF = nullptr; //����tsf_load_filename���ص���һ��ָ�룬�ƺ�tsf�ṹ���Ǵ����ڶ��ڴ��еġ�

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

	bool jsonInput();//�ɹ�����true��ʧ�ܷ���false

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

