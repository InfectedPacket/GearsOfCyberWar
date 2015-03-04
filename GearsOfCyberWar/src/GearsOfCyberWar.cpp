
#include <stdio.h>

#include "Base64.h"
#include "XorCrypt.h"
#include "GetProcessList.h"
#include "TakeScreenshot.h"

int main (int argc, char* argv) {
	printf("GearsOfCyberWar Copyright (C) 2015 Jonathan Racicot.\n");
    printf("This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
    printf("This is free software, and you are welcome to redistribute it\n");
    printf("under certain conditions; type `show c' for details.\n");
	printf("\n");
	
	int result = 0;
	printf("[*] Testing Base64 Encoder/Decoder...\n");
	result = TestBase64();
	if (result==0) {
		printf("[+] Result: [SUCCESS]\n");
	} else {
		printf("[-] Result: [FAIL]\n");
	}
	printf("[*] Testing GetProcessList function...\n");
	TestProcessList();
	printf("[*] TEsting TakeScreenshot function...\n");
	TestScreenshot();

	return 0;
}