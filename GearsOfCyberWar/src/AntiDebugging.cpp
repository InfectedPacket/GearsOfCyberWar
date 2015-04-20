
#include <stdio.h>

bool IsBeingDebugByteSetInPEB() {
	unsigned char IsDebuggerPresent = 0;
    __asm
    {
            mov eax,dword ptr fs:[0x18]
            mov eax,dword ptr [eax+0x30]
            cmp byte ptr [eax+2],0
            je NoDebugger
            mov IsDebuggerPresent, 1
			jmp ReturnToMain
    NoDebugger:
            mov IsDebuggerPresent, 0
	ReturnToMain:
    }
	return IsDebuggerPresent;
}