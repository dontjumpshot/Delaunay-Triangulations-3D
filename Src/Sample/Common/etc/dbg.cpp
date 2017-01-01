
#include "stdafx.h"
#include "dbg.h"

using namespace common;
using namespace dbg;

//------------------------------------------------------------------------
// ���â�� ��Ʈ���� ����Ѵ�.
//------------------------------------------------------------------------
void dbg::Print( const std::string &str)
{
	OutputDebugStringA(str.c_str());
	OutputDebugStringA("\n");
}


//------------------------------------------------------------------------
// ���â�� ��Ʈ���� ����Ѵ�. (�������� ���)
//------------------------------------------------------------------------
void dbg::Print( const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	OutputDebugStringA(textString);
	//OutputDebugStringA("\n");
}


void dbg::Log(const char* fmt, ...)
{
	char textString[256];
	ZeroMemory(textString, sizeof(textString));

	va_list args;
	va_start(args, fmt);
	vsnprintf_s(textString, sizeof(textString)-1, _TRUNCATE, fmt, args);
	va_end(args);

	std::ofstream ofs("log.txt", std::ios::app);
	if (ofs.is_open())
		ofs << textString;

// 	FILE *fp = fopen("log.txt", "a+");
// 	if (fp)
// 	{
// 		fputs(textString, fp);
// 		fclose(fp);
// 	}
}


// fileName �� ���Ͽ� �α׸� �����.
void dbg::Log2(const char *fileName, const char* fmt, ...)
{
	char textString[256] = { '\0' };
	va_list args;
	va_start(args, fmt);
	vsnprintf_s(textString, sizeof(textString), _TRUNCATE, fmt, args);
	va_end(args);

	FILE *fp = fopen(fileName, "a+");
	if (fp)
	{
		fputs(textString, fp);
		fclose(fp);
	}
}


void dbg::ErrLog(const char* fmt, ...)
{
	char textString[256] = { '\0' };
	va_list args;
	va_start(args, fmt);
	vsnprintf_s(textString, sizeof(textString), _TRUNCATE, fmt, args);
	va_end(args);

	FILE *fp = fopen("errlog.txt", "a+");
	if (fp)
	{
		fputs(textString, fp);
		fclose(fp);
	}

	// �α����Ͽ��� ���� �޼����� �����Ѵ�.
	Log( "Error : %s", textString);
}


void dbg::RemoveErrLog()
{
	FILE *fp = fopen("errlog.txt", "w");
	if (fp)
	{
		fputs("", fp);
		fclose(fp);
	}
}


void dbg::RemoveLog2(const char *fileName)
{
	FILE *fp = fopen(fileName, "w");
	if (fp)
	{
		fputs("", fp);
		fclose(fp);
	}
}


void dbg::RemoveLog()
{
	FILE *fp = fopen("log.txt", "w");
	if (fp)
	{
		fputs("", fp);
		fclose(fp);
	}
}

