#define TSF_IMPLEMENTATION
#include "tsf.h"


#include "tms.h"

bool tms::synthesizer()
{
	bool correct = true;
	double endtime = 0;
	for (int track_No = 0; track_No < input.numberOfTracks; ++track_No)
	{
		double endtime_track = this->input.tracks[track_No].notes[this->input.tracks[track_No].numberOfnotes - 1].time_start + this->input.tracks[track_No].notes[this->input.tracks[track_No].numberOfnotes - 1].time_duration;
		if (endtime < endtime_track)
			endtime = endtime_track;
	}
	this->output.settings.samples = (int)((endtime + 1) * this->output.settings.frequency); //估计样本数量

	for (int track_No = 0; track_No < input.numberOfTracks; ++track_No)
		correct = correct && synthesizer_track(track_No);
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
	const tms_track &track = this->input.tracks[track_No];
	tms_wave &wave = this->output.waves[track_No];
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

		tsf_render_float(this->tiniSF, wave.samples + sample_No, block);
	}

	return true;
}

bool tms::mixer()
{
	int sample_No = 0;
	int wave_No = 0;
	const int samples = this->output.settings.samples;
	tms_wave &outwave = this->output.outwave;
	int bits = this->output.settings.bitsPerSample;
	//int vol_max = 0;
	unsigned int *bits_outwave = new unsigned int[samples];

	unsigned int highbits_wave;
	unsigned int *highbits_outwave = new unsigned int[samples];
	unsigned int lowbits_wave;
	unsigned int *lowbits_outwave = new unsigned int[samples];

	for (; sample_No < samples; ++sample_No)
	{
		highbits_outwave[sample_No] = 0;
		lowbits_outwave[sample_No] = 0;
		for (wave_No = 0; wave_No < this->output.numberOfWaves; ++wave_No)
		{
			highbits_outwave[sample_No] += (*(unsigned int*)(this->output.waves[wave_No].samples + sample_No) >> bits); //左声道
			lowbits_outwave[sample_No] += (*(unsigned int*)(this->output.waves[wave_No].samples + sample_No + bits) >> bits); //右声道
		}
		//if (vol_max < highbits_outwave[sample_No]) vol_max = highbits_outwave[sample_No];
		//if (vol_max < lowbits_outwave[sample_No]) vol_max = lowbits_outwave[sample_No];
	}
	for (; sample_No < samples; ++sample_No)
	{
		highbits_outwave[sample_No] = (unsigned int)((double)highbits_outwave[sample_No] / (double)this->output.numberOfWaves);
		lowbits_outwave[sample_No] = (unsigned int)((double)lowbits_outwave[sample_No] / (double)this->output.numberOfWaves);
		bits_outwave[sample_No] = (highbits_outwave[sample_No] << bits) | lowbits_outwave[sample_No];
		outwave.samples[sample_No] = *(float*)&bits_outwave[sample_No];
	}

	return true;
}

int main() {
	return 0;
}

