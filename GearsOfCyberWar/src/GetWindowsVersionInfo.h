

#define		SUCCESS					0x1
#define		ERROR_UNEXPECTED_VALUE	0x10
#define		ERROR_NULL_VALUE		0x11
#define		ERROR_WINDOWS_API		0x20

int GetWindowsVersionInfo(unsigned char* VersionMajor, 
							unsigned char* VersionMinor, 
							unsigned short* BuildNumber,
							unsigned char* PlatformId,
							unsigned char* ServicePackMajor,
							unsigned char* ServicePackMinor,
							unsigned short* Suite,
							unsigned char* ProductType,
							unsigned char* ServerBuild);

char* GetWindowsVersionDesc(unsigned char* VersionMajor, 
							unsigned char* VersionMinor, 
							unsigned short* BuildNumber,
							unsigned char* PlatformId,
							unsigned char* ServicePackMajor,
							unsigned char* ServicePackMinor,
							unsigned short* Suite,
							unsigned char* ProductType,
							unsigned char* ServerBuild);
