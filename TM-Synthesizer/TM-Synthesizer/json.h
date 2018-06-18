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
#include "cJSON.h"

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

/*类定义*/
class instrumentIndex
{
public:
	bool fail();//返回!state
	instrumentIndex(const char* fileName);//构造函数，参数为index的json文件路径
	instrumentIndex(const instrumentIndex&) = delete;
	instrumentIndex& operator=(const instrumentIndex&) = delete;
	~instrumentIndex();//析构函数
	const char* getInstrument(const char* instrumentName);//返回指定乐器的路径字符串，如果getInstrument失败返回nullptr
private:
	bool state;//JSON解析状态，如果文件打开失败或打开成功但解析失败此值均为false,否则为true
	cJSON* Index;//整个json文件的对象指针
};

/*全局函数声明*/
bool cJsonMusic(tms_input*& music, const char* fileName);
