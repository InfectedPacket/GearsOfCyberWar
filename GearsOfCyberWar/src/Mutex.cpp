#include <stdio.h>
#include <windows.h>

/**

<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms686927%28v=vs.85%29.aspx">
Using Mutex Objects</see>
<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms682411%28v=vs.85%29.aspx">
CreateMutex function</see>
*/
int SetupMutex(wchar_t* MutexName) {
	HANDLE hMutex = CreateMutex(
		NULL,
		FALSE,
		MutexName);

	if (hMutex == NULL) {
		return -1;
	}

	return 0;
}

/**

*/
bool MutexExists(wchar_t* MutexName) {
	HANDLE hMutex = OpenMutex(SYNCHRONIZE,
		FALSE, MutexName);
	//return (hMutex != NULL);
	if (hMutex == NULL) {
		return false;
	} else {
		return true;
	}
}

/**

<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms685066(v=vs.85).aspx">
ReleaseMutex function</see>
*/
bool DeleteMutex(wchar_t* MutexName) {
	HANDLE hMutex = OpenMutex(SYNCHRONIZE,
		FALSE, MutexName);
	return ReleaseMutex(MutexName);
}

int TestMutex() {
	bool result = false;
	wchar_t* MutexName = L"TestMutex";
	DeleteMutex(MutexName);
	wprintf(L"[*] Testing if mutex %s exists...", MutexName);
	result = MutexExists(MutexName);
	if (!result) {
		printf("[NO]\n");
	} else {
		wprintf(L"[YES]\n[!] Deleting mutex %s...\n", MutexName);
		DeleteMutex(MutexName);
	}

	wprintf(L"[*] Creating mutex %s ...", MutexName);
	SetupMutex(MutexName);
	printf("[DONE]\n");

	wprintf(L"[*] Testing if mutex %s exists...", MutexName);
	result = MutexExists(MutexName);
	if (!result) {
		printf("[NO]\n");
	} else {
		wprintf(L"[YES]\n[!] Deleting mutex %s...\n", MutexName);
		DeleteMutex(MutexName);
	}

	return 1;
}