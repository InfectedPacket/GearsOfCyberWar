
#include <stdio.h>

/**

<summary>

<para>Linear Disassembly; iterates over a block of code in a sequential fashion, one
instruction at a time.</para>
<para>Flow Disassembly; employs an algorithm wherein it builds lists of locations
to disassemble based upon the flow instruction. It then needs to conduct multiple
passes to disassemble all paths. This usually prevent disassembling useless code.</para>

<para>When met with a conditional jump, many disassembler immediately follow the 
<i>false</i> route when disassembling the binary.</para>

The overall strategy is to add data bytes that will be translated as opcode by the
disassembler. By adding additional bytes after a jump, i.e. rogue bytes, we can 
manipulate the disassembler in many occasions.

<code>
	xor eax, eax
	jz  0x40112343+2
	jnz 0x40112343+2

loc_40112343:
0xEB 0x907611AF	call 0x907611AF
</code>

<para>In the above example, the conditional jump will go directly to byte 0x76
instead of the of the call instruction. However the disassembler will disassemble 
the byte after the false condition and translate it as a call.</para>


</summary>

*/

int Example1() {

	printf("[*] Prior to bypass jump.\n");
	//goto BYTE_BYPASS;
	char endmsg[] = "[*] After bypass jump.\n";
	//char bytes[] = {0xEB, 0x90}; //new char[2];
	//bytes[0] = 0xEB;
	//bytes[1] = 0x90;
	__asm
	{
		push eax
		//mov ecx, 58909090h
		xor  eax, eax
		jz  $+7
		push 58909090h
		ret
		xor edx, edx
		inc edx
		dec edx
	}
	
	//printf();
	return 0;
}
/*
int Example2() {

	printf("[*] Prior to bypass jump.\n");

	__asm{
		push ax;
		mov ax, 0x05eb;
		xor eax, eax;
		jz -6;
		db 0xE8;
		db 0x58;
		db 0xC3;
		db 0x90;
		db 0x90;
	}

	printf("[*] After bypass jump.\n");

	return 0;
}
*/
/*
int exmaple3() {
	__asm{
		call $+5;
		add [esp+4+var_4], 5;
		retn;
label_continue:
		push ebp;
		mov ebp, esp;
		mov eax, [ebp+8];
		imul eax, 0x2A;
		mov esp, ebp;
		pop ebp;
		retn;
	}
}
*/
/**
SEH is at FS:[0]
Need to put address of our handler to FS:[0]
Throw an exception
*/
/*
int ReplaceSEH() {

	__asm{

		mov eax, (offset ProgramExceptionHandler+1);
		add eax, 14h;
		push eax;

	}
}
*/