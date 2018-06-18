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
#include "cJSON.h"

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

/*�ඨ��*/
class instrumentIndex
{
public:
	bool fail();//����!state
	instrumentIndex(const char* fileName);//���캯��������Ϊindex��json�ļ�·��
	instrumentIndex(const instrumentIndex&) = delete;
	instrumentIndex& operator=(const instrumentIndex&) = delete;
	~instrumentIndex();//��������
	const char* getInstrument(const char* instrumentName);//����ָ��������·���ַ��������getInstrumentʧ�ܷ���nullptr
private:
	bool state;//JSON����״̬������ļ���ʧ�ܻ�򿪳ɹ�������ʧ�ܴ�ֵ��Ϊfalse,����Ϊtrue
	cJSON* Index;//����json�ļ��Ķ���ָ��
};

/*ȫ�ֺ�������*/
bool cJsonMusic(tms_input*& music, const char* fileName);
