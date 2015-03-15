#include <Windows.h>
#include <stdio.h>

int CreateEmptyFile(wchar_t* Filename, unsigned short FilenameSize) {
	HANDLE hFile = CreateFile(
			Filename,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}
	CloseHandle(hFile);
	return 0;
}

/**

<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/aa365747%28v=vs.85%29.aspx">
WriteFile function</see>
*/
int WriteDataToFile(wchar_t* Filename, 
	unsigned short FilenameSize, unsigned char* Bytes, unsigned int BytesSize) {

	HANDLE hFile = CreateFile(
			Filename,
			GENERIC_READ | GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}
	DWORD BytesWrittenSize = 0;
	BOOL WriteSuccess = WriteFile(
		hFile,
		Bytes,
		BytesSize,
		&BytesWrittenSize,
		NULL);

	if (WriteSuccess == FALSE) {
		CloseHandle(hFile);
		return -1;
	}

	CloseHandle(hFile);
	return 0;
}

/**

*/
bool FileExists(const wchar_t* Filename)
{
    DWORD       fileAttr;

    fileAttr = GetFileAttributes(Filename);
    if (0xFFFFFFFF == fileAttr) {
        return false;
	}
    return true;
}

int TestFileIO() {
	wchar_t* TestPath = L"C:\\tmp\\";
	wchar_t* TestFile = L"text.txt";
	wchar_t* TestFilename = L"C:\\tmp\\text.txt";
	wchar_t* TestData = L"Hello World!";
	int result = 0;
	wprintf(L"[*] Checking if file %s exists...", TestFile);
	if (FileExists(TestFilename)) {
		wprintf(L"[FOUND]\n");
		wprintf(L"[!] Deleting %s.\n", TestFilename);
		DeleteFile(TestFilename);
	} else {
		wprintf(L"[NOT FOUND]\n");
	}

	wprintf(L"[*] Creating file %s...\n", TestFile);
	result = CreateEmptyFile(TestFilename, wcslen(TestFilename));
	if (result == 0) {
		wprintf(L"[+] %s%s created.\n", TestPath, TestFile);
	} else {
		wprintf(L"[-] Fail to create %s%s...\n", TestPath, TestFile);
		return -1;
	}

	wprintf(L"[*] Writing %d byte(s) to %s...\n", 2*wcslen(TestData), TestFile);
	result = WriteDataToFile(TestFilename, wcslen(TestFilename), 
		(unsigned char*)TestData, wcslen(TestData)*2);
	if (result == 0) {
		wprintf(L"[+] Wrote %d byte(s) to %s...\n", 2*wcslen(TestData), TestFile);
	} else {
		DWORD LastError = GetLastError();
		wprintf(L"[-] Failed to write to %s.(ERR_CODE: %02x)...\n", TestFile, LastError);
		return -1;
	}
	return 0;
}