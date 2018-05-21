#pragma once

using namespace std;

struct tms_note
{

};

struct tms_track_warnings
{

};

struct tms_track_settings
{

};

struct tms_track_meta
{

};

struct tms_track
{

	tms_note *notes = nullptr;

	int number;

	tms_track_warnings warnings;

	tms_track_settings settings;

	tms_track_meta meta;

	string instrument;

	string ID;
};

struct tms_chunk_warnings
{

};

struct tms_chunk
{

	tms_track *trcaks = nullptr;

	int number;

	tms_chunk_warnings warnings;
};

struct tms_input
{

	tms_chunk *chunks = nullptr;

	int number;
};

struct tms_wave
{
	float *buffer;

	int length;

	int channels;
};

struct tms_output
{

	tms_wave *waves = nullptr;

	int number;

	tms_wave *outwave = new tms_wave;
};

class tms
{

	ifstream json;

	ifstream sf;

	tms_input input;

	tsf tiniSF;

	//...

	tms_output output;

public:

	void jsonInput();

	void jsonInput(string jsonfp);

	void sfInput();

	//...

	string operator()(string jsonfp, string sffp);
};

