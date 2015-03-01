// <copyright file="GetProcessList.cpp" company="Jonathan Racicot">
//    <one line to give the program's name and a brief idea of what it does.>
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
// <summary></summary>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

typedef void (*DoProcessAction)(PROCESSENTRY32);

BOOL GetProcessList( DoProcessAction )
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;

  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE )
  {
    //printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
    return( FALSE );
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
   // printError( TEXT("Process32First") ); // show cause of failure
    CloseHandle( hProcessSnap );          // clean the snapshot object
    return( FALSE );
  }

  do
  {
	    dwPriorityClass = 0;
		hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess == NULL ) {
		 // printError( TEXT("OpenProcess") );
		} else {
		  dwPriorityClass = GetPriorityClass( hProcess );
		  if( !dwPriorityClass ) {}
			//printError( TEXT("GetPriorityClass") );
		  CloseHandle( hProcess );
		}

		//pe32.szExeFile			Process Name
		//pe32.th32ProcessID		Process ID
		//pe32.cntThreads			Thread count
		//pe32.th32ParentProcessID	Parent process ID
		//pe32.pcPriClassBase		Priority base
		DoProcessAction(pe32);
  } while( Process32Next( hProcessSnap, &pe32 ) );

  CloseHandle( hProcessSnap );
  return TRUE;
}