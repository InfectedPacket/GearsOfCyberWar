#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

unsigned int GenerateRandomInt(unsigned int Minimum, unsigned int Maximum) {
	unsigned int RandomSleepPeriod = 0;

	if (Minimum > Maximum) {
		unsigned int t = Maximum;
		Maximum = Minimum;
		Minimum = t;
	}

	srand(time(NULL));
	RandomSleepPeriod = rand()%Maximum+Minimum;

	return RandomSleepPeriod;
}

char* GenerateRandomString(unsigned int MinimumLength, unsigned int MaximumLength, 
	const char* Alphabet, unsigned int AlphabetLength) {
		unsigned int RandomStringLength = 0;

		if (MinimumLength > MaximumLength) {
			unsigned int t = MaximumLength;
			MaximumLength = MinimumLength;
			MinimumLength = t;
		}

		srand(time(NULL));
		RandomStringLength = rand()%MaximumLength+MinimumLength;
		char* RandomString = new char[RandomStringLength];
		SecureZeroMemory(RandomString, RandomStringLength);
		unsigned int RandomCharacterIdx = 0;
		char RandomCharacter = 'a';
		for (unsigned int i = 0; i < (RandomStringLength-1); i++) {
			RandomCharacterIdx = rand()%(AlphabetLength-1);
			RandomCharacter = Alphabet[RandomCharacterIdx];
			RandomString[i] = RandomCharacter;
		}

		RandomString[RandomStringLength] = 0x0;
		return RandomString;
}

int TestRandomGenerators() {
	int result = 0;
	unsigned int SleepDuration = 0;
	printf("[*] Testing random integer generation...\n");
	SleepDuration = GenerateRandomInt(3476, 125674);
	printf("[+] Random integer generated: %dms.\n", SleepDuration);

	char* RandomString;
	char* RandomAlphabet = "ABCDEFGHIxyz02371!-+";
	printf("[*] Testing random string generation...\n");
	RandomString = GenerateRandomString(6, 12, RandomAlphabet, strlen(RandomAlphabet));
	printf("[+] Random string generated: %s.\n", RandomString);


	return 0;
}