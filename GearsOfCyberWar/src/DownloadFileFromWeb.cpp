#pragma once

#include <stdio.h>
#include <Windows.h>
#include <Iepmapi.h>
#include <UrlMon.h>

#pragma comment(lib,"urlmon.lib")
#pragma comment(lib,"Iepmapi.lib")

/**

<remarks>
Require Iepmapi.h/Iepmapi.lib
</remarks>

<see cref="https://msdn.microsoft.com/en-us/library/bb250462%28v=vs.85%29.aspx">
Understanding and Working in Protected Mode Internet Explorer
</see>
*/
bool IsIERunningInProtectedMode() {
	HRESULT hResult = 0;
	BOOL bProtectedMode = FALSE;
 
	hResult = IEIsProtectedModeProcess ( &bProtectedMode );
 
	return ( SUCCEEDED(hResult) && bProtectedMode );
}

/**
<summary></summary>
<see cref="https://msdn.microsoft.com/en-us/library/bb250462%28v=vs.85%29.aspx">
Understanding and Working in Protected Mode Internet Explorer
</see>
*/
int DownloadFileViaUrlmon(const wchar_t* Url, const wchar_t* RemotePath, 
	const wchar_t* LocalFile) {

		if (Url == NULL) { return -1; }
		if (LocalFile == NULL) { return -1;}

		wchar_t* RemoteFile = new wchar_t[65535];
		SecureZeroMemory(RemoteFile, 65535);
		wcscpy_s(RemoteFile, 65535, Url);
		wcscat(RemoteFile, RemotePath);
		//COM object on Internet explorer must create file in IE template folder. Because IE protected mode.
		HRESULT result = URLDownloadToFile(NULL, RemoteFile, LocalFile, 0, NULL);

		if (result != S_OK) { return -1; }
		return 0;
}

int TestDownloadFile() {
	//http://mirror.internode.on.net/pub/test/1meg.test
	const wchar_t* Url = L"http://mirror.internode.on.net";
	const wchar_t* RemoteFile = L"/pub/test/1meg.test";
	const wchar_t* LocalFile = L"c:\\tmp\\test.bin";
	int result = 0;
	bool isProtectedMode =  IsIERunningInProtectedMode();

	if (isProtectedMode) {
		printf("[!] Internet Explorer is running in protected mode.\n");
	} else {
		printf("[+] Internet Explorer is not running in protected mode.\n");
	}
	wprintf(L"[*] Downloading %s from %s to %s.\n", RemoteFile, Url, LocalFile);
	result = DownloadFileViaUrlmon(Url, RemoteFile, LocalFile);

	if (result < 0) {
		wprintf(L"[-] Failed to download %s from %s.\n", RemoteFile, Url);
	} else {
		wprintf(L"[-] File successfuly downloaded to %s.\n", LocalFile);
	}

	return 0;
}