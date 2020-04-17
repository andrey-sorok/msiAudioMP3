#include "ClsAudio.h"

//_______________

vector<string> ClsAudio::getFileName(string Dir)
{
	vector<string> rtn;

	HANDLE fh;
	WIN32_FIND_DATA fdata;

	wstring wDir(Dir.begin(), Dir.end());

	wDir += L"\\*.*";

	fh = FindFirstFile(wDir.c_str(), &fdata);

	if (fh != INVALID_HANDLE_VALUE)
	{
		
		while (FindNextFile(fh, &fdata) != 0)
		{		
			wstring ws = fdata.cFileName;
			wstring wtmp = ws.substr(ws.find(L'.') + 1, ws.length());
			if (wtmp == L"mp3")
			{
				string s(ws.begin(), ws.end());
				s = Dir + "\\" + s;
				rtn.push_back(s);
			}			
		}
	}

	return rtn;
}

void ClsAudio::outFiles(vector<string> vFiles)
{
	for (int i = 0; i < vFiles.size(); ++i)
	{
		cout << i << ": " << vFiles[i] << endl;
	}
}

string ClsAudio::getNumTrack(string str, int pos)
{
	string tmp = str.substr(pos +1, str.length());
	tmp = tmp.substr(0, tmp.find(" "));
	int index = atoi(tmp.c_str());

	str = str.substr(0, pos) +" " + fNames[index];
	return str;
}

void ClsAudio::setCom(string str, string & alias)
{
	int pos = str.find(" ");
	string com = str.substr(0, pos);
	if (com == "open")
	{
		if (alias != "")
			Close(alias);
		
		str = getNumTrack(str, pos);
		alias = str.substr(str.length() - 3, str.length());

		Open(str, alias);
		cout << getMusicLength(alias) << " min" << endl;
		Play(alias);
	}

	if (com == "close")
		Close(alias);

	if (com == "play")
		Play(alias);

	if (com == "pause")
		Pause(alias);

	if (com == "stop")
		Stop(alias);

	if (com == "replay")
		Replay(alias);

	if (com == "length")
		cout << getMusicLength(alias) << endl;

	if (com == "time")
		timeFormat(alias);

	if (com == "pos")
		cout << curPosition(alias) << endl;

	if (com == "ff")
	{
		str = str.substr(str.find(" ") + 1, str.length());
		increasPosition(alias, atoi(str.c_str()));
	}

	if (com == "rw")
	{
		str = str.substr(str.find(" ") + 1, str.length() -1);
		decreasPosition(alias, atoi(str.c_str()));
	}

}

DWORD ClsAudio::Open(string str, string & alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw(str += " type mpegvideo alias " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Close(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("close " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Play(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("play " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Stop(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("stop " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Resume(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("resume " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Pause(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("pause " + alias);
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::toBegin(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("seek " + alias + " to start");
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::toEnd(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("seek " + alias + " to end");
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

DWORD ClsAudio::Replay(string alias)
{
	Stop(alias);
	toBegin(alias);	
	return Play(alias);
}

string ClsAudio::getMusicLength(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("status " + alias + " length");
	
	TCHAR buf[256];
	mciSendString(lcom.c_str(), buf, sizeof(buf), NULL);

	int s = _ttol(buf);

	string rtn = msToMin(s);
	return rtn;
}

DWORD ClsAudio::timeFormat(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("set " + alias + " time format milliseconds");
	return mciSendString(lcom.c_str(), NULL, 0, NULL);
}

int ClsAudio::getPosition(string alias)
{
	wstring lcom = L"";
	lcom = StrToLpcw("status " + alias + " position");
	TCHAR buf[256];
	mciSendString(lcom.c_str(), buf, sizeof(buf), NULL);

	return _ttol(buf);
}

void ClsAudio::setPosition(string alias, int pos)
{
	if (pos < 0)
		pos = 0;
	
	wstring lcom = L"";
	lcom = StrToLpcw("play " + alias + " from " + to_string(pos));
	mciSendString(lcom.c_str(), NULL, 0, NULL);
}

string ClsAudio::curPosition(string alias)
{
	int pos = getPosition(alias);
	string rtn = msToMin(pos);
	return rtn;
}

void ClsAudio::increasPosition(string alias, int pos)
{
	int position = getPosition(alias);
	setPosition(alias, position + (pos * 1000));
}

void ClsAudio::decreasPosition(string alias, int pos)
{
	int position = getPosition(alias);
	setPosition(alias, position - (pos * 1000));
}

string ClsAudio::msToMin(float f)
{
	int tmp = f / 60000 * 100;
	string str = "";
	if (tmp >= 100)
	{
		str = to_string(tmp);
		str = str.substr(0, str.length() - 2) + "." + str.substr(str.length() - 2, str.length());
	}
	else
	{
		if (tmp >= 60)
		{
			tmp = tmp - 60;
			str = "1." + to_string(tmp);		 
		}
		else
		{
			str = to_string(tmp);
		}
	}

	return str;
}

wstring ClsAudio::StrToLpcw(string str)
{
	wstring  wcom = wstring(str.begin(), str.end());
	return wcom;
}

ClsAudio::ClsAudio()
{
	musicPath = "f:\\music";
}

void ClsAudio::comProcessing()
{
	setlocale(0, "rus");
	fNames = getFileName(musicPath);
	outFiles(fNames);

	string alias = "";

	string str = "";
	while (true)
	{
		getline(cin, str);
		if (str == "x")
			break;

		setCom(str += " ", alias);
	}
}

ClsAudio::~ClsAudio()
{
}

/*
void ClsAudio::getThreadCom(string alias)
{
	string str("");
	cin >> str;

	wstring lcom = L"";
	int isCom = checkCom(str);
	if (isCom == 1)
		setCom(str, alias);

	mciSendString(lcom.c_str(), NULL, 0, NULL);

}
*/
