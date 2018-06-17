#pragma once

using namespace std;





//这里是从json中读取结构体的部分
/*宏定义*/

/*库文件引用*/
#include <fstream>  
#include <string>  
#include<cstdio>  
#include<cstdlib>  
#include<vector>
#include<iostream>

/*全局量声明*/

/*结构体定义*/
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

	int fadeout;

	int fadein;

	int rev;
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


/*全局函数声明*/
bool cJsonMusic(tms_input*& music, const char* fileName);
