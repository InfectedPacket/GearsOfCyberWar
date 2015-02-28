#include <stdio.h>
#include "XorCrypt.h"

unsigned char* XorCrypt(unsigned char* CipherData, unsigned int DataLength, char* Key, unsigned int KeyLength) {
	
	if ((CipherData && DataLength > 0) &&
		(Key && KeyLength > 0)) {
		unsigned char* PlainData = new unsigned char[DataLength];
		unsigned int idx = 0;
		for (idx = 0; idx < DataLength; idx++) {
			PlainData[idx] = (CipherData[idx] ^ Key[idx%KeyLength]);
		}

		return PlainData;
	}

	return 0;
}

int Test(int argc, char* argv) {
	unsigned char TestData[] = {0x0, 'A', 'B', 'C', 'D', '1', 0xFA};
	unsigned int TestDataLen = 7;
	char Key[] = {0x41};
	unsigned int KeyLen = 1;

	printf("[*] Test Bytes: ");
	for (unsigned int i = 0; i < TestDataLen; i++) {
		printf("0x%02X ", TestData[i]);
	}
	printf("\n");

	printf("[*] Key Bytes: ");
	for (unsigned int i = 0; i < KeyLen; i++) {
		printf("0x%02X ", Key[i]);
	}
	printf("\n");
	unsigned char* CipherData = XorCrypt(TestData, TestDataLen, Key, KeyLen);

	printf("[*] Result: ");
	for (unsigned int i = 0; i < TestDataLen; i++) {
		printf("0x%02X ", CipherData[i]);
	}
	printf("\n");

	return 0;
}