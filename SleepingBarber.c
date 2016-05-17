// Simulating Sleeping Barber Problem in C 

#include "stdio.h"
#include "windows.h"

int i, size, number, CustomerNumber = 0, CustomerCount = 0;
HANDLE Mutex;

void Customer(int *i) {
	while (1) {
		WaitForSingleObject (Mutex, INFINITE);
		if (CustomerCount == size) {
			printf ("Customer %d comes but gets out because there is no slot\n", i + 1);
			ReleaseMutex (Mutex);
			Sleep (1000);
			continue;
		}
		printf ("Customer %d comes\n", i + 1);
		if (CustomerCount == 0) {
			printf ("Barber is waked up\n");
		}
		CustomerCount += 1;
		ReleaseMutex (Mutex);
		Sleep (500);
		WaitForSingleObject (Mutex, INFINITE);
		printf ("Customer %d outed\n", i + 1);
		CustomerCount -= 1;
		if (CustomerCount == 0) {
			printf ("Barber sleeps\n");
		}
		ReleaseMutex (Mutex);
		Sleep (1000);
	}
}

int main() {
	printf ("Number of customer:");
	scanf ("%d", &number);
	printf ("Size:");
	scanf ("%d", &size);
	HANDLE customerHANDLE[number];
	DWORD customerID[number];
	Mutex = CreateMutex(NULL, FALSE, NULL);
	for (i = 0; i < number; i += 1) {
		customerHANDLE[i] = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) Customer, (int *)i, 0, &customerID[i]); 
	}
	WaitForMultipleObjects (number, customerHANDLE, TRUE, INFINITE);
	return 0;
}
