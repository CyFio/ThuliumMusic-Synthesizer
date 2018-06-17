#include"cJSON.h"  
#include"json.h"

using namespace std;

char* cJsonMusic_open(const char* fileName)
{
	char* str;
	unsigned int count = 0;
	char a;
	ifstream openfile(fileName);//xx代表文件位置及名称
	if (!openfile)
	{
		cout << "open failed!" << endl;
		return 0;
	}
	do {
		openfile.get(a);
		count++;
		if (openfile.eof())
			break;
	} while (!openfile.eof());
	//cout << count;
	//cout << endl;

	str = new char[count + 1];
	//	cout << openfile.tellg() << endl;
	openfile.close();
	openfile.open(fileName);
	for (int i = 0; i < count; ++i)
	{
		openfile.get(a);
		str[i] = a;
	}
	openfile.close();
	str[count] = '\0';
	//	cout << s;
	return str;
}

int music_getPitch(int k, cJSON*track)
{
	int t = 0;
	t = cJSON_GetObjectItem(track, "Pitch")->valueint;
	//cout << t<<endl;
	return t;
}
double music_duration(int k, cJSON*track)
{
	double t = 0;
	t = cJSON_GetObjectItem(track, "Duration")->valuedouble;
	//cout << t<<endl;
	return t;
}
double music_start(int k, cJSON*track)
{
	double t = 0;
	t = cJSON_GetObjectItem(track, "StartTime")->valuedouble;
	//cout << t<<endl;
	return t;
}
double music_vol(int k, cJSON*track)
{
	double t = 0;
	t = cJSON_GetObjectItem(track, "Volume")->valuedouble;
	//cout << t<<endl;
	return t;
}

int cJsonMusic_getTrackNumber(cJSON* file)
{
	return cJSON_GetArraySize(file);
}

bool cJsonMusic(tms_input*& music, const char* fileName)
{
	char* str = cJsonMusic_open(fileName);
	cJSON* file = cJSON_Parse(str);
	delete[]str;
	if (!file)return false;
	delete music;
	music = new tms_input;
	music->numberOfTracks = cJsonMusic_getTrackNumber(file);
	music->tracks = new tms_track[music->numberOfTracks];
	for (int i = 0; i < music->numberOfTracks; i++)
	{
		cJSON* current_track = cJSON_GetArrayItem(file, i);
		music->tracks[i].settings.instrument = cJSON_GetObjectItem(current_track, "Instrument")->valuestring;
		music->tracks[i].numberOfnotes = cJSON_GetArraySize(cJSON_GetObjectItem(current_track, "Content"));
		music->tracks[i].notes = new tms_note[music->tracks[i].numberOfnotes];
		cJSON*s1 = current_track->child->next->child;
		for (int j = 0; j < music->tracks[i].numberOfnotes; j++)
		{
			music->tracks[i].notes[j].pit = music_getPitch(j, s1);
			s1 = s1->next;
		}
		cJSON_Delete(s1);
		cJSON*s2 = current_track->child->next->child;
		for (int j = 0; j < music->tracks[i].numberOfnotes; j++)
		{
			music->tracks[i].notes[j].time_duration = music_duration(j, s2);
			s2 = s2->next;
		}
		cJSON_Delete(s2);
		cJSON*s3 = current_track->child->next->child;
		for (int j = 0; j < music->tracks[i].numberOfnotes; j++)
		{
			music->tracks[i].notes[j].time_start = music_start(j, s3);
			s3 = s3->next;
		}
		cJSON_Delete(s3);
		cJSON*s4 = current_track->child->next->child;
		for (int j = 0; j < music->tracks[i].numberOfnotes; j++)
		{
			music->tracks[i].notes[j].vol = music_vol(j, s4);
			s4 = s4->next;
		}
		cJSON_Delete(s4);
		music->tracks[i].settings.fadein = cJSON_GetObjectItem(current_track->child->next->next, "FadeOut")->valueint;
		music->tracks[i].settings.fadeout = cJSON_GetObjectItem(current_track->child->next->next, "FadeIn")->valueint;
		music->tracks[i].settings.rev = cJSON_GetObjectItem(current_track->child->next->next, "Rev")->valueint;
		/*	cout << music->tracks[i].settings.fadein << endl;
		cout << music->tracks[i].settings.fadeout << endl;
		cout << music->tracks[i].settings.rev << endl;
		cout << music->tracks[i].settings.instrument<<endl;
		cout << endl;*/
	}
	cJSON_Delete(file);
	return true;
}

//int main()
//{
//	tms_input* test = 0;
//	cout << cJsonMusic(test, "test.output.json");
//
//	delete test;
//	system("pause");
//	return 0;
//}