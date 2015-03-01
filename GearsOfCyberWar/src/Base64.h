// <copyright file="Base64.h" company="Jonathan Racicot">
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

int TestBase64();

char* Base64Encode(const unsigned char* PlainData, unsigned int DataLength, const char* Alphabet, unsigned char PaddingChar);

char* Base64Decode(const unsigned char* EncodedData, unsigned int EncodedDataLength, const char* Alphabet, const char PaddingChar);
