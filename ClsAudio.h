#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <control.h>
#include <conio.h>
#include <thread>
#include <vector>
#include <tchar.h>

#include <MMSystem.h>
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

using namespace std;

class ClsAudio
{
private:

	string musicPath = "";
	vector<string> fNames;

	vector<string> getFileName(string Dir);
	void outFiles(vector<string> vFiles);

	string getNumTrack(string str, int pos);
	void setCom(string str, string & alias);
	
	//Command Proc
	DWORD Open(string str, string & alias);
	DWORD Close(string alias);
	DWORD Play(string alias);
	DWORD Stop(string alias);
	DWORD Resume(string alias);
	DWORD Pause(string alias);
	DWORD toBegin(string alias);
	DWORD toEnd(string alias);
	DWORD Replay(string alias);
	string getMusicLength(string alias);
	DWORD timeFormat(string alias);
	int getPosition(string alias);
	void setPosition(string alias, int pos);
	
	string curPosition(string alias);
	void increasPosition(string alias, int pos);
	void decreasPosition(string alias, int pos);

	//end_Command Proc

	string msToMin(float ws);

	wstring StrToLpcw(string str);

public:
	ClsAudio();

	void comProcessing();

	~ClsAudio();
};

