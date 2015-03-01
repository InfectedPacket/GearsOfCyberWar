// <copyright file="XorCrypt.h" company="Jonathan Racicot">
//    C++ XOR Encryption/Decryption Implementation
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
// <summary>C++ implementation of the XOR encryption/decryption mechanism.</summary>

int TestXorCrypt();
unsigned char* XorCrypt(unsigned char* CipherData, unsigned int DataLength, char* Key, unsigned int KeyLength);