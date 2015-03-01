#include <windows.h>
#include <tlhelp32.h>

typedef void (*ProcessAction)(PROCESSENTRY32);


int TestProcessList();
BOOL GetProcessList( ProcessAction DoProcessAction );