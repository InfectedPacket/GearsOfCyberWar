
#include <stdio.h>
#include <Windows.h>
#include "GetWindowsVersionInfo.h"


/**
<summary>Obtains information about the current Windows operating system by 
using the GetVersionEx and GetSystemMetrics Windows API functions. The results
are stored in the provided non-null parameters.</summary>
<param name="VersionMajor">Stores the major version of Windows.
This parameter cannot be NULL.</param>
<param name="VersionMinor">Stores the minor version of Windows.
This parameter cannot be NULL.</param>
<param name="ProductType">Stores the product type information. The
product type is useful to determine if the OS is a Domain Controller, 
a server or a workstation. If a NULL pointer is provided, the product type is
not returned.</param>
<param name="BuildNumber">Stores the build number of Windows if any.
If a NULL pointer is provided, the product type is
not returned.</param>
<param name="PlatformId">Stores the platform id of Windows if any.
If a NULL pointer is provided, the platform id is not returned.</param>
<param name="ServicePackMajor">Stores the major version of the service pack if any.
If a NULL pointer is provided, the platform id is not returned.</param>
<param name="ServicePackMinor">Stores the minor version of the service pack if any.
If a NULL pointer is provided, the platform id is not returned.</param>
<param name="Suite">Stores information about the product suite if any.
If a NULL pointer is provided, the platform id is not returned.</param>
<param name="ServerBuild">Stores information about the build of the server if any.
If a NULL pointer is provided, the platform id is not returned.</param>
<returns>Returns 1 if the function completed successfully.</returns>
<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms724833%28v=vs.85%29.aspx">
OSVERSIONINFOEX structure</see>
<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms724451%28v=vs.85%29.aspx">
GetVersionEx function</see>
<see cref="https://msdn.microsoft.com/en-us/library/windows/desktop/ms724385%28v=vs.85%29.aspx">
GetSystemMetrics function</see>
*/
int GetWindowsVersionInfo(unsigned char* VersionMajor, 
							unsigned char* VersionMinor, 
							unsigned short* BuildNumber,
							unsigned char* PlatformId,
							unsigned char* ServicePackMajor,
							unsigned char* ServicePackMinor,
							unsigned short* Suite,
							unsigned char* ProductType,
							unsigned char* ServerBuild) {
//Structure holding the information about
//the operating system.
OSVERSIONINFOEX VersionInformation;

//Variable to hold the return value of
//function calls
BOOL result = false;

SecureZeroMemory(&VersionInformation, sizeof(OSVERSIONINFOEX));
VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

// Call to Windows API function
result = GetVersionEx((LPOSVERSIONINFO)&VersionInformation);

if (result == 0) {
    return 0xFFFF;
}

//Retrieve basic version information of Windows
*VersionMajor		= (unsigned char)VersionInformation.dwMajorVersion;
*VersionMinor		= (unsigned char)VersionInformation.dwMinorVersion;

//If requested, return the build number
if(BuildNumber) {
	*BuildNumber		= (unsigned short)VersionInformation.dwBuildNumber;
}

//If requested, return the platform id
if (PlatformId) {
	*PlatformId			= (unsigned char)VersionInformation.dwPlatformId;
}

//If requested, return the major version of the service pack
if (ServicePackMajor) {
	*ServicePackMajor   = (unsigned char)VersionInformation.wServicePackMajor;
}

//If requested, return the minor version of the service pack
if (ServicePackMinor) {
	*ServicePackMinor   = (unsigned char)VersionInformation.wServicePackMinor;
}

//If requested, return the product suite of the service pack
if (Suite) {
	*Suite				= (unsigned short) VersionInformation.wSuiteMask;
}

if (ProductType) {
	*ProductType		= (unsigned char)VersionInformation.wProductType;
}

if (ServerBuild) {
	*ServerBuild = GetSystemMetrics(SM_SERVERR2);
}

//Return SUCCESS
return 0x0001;
} // end function

/**
<summary></summary>
<returns>A char array containing a human readable description of the
operating system based on the given parameters.</returns>
*/
char* GetWindowsVersionDesc(unsigned char* VersionMajor, 
							unsigned char* VersionMinor, 
							unsigned short* BuildNumber,
							unsigned char* PlatformId,
							unsigned char* ServicePackMajor,
							unsigned char* ServicePackMinor,
							unsigned short* Suite,
							unsigned char* ProductType,
							unsigned char* ServerBuild) {
unsigned char BufferSize = 128;
char* WindowsDescription = new char[BufferSize];
SecureZeroMemory(WindowsDescription, BufferSize);

//We assume that at this point, we're on a Windows machine.
strcpy_s(WindowsDescription, BufferSize, "Windows");

//If nothing was requested, return.
if (VersionMajor == NULL || VersionMinor == NULL) {
	return WindowsDescription;
}

//Start by describing the main Windows version
//Windows XP, Server 2012, Windows 8....
if (*VersionMajor == 6) {
	switch (*VersionMinor) {
	case 4:
		strcat_s(WindowsDescription, BufferSize, " 10");
		break;
	case 3:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat_s(WindowsDescription, BufferSize, "  8.1");
			} else {
				strcat_s(WindowsDescription, BufferSize, "  Server 2012 R2");
			}
		} else {
			strcat_s(WindowsDescription, BufferSize, " 8.1 or Server 2012 R2");
		}
		break;
	case 2:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat_s(WindowsDescription, BufferSize, " 8");
			} else {
				strcat_s(WindowsDescription, BufferSize, " Server 2012");
			}
		} else {
			strcat_s(WindowsDescription, BufferSize, " 8 or Server 2012");
		}
		break;
	case 1:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat_s(WindowsDescription, BufferSize, " 7");
			} else {
				strcat_s(WindowsDescription, BufferSize, " Server 2008 R2");
			}
		} else {
			strcat_s(WindowsDescription, BufferSize, " 7 or Server 2008 R2");
		}
		break;
	case 0:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat_s(WindowsDescription, BufferSize, " Vista");
			} else {
				strcat_s(WindowsDescription, BufferSize, " Server 2008");
			}
		} else {
			strcat_s(WindowsDescription, BufferSize, " Vista or Server 2008");
		}
		break;
	default:
		break;
	}
} else if (*VersionMajor == 5) {
	switch (*VersionMinor) {
	case 2:
		if (*ServerBuild != 0) {
			strcat_s(WindowsDescription, BufferSize, " Server 2003 R2");
		} else if (*ServerBuild == 0) {
			strcat_s(WindowsDescription, BufferSize, " Server 2003");
		} else if (*Suite & 0x00008000) {
			strcat_s(WindowsDescription, BufferSize, " Home Server");
		} else {
			strcat_s(WindowsDescription, BufferSize, " XP Professional (x64)");
		}
		break;
	case 1:
		strcat_s(WindowsDescription, BufferSize, " XP");
		break;
	case 0:
		strcat_s(WindowsDescription, BufferSize, " 2000");
		break;
	default:
		break;
	}
} else if (*VersionMajor == 4) {
	switch (*VersionMinor) {
	case 90:
		strcat_s(WindowsDescription, BufferSize, " Millenium");
		break;
	case 10:
		strcat_s(WindowsDescription, BufferSize, " 98");
		break;
	case 0:
		strcat_s(WindowsDescription, BufferSize, " 95");
		break;
	}
} else if (*VersionMajor == 3) {
	//Wow if we ever get here, we're at the zombie apocalypse...
	strcat_s(WindowsDescription, BufferSize, " 3.1 or inferior.");
} else {
	return WindowsDescription;
}

//If there is a suite, add it after the version
//of Windows
if (Suite) {
	switch (*Suite) {
	case 0x001:
		strcat_s(WindowsDescription, BufferSize, " Small Business");
		break;
	case 0x002:
		strcat_s(WindowsDescription, BufferSize, " Enterprise");
		break;
	case 0x004:
		strcat_s(WindowsDescription, BufferSize, " BackOffice");
		break;
	case 0x008:
		strcat_s(WindowsDescription, BufferSize, " Communications");
		break;
	case 0x010:
		strcat_s(WindowsDescription, BufferSize, " Terminal");
		break;
	case 0x020:
		strcat_s(WindowsDescription, BufferSize, " Small Business (Restricted)");
		break;
	case 0x040:
		strcat_s(WindowsDescription, BufferSize, " EmbeddedNT");
		break;
	case 0x080:
		strcat_s(WindowsDescription, BufferSize, " Data Center");
		break;
	case 0x100:
		strcat_s(WindowsDescription, BufferSize, " Single User");
		break;
	case 0x200:
		strcat_s(WindowsDescription, BufferSize, " Personal");
		break;
	case 0x400:
		strcat_s(WindowsDescription, BufferSize, " Web Edition");
		break;
	case 0x800:
		strcat_s(WindowsDescription, BufferSize, " Embedded (Restricted)");
		break;
	default:
		break;
	}
}


//Continue by appending the service pack information.
if (ServicePackMajor) {
	strcat_s(WindowsDescription, BufferSize, " Service Pack ");
	char* lpServPack = new char[2];
	_itoa_s(*ServicePackMajor, lpServPack, 2, 10);
	strcat_s(WindowsDescription, BufferSize, lpServPack);
	if (ServicePackMinor && *ServicePackMinor != 0) {
		_itoa_s(*ServicePackMinor, lpServPack, 2, 10);
		strcat_s(WindowsDescription, BufferSize, ".");
		strcat_s(WindowsDescription, BufferSize, lpServPack);
	}
	SecureZeroMemory(lpServPack, sizeof(lpServPack));
//	delete lpServPack;
}

// Add the build number to the description.
if (BuildNumber) {
	char *lpBuild = new char[5];
	_itoa_s(*BuildNumber, lpBuild, 5, 10);
	strcat_s(WindowsDescription, BufferSize, " Build ");
	strcat_s(WindowsDescription, BufferSize, lpBuild);
	SecureZeroMemory(lpBuild, sizeof(lpBuild));
	//delete lpBuild;
}
return WindowsDescription;
} // end function

int Test(int argc, char* argv) {
	unsigned char* VersionMajor = new unsigned char[1]; 
	unsigned char* VersionMinor = new unsigned char[1]; 
	unsigned short* BuildNumber = new unsigned short[1];
	unsigned char* PlatformId  = new unsigned char[1];
	unsigned char* ServicePackMajor = new unsigned char[1];
	unsigned char* ServicePackMinor = new unsigned char[1];
	unsigned short* Suite = new unsigned short[1];
	unsigned char* ProductType = new unsigned char[1];
	unsigned char* ServerBuild = new unsigned char[1];
	
	int result = 0;
	char* WindowsDescription = NULL;

	result = GetWindowsVersionInfo(VersionMajor, 
									VersionMinor, 
									BuildNumber,
									PlatformId,
									ServicePackMajor,
									ServicePackMinor,
									Suite,
									ProductType, 
									ServerBuild);

	if (result) {
		WindowsDescription = GetWindowsVersionDesc(VersionMajor, 
									VersionMinor, 
									BuildNumber,
									PlatformId,
									ServicePackMajor,
									ServicePackMinor,
									Suite,
									ProductType,
									ServerBuild);
		if (WindowsDescription) {
			printf(WindowsDescription);
			printf("\n");
		} else {
			printf("[-] Error while retrieving operating system version.\n");
		}
	}
	
	delete VersionMajor; 
	delete VersionMinor; 
	delete BuildNumber;
	delete PlatformId;
	delete ServicePackMajor;
	delete ServicePackMinor;
	delete Suite;
	delete ProductType;
	delete ServerBuild;
	SecureZeroMemory(WindowsDescription, sizeof(WindowsDescription));
	delete WindowsDescription;
	return 0;
}