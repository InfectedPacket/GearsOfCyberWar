
#include <stdio.h>
#include <Windows.h>
#include "Base64.h"

const static char* RegularAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

/**
<summary></summary>
<param name="PlainData"></param>
<param name="DataLength"></param>
<param name="Alphabet"></param>
<param name="PaddingChar"></param>
<returns></returns>
*/
char* Base64Encode(const unsigned char* PlainData, unsigned int DataLength, const char* Alphabet, unsigned char PaddingChar) {
    
    int ByteCounter = 0;
    int ByteIndex = 0;
    int PaddingLen = (((DataLength%3)&1)<<1)+(((DataLength%3)&2)>>1);
    unsigned int EncodedDataLength = 4*(DataLength+PaddingLen)/3;
    char* EncodedData = new char[EncodedDataLength+1];

    if (!EncodedData) {
        return 0;
    }

    unsigned char* TempByteArray3 = new unsigned char[3];
    for(ByteIndex = 0; ByteIndex <= DataLength-3; ByteIndex += 3) {
        TempByteArray3[0] = PlainData[ByteIndex];
        TempByteArray3[1] = PlainData[ByteIndex+1];
        TempByteArray3[2] = PlainData[ByteIndex+2];

        EncodedData[ByteCounter++] = Alphabet[TempByteArray3[0] >> 2];
        EncodedData[ByteCounter++] = Alphabet[((0x03&TempByteArray3[0])<<4) + (TempByteArray3[1] >> 4)];
        EncodedData[ByteCounter++] = Alphabet[((0x0F&TempByteArray3[1])<<2) + (TempByteArray3[2] >> 6)];
        EncodedData[ByteCounter++] = Alphabet[(0x3F&TempByteArray3[2])];
    }

    if (PaddingLen == 2)
    {
        EncodedData[ByteCounter++] = Alphabet[PlainData[ByteIndex] >> 2];
        EncodedData[ByteCounter++] = Alphabet[(0x3F&PlainData[ByteIndex]) << 4];
        EncodedData[ByteCounter++] = PaddingChar;
        EncodedData[ByteCounter++] = PaddingChar;
    } else if (PaddingLen == 1) {
        EncodedData[ByteCounter++] = Alphabet[PlainData[ByteIndex] >> 2];
        EncodedData[ByteCounter++] = Alphabet[((0x03&PlainData[ByteIndex])<<4) + (PlainData[ByteIndex+1] >> 4)];
        EncodedData[ByteCounter++] = Alphabet[(0x0F&PlainData[ByteIndex+1]) << 2];
        EncodedData[ByteCounter++] = PaddingChar;
    }
    
    EncodedData[ByteCounter] = 0;
    return EncodedData;
}

//TODO: 2 last characters truncated.
char* Base64Decode(const unsigned char* EncodedData, unsigned int EncodedDataLength, const char* Alphabet, const char PaddingChar) {
	char* PlainData = 0;
	unsigned int PlainDataLen = 0;
	int i = 0;
	int j = 0;
	int ByteCounter = 0;

	if (EncodedData != 0 && EncodedDataLength >= 2) {
		int ByteIndex = 0;
		unsigned int PaddingLen = 0;
		if (EncodedData[EncodedDataLength-1] == PaddingChar) PaddingLen++;
		if (EncodedData[EncodedDataLength-2] == PaddingChar) PaddingLen++;
		PlainDataLen = 3*EncodedDataLength/4 - PaddingLen;
		PlainData = new char[PlainDataLen+1];
		if (!PlainData) {
			return 0;
		}

		unsigned char* TempByteArray3 = new unsigned char[3];
		unsigned char* TempByteArray4 = new unsigned char[4];

		while(EncodedDataLength-- && (EncodedData[ByteIndex]) != PaddingChar) {
			TempByteArray4[i++] = EncodedData[ByteIndex];
			ByteIndex++;

			if (i == 4) {
				for (i = 0; i < 4; i++) {
					TempByteArray4[i] = strchr(Alphabet, TempByteArray4[i])-Alphabet;
				}

				TempByteArray3[0] = (TempByteArray4[0] << 2) + ((TempByteArray4[1] & 0x30) >> 4);
				TempByteArray3[1] = ((TempByteArray4[1] & 0xF) << 4) + ((TempByteArray4[2] & 0x3C) >> 2);
				TempByteArray3[2] = ((TempByteArray4[2] & 0x3) << 6) + TempByteArray4[3];

				for (i = 0; (i < 3); i++) {
					PlainData[ByteCounter++] = TempByteArray3[i];
				}
				i = 0;
			} // end if
		} // end while

		if (i) {
			for (j = i; j <4; j++)
			  TempByteArray4[j] = 0;

			for (j = 0; j <4; j++) {
			  TempByteArray4[j] = strchr(Alphabet, TempByteArray4[j])-Alphabet;
			}

			TempByteArray3[0] = (TempByteArray4[0] << 2) + ((TempByteArray4[1] & 0x30) >> 4);
			TempByteArray3[1] = ((TempByteArray4[1] & 0xf) << 4) + ((TempByteArray4[2] & 0x3c) >> 2);
			TempByteArray3[2] = ((TempByteArray4[2] & 0x3) << 6) + TempByteArray4[3];

			for (j = 0; (j < i - 1); j++) {
				PlainData[ByteCounter++] = TempByteArray3[j];
			}
		  }
	}

	PlainData[PlainDataLen] = 0;
	return PlainData;
}

int main(int argc, char* argv) {
    const char* TestBase64Encode = "Base64 Encoding Test";
    const char* TestBase64Decode = "QmFzZTY0IEVuY29kaW5nIFRlc3Q=";

    printf("[*] Plain string: ");
    printf(TestBase64Encode);
    printf("\n");
    char* Base64EncodeResult = Base64Encode((const unsigned char*)TestBase64Encode, strlen(TestBase64Encode), RegularAlphabet, '=');
    printf("[*] Encoded string :");
    printf(Base64EncodeResult);
	printf("\n");
	char * Base64DecodeResult = Base64Decode((const unsigned char*)Base64EncodeResult, strlen(Base64EncodeResult), RegularAlphabet, '=');
    printf("[*] Reverted plain string :");
    printf(Base64DecodeResult);
	printf("\n");

    return 0;
}

