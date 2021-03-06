#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "tms.h"

using namespace std;

void _mixer(BYTE** wave, int waves_num, int bytes_num, BYTE* outwave)
{
	int bit;
	double divisor = 2;
	int samples_num = bytes_num / 2;
	for (int samples_No = 0; samples_No < samples_num; ++samples_No)
	{
		bit = 0;
		for (int wave_No = 0; wave_No < waves_num; ++wave_No)
		{
			WORD a = (wave[wave_No][2 * samples_No]) + (wave[wave_No][2 * samples_No + 1] << 8);
			bit += (short&)a;
		}
		bit /= divisor;
		if (bit >= (1 << 15))
		{
			outwave[2 * samples_No] = (1 << 8) - 1;
			outwave[2 * samples_No + 1] = (1 << 7) - 1;
		}
		else
		{
			outwave[2 * samples_No] = bit;
			outwave[2 * samples_No + 1] = (int)(bit) >> 8;
		}
	}
}

bool tms::synthesizer()
{
	bool correct = true;
	double endtime = 0;
	for (int track_No = 0; track_No < input->numberOfTracks; ++track_No)
	{
		double endtime_track = this->input->tracks[track_No].notes[this->input->tracks[track_No].numberOfnotes - 1].time_start + this->input->tracks[track_No].notes[this->input->tracks[track_No].numberOfnotes - 1].time_duration;
		if (endtime < endtime_track)
			endtime = endtime_track;
	}
	this->output.settings.samples = (int)((endtime + 1) * this->output.settings.frequency * 2); //估计样本数量

	this->output.waves = new BYTE*[this->input->numberOfTracks];
	for (int track_No = 0; track_No < input->numberOfTracks; ++track_No)
	{
		this->output.waves[track_No] = new BYTE[this->output.settings.samples * this->output.settings.bitsPerSample / 8];
		correct = correct && synthesizer_track(track_No);
	}
	correct = correct && mixer();
	return correct;
}

bool tms::synthesizer_track(int track_No)
{
	double time = 0;
	int sample_No = 0;
	int note_on_No = 0;
	int note_off_No = 0;
	int block = TSF_RENDER_EFFECTSAMPLEBLOCK;
	double interval = (double)block / (double)this->output.settings.frequency;
	const int samples = this->output.settings.samples;
	const tms_track &track = this->input->tracks[track_No];
	BYTE *&wave = this->output.waves[track_No];
	tsf *tiniSF = nullptr;

	//if we use more than one SoundFont files, we find and load the matched SF file here.
	if (!this->singleSF) tiniSF = tsf_load_filename(this->instru->getInstrument(this->input->tracks->settings.instrument.c_str()));
	else tiniSF = this->tiniSF;

	//find the matched instrument in tsf.
	int presetIndex = 0;
	for (; presetIndex < tiniSF->presetNum && track.settings.instrument != tiniSF->presets[presetIndex].presetName; ++presetIndex); //乐器匹配

	tsf_set_output(tiniSF, this->output.settings.channels == 0 ? TSF_MONO : TSF_STEREO_INTERLEAVED, this->output.settings.frequency); //输出设置

	for (; sample_No < samples; sample_No += 2 * block)
	{
		if (sample_No + block > samples)
		{
			block = samples - sample_No;
			interval = (double)block / (double)this->output.settings.frequency;
		}
		time += interval;
		for (; note_on_No < track.numberOfnotes && track.notes[note_on_No].time_start < time; ++note_on_No)
		{
			tsf_note_on(tiniSF, presetIndex, track.notes[note_on_No].pit + PIT_OFFSET, track.notes[note_on_No].vol);
		}
		for (; note_off_No < track.numberOfnotes && track.notes[note_off_No].time_start + track.notes[note_off_No].time_duration < time; ++note_off_No)
		{
			tsf_note_off(tiniSF, presetIndex, track.notes[note_off_No].pit + PIT_OFFSET);
		}

		tsf_render_short(tiniSF, (short*)(wave + 2 * sample_No), block);
		//if (time > 1)
		//{
		//	for (int i = 0; i < 8 * block; ++i) cout << (int)*(wave + 8 * sample_No + i) << ends;
		//	char a[1];
		//	cin.getline(a, 1);
		//}
	}

	return true;
}

bool tms::mixer()
{
	this->output.outwave = new BYTE[this->output.settings.samples * this->output.settings.bitsPerSample / 8];
	_mixer(this->output.waves, this->input->numberOfTracks, this->output.settings.samples * this->output.settings.bitsPerSample / 8, this->output.outwave);
	return true;
}

tms tms1;

int main()
{
	tms1("test.output2.json", "florestan-subset.sf2", "test.wav");



	return 0;
}
