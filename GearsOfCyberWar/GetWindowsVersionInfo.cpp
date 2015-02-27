
#include <stdio.h>
#include <Windows.h>
#include "GetWindowsVersionInfo.h"


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
    return ERROR_WINDOWS_API;
}

*VersionMajor		= (unsigned char)VersionInformation.dwMajorVersion;
*VersionMinor		= (unsigned char)VersionInformation.dwMinorVersion;

if(BuildNumber) {
	*BuildNumber		= (unsigned short)VersionInformation.dwBuildNumber;
}

if (PlatformId) {
	*PlatformId			= (unsigned char)VersionInformation.dwPlatformId;
}

if (ServicePackMajor) {
	*ServicePackMajor   = (unsigned char)VersionInformation.wServicePackMajor;
}

if (ServicePackMinor) {
	*ServicePackMinor   = (unsigned char)VersionInformation.wServicePackMinor;
}

if (Suite) {
	*Suite				= (unsigned short) VersionInformation.wSuiteMask;
}

if (ProductType) {
	*ProductType		= (unsigned char)VersionInformation.wProductType;
}

if (ServerBuild) {
	*ServerBuild = GetSystemMetrics(SM_SERVERR2);
}

return SUCCESS;
} // end function

/**

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
char* WindowsDescription = new char[256];
SecureZeroMemory(WindowsDescription, 256);

strcpy(WindowsDescription, "Windows");

if (VersionMajor == NULL || VersionMinor == NULL) {
	return NULL;
}

if (*VersionMajor == 6) {
	switch (*VersionMinor) {
	case 4:
		strcat(WindowsDescription, " 10");
		break;
	case 3:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat(WindowsDescription, "  8.1");
			} else {
				strcat(WindowsDescription, "  Server 2012 R2");
			}
		} else {
			strcat(WindowsDescription, " 8.1 or Server 2012 R2");
		}
		break;
	case 2:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat(WindowsDescription, " 8");
			} else {
				strcat(WindowsDescription, " Server 2012");
			}
		} else {
			strcat(WindowsDescription, " 8 or Server 2012");
		}
		break;
	case 1:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat(WindowsDescription, " 7");
			} else {
				strcat(WindowsDescription, " Server 2008 R2");
			}
		} else {
			strcat(WindowsDescription, " 7 or Server 2008 R2");
		}
		break;
	case 0:
		if (ProductType) {
			if (*ProductType == 1) {
				strcat(WindowsDescription, " Vista");
			} else {
				strcat(WindowsDescription, " Server 2008");
			}
		} else {
			strcat(WindowsDescription, " Vista or Server 2008");
		}
		break;
	default:
		break;
	}
} else if (*VersionMajor == 5) {
	switch (*VersionMinor) {
	case 2:
		//TODO:Differ various 5.2 versions
		break;
	case 1:
		strcat(WindowsDescription, " XP");
		break;
	case 0:
		strcat(WindowsDescription, " 2000");
		break;
	default:
		break;
	}
} else if (*VersionMajor == 4) {
	switch (*VersionMinor) {
	case 90:
		strcat(WindowsDescription, " Millenium");
		break;
	case 10:
		strcat(WindowsDescription, " 98");
		break;
	case 0:
		strcat(WindowsDescription, " 95");
		break;
	}
} else if (*VersionMajor == 3) {
	//TODO:Complete lower versions
} else {
	return WindowsDescription;
}

if (ServicePackMajor) {
	strcat(WindowsDescription, " Service Pack ");
	char* lpServPack = new char[2];
	itoa(*ServicePackMajor, lpServPack, 10);
	strcat(WindowsDescription, lpServPack);
	if (ServicePackMinor && *ServicePackMinor != 0) {
		itoa(*ServicePackMinor, lpServPack, 10);
		strcat(WindowsDescription, ".");
		strcat(WindowsDescription, lpServPack);
	}
	SecureZeroMemory(lpServPack, sizeof(lpServPack));
//	delete lpServPack;
}

if (BuildNumber) {
	char *lpBuild = new char[4];
	itoa(*BuildNumber, lpBuild, 10);
	strcat(WindowsDescription, " Build ");
	strcat(WindowsDescription, lpBuild);
	SecureZeroMemory(lpBuild, sizeof(lpBuild));
	//delete lpBuild;
}
return WindowsDescription;
} // end function

int main(int argc, char* argv) {
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