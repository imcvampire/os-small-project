// not completed

#include "stdio.h"
#include "windows.h"

HANDLE Mutex;
int i, number, size, MaleNumber = 0, FemaleNumber = 0;

void female(int *i) {
	while (1) {
		DWORD WaitResult;
		WaitForSingleObject (Mutex, INFINITE); //
		if ((MaleNumber != 0) || (FemaleNumber == size)) {
			ReleaseMutex (Mutex);
			continue;
		}
		printf ("Female %d goes to bathroom\n", i);
		FemaleNumber += 1;
		ReleaseMutex (Mutex);
		DWORD timer = rand() % 1000;
		Sleep (timer);
		WaitForSingleObject (Mutex, INFINITE);
		printf("Female %d outed\n", i);
		FemaleNumber -= 1;
		ReleaseMutex (Mutex);
		Sleep (500);
	}
}

void male(int *i) {
	while (1) {
		DWORD WaitResult;
		WaitForSingleObject (Mutex, INFINITE);
		if ((FemaleNumber != 0) || (MaleNumber == size)) {
			ReleaseMutex (Mutex);
			continue;
		}
		printf ("Male %d goes to bathroom\n", i);
		MaleNumber += 1;
		ReleaseMutex (Mutex);
		DWORD timer = rand() % 1000;
		Sleep (timer);
		WaitForSingleObject (Mutex, INFINITE);
		printf("Male %d outed\n", i);
		MaleNumber -= 1;
		ReleaseMutex (Mutex);
		Sleep (500);
	}
}

int main() {
	printf ("Number of female or male:");
	scanf ("%d", &number);
	printf ("Bathroom's size:");
	scanf ("%d", &size);
	HANDLE femaleHANDLE[number], maleHANDLE[number];
	DWORD femaleID[number], maleID[number];
	Mutex = CreateMutex (NULL, FALSE, NULL);
    for (i = 0; i < number; i += 1) {
    	femaleHANDLE[i] = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) female, (int *)i, 0, &femaleID[i]); 
    	maleHANDLE[i] = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) male, (int *)i, 0, &maleID[i]); 
    }
    WaitForMultipleObjects (number, femaleHANDLE, TRUE, INFINITE);
    WaitForMultipleObjects (number, maleHANDLE, TRUE, INFINITE);
	return 0;
}
