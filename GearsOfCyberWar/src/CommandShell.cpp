#include <Windows.h>
#include <stdio.h>

 	HANDLE ChildStdIn_Read = NULL;
 	HANDLE ChildStdIn_Write = NULL;
 	HANDLE ChildStdOut_Read = NULL;
 	HANDLE ChildStdOut_Write = NULL;

int ConnectPipesToProcess(wchar_t* Program) {//, HANDLE OutputHandle, HANDLE ErrorHandle, HANDLE InputHandle) {
//	HANDLE g_hChildStd_IN_Rd = NULL;
//	HANDLE g_hChildStd_IN_Wr = NULL;
//	HANDLE g_hChildStd_OUT_Rd = NULL;
//	HANDLE g_hChildStd_OUT_Wr = NULL;
	BOOL IsSuccess = FALSE;


	/*
	SECURITY_ATTRIBUTES SecurityAttributes;
	SecureZeroMemory(&SecurityAttributes, sizeof(SECURITY_ATTRIBUTES));
	SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecurityAttributes.bInheritHandle = TRUE;
	SecurityAttributes.lpSecurityDescriptor = NULL;
	*/
	SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDOUT. 
	if ( ! CreatePipe(&ChildStdOut_Read, &ChildStdOut_Write, &saAttr, 0) ) {
		return -1;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if ( ! SetHandleInformation(ChildStdOut_Read, HANDLE_FLAG_INHERIT, 0) ) {
		return -1;
	}

	// Create a pipe for the child process's STDIN. 
	if ( ! CreatePipe(&ChildStdIn_Read, &ChildStdIn_Write, &saAttr, 0) ) {
		return -1;
	}

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if ( ! SetHandleInformation(ChildStdIn_Write, HANDLE_FLAG_INHERIT, 0) ) {
		return -1;
	}

	PROCESS_INFORMATION ProcessInformation; 
	SecureZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));
	
    STARTUPINFO StartInfo;
	SecureZeroMemory(&StartInfo, sizeof(STARTUPINFO	));
	StartInfo.cb = sizeof(STARTUPINFO); 
	StartInfo.hStdError		=	ChildStdOut_Read;//ErrorHandle;
	StartInfo.hStdOutput	=	ChildStdOut_Read;//OutputHandle;
	StartInfo.hStdInput		=	ChildStdIn_Write;//InputHandle;
	StartInfo.dwFlags	|= STARTF_USESTDHANDLES;
	TCHAR szCmdline[] = TEXT("C:\\Windows\\System32\\cmd.exe");
	IsSuccess = CreateProcess(
		NULL,					// application name
		szCmdline,				// command line 
		NULL,					// process security attributes 
		NULL,					// primary thread security attributes
		TRUE,					// handles are inherited 
		0,						// creation flags 
		NULL,					// use parent's environment
		NULL,					// use parent's current directory 
		&StartInfo,				// STARTUPINFO pointer 
		&ProcessInformation);	// receives PROCESS_INFORMATION

	if (!IsSuccess) {
		return -1;
	}

	CloseHandle(ProcessInformation.hProcess);
	CloseHandle(ProcessInformation.hThread);
	return 0;
}

//int CreatePipes(HANDLE ChildStdOut_Read, HANDLE ChildStdOut_Write,
//	HANDLE ChildStdIn_Read, HANDLE ChildStdIn_Write) {
int CreatePipes() {
	SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDOUT. 
	if ( ! CreatePipe(&ChildStdOut_Read, &ChildStdOut_Write, &saAttr, 0) ) {
		return -1;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if ( ! SetHandleInformation(ChildStdOut_Read, HANDLE_FLAG_INHERIT, 0) ) {
		return -1;
	}

	// Create a pipe for the child process's STDIN. 
	if ( ! CreatePipe(&ChildStdIn_Read, &ChildStdIn_Write, &saAttr, 0) ) {
		return -1;
	}

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if ( ! SetHandleInformation(ChildStdIn_Write, HANDLE_FLAG_INHERIT, 0) ) {
		return -1;
	}

	return 0;
}

int TestCommandShell() {

// 	HANDLE ChildStdIn_Read = NULL;
// 	HANDLE ChildStdIn_Write = NULL;
// 	HANDLE ChildStdOut_Read = NULL;
// 	HANDLE ChildStdOut_Write = NULL;
	wchar_t* ProcCommandPrompt = L"c:\\windows\\system32\\cmd.exe";
	int result = 0;
	printf("[*] Creating pipes...\n");
	//result = CreatePipes(ChildStdOut_Read, ChildStdOut_Write, ChildStdIn_Read, ChildStdIn_Write);
	/*
	result = CreatePipes();
	if (result != 0) {
		printf("[-] Failed to create the pipes.\n");
		return -1;
	} else {
		printf("[+] Successfully created pipes.\n");
	}
	*/
	wprintf(L"[*] Connecting pipes to %s...\n", ProcCommandPrompt);
	result = ConnectPipesToProcess(ProcCommandPrompt);//,ChildStdOut_Read,ChildStdOut_Read,ChildStdIn_Write);
	if (result != 0) {
		wprintf(L"[-] Failed to connect the pipes to %s.\n", ProcCommandPrompt);
		return -1;
	} else {
		printf("[+] Successfully created pipes.\n");
	}

	char* CmdInput = "whoami\n";
	wchar_t* CmdOutput = new wchar_t[4096];
	SecureZeroMemory(CmdOutput, 4096);
	DWORD dwRead = strlen(CmdInput);
    DWORD dwWritten = 0;
	bool IsSuccess = false;
	IsSuccess = WriteFile(ChildStdIn_Write, CmdInput, dwRead, &dwWritten, NULL);
	if (!IsSuccess) {
		wprintf(L"[-] Failed to write to the pipe.\n");
	} else {
		Sleep(1000);
	}
	dwRead = 0;
	dwWritten = 0;
	wchar_t* buffer = new wchar_t[1024];
	SecureZeroMemory(buffer, 1024);
	while (dwRead > 0) {
		IsSuccess = ReadFile( ChildStdOut_Read, buffer, 1024, &dwRead, NULL);
		wcscat_s(CmdOutput, 4096, buffer);
	}
	if (!IsSuccess || wcslen(CmdOutput) <= 0) {
		wprintf(L"[-] Failed to read from output of %s.\n", ProcCommandPrompt); 
	} else {
    	wprintf(L"[+] Read %d byte(s) from output of %s:\n\t", dwRead, ProcCommandPrompt);
		wprintf(CmdOutput);
		wprintf(L"\n");
	}

	CloseHandle(ChildStdIn_Read);
	CloseHandle(ChildStdIn_Write);
	CloseHandle(ChildStdOut_Read);
	CloseHandle(ChildStdOut_Write);
	return 0;
}