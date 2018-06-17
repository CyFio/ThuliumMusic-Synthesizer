#pragma once

using namespace std;





//�����Ǵ�json�ж�ȡ�ṹ��Ĳ���
/*�궨��*/

/*���ļ�����*/
#include <fstream>  
#include <string>  
#include<cstdio>  
#include<cstdlib>  
#include<vector>
#include<iostream>

/*ȫ��������*/

/*�ṹ�嶨��*/
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


/*ȫ�ֺ�������*/
bool cJsonMusic(tms_input*& music, const char* fileName);
