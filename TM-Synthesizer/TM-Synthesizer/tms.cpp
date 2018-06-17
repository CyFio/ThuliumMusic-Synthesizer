#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "tms.h"

using namespace std;

void _mixer(BYTE** wave, int waves_num, int bytes_num, BYTE* outwave)
{
	int bit;
	double divisor = 3;
	int samples_num = bytes_num / 2;
	for (int samples_No = 0; samples_No < samples_num; ++samples_No)
	{
		bit = 0;
		for (int wave_No = 0; wave_No < waves_num; ++wave_No)
		{
			bit += (wave[wave_No][2 * samples_No] << 8) + wave[wave_No][2 * samples_No + 1];
		}
		if (bit >= (1 << 16))
			outwave[2 * samples_No] = outwave[2 * samples_No + 1] = (1 << 8) - 1;
		else
		{
			outwave[2 * samples_No] = (int)(bit / divisor) >> 8;
			outwave[2 * samples_No + 1] = bit / divisor;
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

	tsf_set_output(this->tiniSF, TSF_STEREO_INTERLEAVED, this->output.settings.frequency);

	this->output.waves = new BYTE*[this->input->numberOfTracks];
	for (int track_No = 0; track_No < input->numberOfTracks; ++track_No)
	{
		this->output.waves[track_No] = new BYTE[this->output.settings.samples * this->output.settings.bitsPerSample / 8 * 2];
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
	tsf_set_output(this->tiniSF, this->output.settings.channels == 0 ? TSF_MONO : TSF_STEREO_INTERLEAVED, this->output.settings.frequency); //输出设置

	int presetIndex = 0;
	for (; presetIndex < this->tiniSF->presetNum && track.settings.instrument != this->tiniSF->presets[presetIndex].presetName; ++presetIndex); //乐器匹配

	for (; sample_No < samples; sample_No += block)
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

		tsf_render_float(this->tiniSF, (float*)(wave + 4 * sample_No), block);
	}

	return true;
}

bool tms::mixer()
{
	this->output.outwave = new BYTE[this->output.settings.samples * this->output.settings.bitsPerSample / 8 * 2];
	_mixer(this->output.waves, this->output.numberOfWaves, this->output.settings.samples * this->output.settings.bitsPerSample / 8, this->output.outwave);
	return true;
}

tms tms1;

int main()
{
	tms1("test.output.json", "florestan-subset.sf2", "test.wav");



	return 0;
}

