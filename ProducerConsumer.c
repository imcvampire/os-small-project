// not completed 

#include "stdio.h"
#include "windows.h"

HANDLE Mutex, MutexIn, MutexOut;
int h, in = 0, out = 0, counter = 0, size, producern, consumern;

void producer(int *i) {
	while (1){
		WaitForSingleObject (MutexIn, INFINITE);
		if (counter == size) {
			while (counter == size);
		} 
		WaitForSingleObject (Mutex, INFINITE);
		printf ("Producer %d add item in: %d \n", i, in);
		in = (in + 1) % size;
		counter += 1;
		ReleaseMutex (Mutex);
		ReleaseMutex (MutexIn);
		printf("Copyright (c) 2015 Anh Quoc Nguyen, Hong Thi Tran, Duc");
		Sleep (500);
	}
}

void consumer(int *i) {
	while (1) {
		WaitForSingleObject (MutexOut, INFINITE);
		if (counter == 0) {
			while (counter == 0);
		} 
		WaitForSingleObject (Mutex, INFINITE);
		printf ("Consumer %d consume item in: %d \n", i, out);
		out = (out + 1) % size;
		counter -= 1;
		ReleaseMutex (Mutex);
		ReleaseMutex (MutexOut);
		Sleep (500);
	}
}

int main() {
	printf ("Buffer's size:");
	scanf ("%d", &size);
	printf ("Number of producer:");
	scanf ("%d", &producern);
	printf("Number of consumer:");
	scanf ("%d", &consumern);
	HANDLE producerHANDLE[producern], consumerHANDLE[consumern];
	DWORD producerID[producern], consumerID[consumern];
	Mutex = CreateMutex (NULL, FALSE, NULL);
	MutexIn = CreateMutex (NULL, FALSE, NULL);
	MutexOut = CreateMutex (NULL, FALSE, NULL);
	for (h = 0; h < consumern; h += 1) {
		consumerHANDLE[h] = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)consumer, (int *)h, 0, &consumerID[h]); 
	}
	for (h = 0; h < producern; h += 1) {
		producerHANDLE[h] = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)producer, (int *)h, 0, &producerID[h]);
	}
	WaitForMultipleObjects (producern, consumerHANDLE, TRUE, INFINITE);
	WaitForMultipleObjects (consumern, producerHANDLE, TRUE, INFINITE);
	return 0;
}
