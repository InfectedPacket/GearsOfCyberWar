// <copyright file="Base64.cpp" company="Jonathan Racicot">
//    C++ Base64 Encoder/Decoder.
//    Copyright (C) 2015 Jonathan Racicot
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
// </copyright>
// <author>Jonathan Racicot</author>
// <email>infectedpacket@gmail.com</email>
// <date>2015-03-01</date>
// <summary>C++ implementation of the Base64 encoder/decoder function.</summary>
#include <stdio.h>
#include <Windows.h>
#include "Base64.h"

/**
 Standard alphabet for Base64 encoding.
*/
const static char* RegularAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

/**
<summary>Encodes a given string using the standard Base64 encoding algorithm
using the specified alphabet.</summary>
<param name="PlainData">The array of bytes to encode.</param>
<param name="DataLength">The number of bytes contained in the array.</param>
<param name="Alphabet">The alphabet to used in conjunction with the base64
algorithm. </param>
<param name="PaddingChar">The padding character to used. The standard character
is the equal sign (=).</param>
<param name="EncodedDataLength">The length of the resulting byte array of encoded
data.</param>
<returns>Returns an array of encoded bytes.</returns>
*/
char* Base64Encode(const unsigned char* PlainData, unsigned int DataLength, 
	const char* Alphabet, unsigned char PaddingChar, 
	unsigned int* EncodedDataLength) {
    
    int ByteCounter = 0;
    int ByteIndex = 0;
    int PaddingLen = (((DataLength%3)&1)<<1)+(((DataLength%3)&2)>>1);
    *EncodedDataLength = 4*(DataLength+PaddingLen)/3;
    char* EncodedData = new char[*EncodedDataLength+1];

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
    } //end for

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
    } //end if
    
    EncodedData[ByteCounter] = 0;
    return EncodedData;
}

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

int TestBase64() {
    const char* TestBase64Encode = "Base64 Encoding Test";
    const char* TestBase64Decode = "QmFzZTY0IEVuY29kaW5nIFRlc3Q=";
	unsigned int EncodedDataLength = 0;
    printf("[*] Plain string: ");
    printf(TestBase64Encode);
    printf("\n");
    char* Base64EncodeResult = Base64Encode((const unsigned char*)TestBase64Encode, 
		strlen(TestBase64Encode), RegularAlphabet, '=', &EncodedDataLength);
    printf("[*] Encoded string :");
    printf(Base64EncodeResult);
	printf("\n");
	char * Base64DecodeResult = Base64Decode((const unsigned char*)Base64EncodeResult, EncodedDataLength, RegularAlphabet, '=');
    printf("[*] Reverted plain string :");
    printf(Base64DecodeResult);
	printf("\n");

    return strcmp(TestBase64Encode, Base64DecodeResult);
}

