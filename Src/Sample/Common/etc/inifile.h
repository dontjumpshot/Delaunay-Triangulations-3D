//
// 2016-04-26, jjuiddong
//
// *.ini ���� ������ �о���� ����� ��Ҵ�.
//
#pragma once

#include <string.h>


namespace uiutil
{
	using namespace common;
	using std::string;

	int GetProfileInt(const string &appName, const string &keyName, const int defaultValue, const string &fileName);
	float GetProfileFloat(const string &appName, const string &keyName, const float defaultValue, const string &fileName);
	string GetProfileString(const string &appName, const string &keyName, const string &defaultValue, const string &fileName);
}
