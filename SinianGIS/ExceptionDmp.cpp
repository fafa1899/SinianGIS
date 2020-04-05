#include "ExceptionDmp.h"

#include <tchar.h>
#include <Windows.h>
#include <DbgHelp.h>

#include <QMessageBox>

#pragma comment( lib, "Dbghelp.lib" )

static void GetAppDir(TCHAR * dmp_path, size_t num)
{
	TCHAR AppPath[MAX_PATH];
    ::GetModuleFileName(nullptr, AppPath, _countof(AppPath));
	//_tprintf(_T("%s\n"), AppPath);

	TCHAR drive[_MAX_DRIVE] = { 0 };
	TCHAR dir[_MAX_DIR] = { 0 };
	TCHAR fname[_MAX_FNAME] = { 0 };
	TCHAR ext[_MAX_EXT] = { 0 };
    _tsplitpath_s(AppPath, drive, dir, fname, ext);
	//_tprintf(_T("drive: %s\n"), drive);
	//_tprintf(_T("dir: %s\n"), dir);
	//_tprintf(_T("fname: %s\n"), fname);
	//_tprintf(_T("ext: %s\n"), ext);

	_stprintf_s(dmp_path, num, _T("%s%s"), drive, dir);
}

static LONG WINAPI lpTopLevelExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
	TCHAR dmp_path[MAX_PATH];
	//::GetTempPath(_countof(dmp_path), dmp_path);
	GetAppDir(dmp_path, MAX_PATH);
    //_tprintf(_T("%s\n"), dmp_path);

	SYSTEMTIME tm;
	GetLocalTime(&tm);//获取时间
	TCHAR file_name[128];
	_stprintf_s(file_name, L"%spbim%d-%02d-%02d-%02d-%02d-%02d.dmp", dmp_path,
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);//设置dmp文件名称
    HANDLE hFile = CreateFile(file_name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION info;//构造dmp异常数据结构
		info.ThreadId = GetCurrentThreadId();
		info.ClientPointers = FALSE;
		info.ExceptionPointers = pExInfo;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
            (MINIDUMP_TYPE)MiniDumpNormal, &info, nullptr, nullptr);//写dmp文件
		CloseHandle(hFile);

        const TCHAR *fmt =
		{
            L"遇到问题需要关闭，您的数据有可能丢失。\n\n"
            L"我们对此引起的不便表示抱歉；请将\n\n"
			L"\"%s\"\n\n"
            L"发送给我们以便快速查找问题之所在，谢谢。\n\n"
            L"邮箱：holybell44@126.com\n\n"
            L"github主页：https://github.com/fafa1899"
		};
		TCHAR msg[400];

        _stprintf(msg, fmt, file_name);

        //MessageBox(NULL, msg, L"程序异常报告", MB_ICONERROR | MB_SYSTEMMODAL);
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("程序异常报告"), QString::fromWCharArray(msg));
	}
	else
	{
		TCHAR info[300] = { L"fail to create dump file:" };
        _tcscat_s(info, file_name);
        //MessageBox(NULL, info, L"dump", MB_ICONERROR | MB_SYSTEMMODAL);
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("程序异常报告"), QString::fromWCharArray(info));
    }
	return EXCEPTION_EXECUTE_HANDLER;
}

ExceptionDmp::ExceptionDmp()
{	
    SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
}

static ExceptionDmp s_ExceptionDmp;

ExceptionDmp::~ExceptionDmp()
{
}
