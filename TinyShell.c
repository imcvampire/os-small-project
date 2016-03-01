#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tlhelp32.h>
#include <string.h>

char* FixDay (long day)
{
	switch (day)
	{
		case 1: return "thu hai";
		case 2: return "thu ba";
		case 3: return "thu tu";
		case 4: return "thu nam";
		case 5: return "thu sau";
		case 6: return "thu bay";
		case 7: return "chu nhat";
	}
}
void GetTime (int i)
{
	SYSTEMTIME  time;
	GetLocalTime (&time);
	if (i == 1)
		printf ("\nHom nay la: %s ngay %d thang %d nam %d", FixDay(time.wDayOfWeek), time.wDay, time.wMonth, time.wYear);
	else
		printf ("\nBay gio la: %d gio %d phut %d giay", time.wHour, time.wMinute, time.wSecond);
}

void List (void)
{
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            printf ("\n%-35s    PID:%-6d    Parent PID: %-6d",entry.szExeFile,entry.th32ProcessID,entry.th32ParentProcessID);
        }
    }
    CloseHandle(snapshot);
}

void TerminateProcessViaPID ()
{
	printf ("Nhap PID cua process can terminate:");
	long PID;
	scanf ("%li",&PID);
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if ((DWORD)entry.th32ProcessID == PID)
            {
            	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,(DWORD) entry.th32ProcessID);
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
}

void TerminateProcessViaName ()
{
	printf ("Nhap ten cua process can terminate:");
	char file[1000];
	fflush (stdin);
	gets (file);
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, file) == 0)
            {
            	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,(DWORD) entry.th32ProcessID);
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
}

void copyright() {
	printf("Copyright (c) 2015 Anh Quoc Nguyen, Hong Thi Tran, Duc");	
}

void Process ()
{
	printf ("Nhap duong dan:");
	char link[100000];
	fflush (stdin);
	gets (link);
	printf ("\nChon thoi gian cho: \n\t* Chu y: Nhap vao so nguyen duong. \n\t* -Dac biet: \n\t\t 0: mai mai hoac process tu terminate \n\t\t -1: tro lai NQAShell luon");
	printf ("\n\n\tNhap so ban muon chon:");
	int time;
	scanf ("%d",&time);
	while (!(time >= -1))
	{
		printf ("\n\t\tSo ban nhap co van de. \n\t\tNhap lai:");
		scanf ("%d",&time);
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if (CreateProcess(link,NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi) == 0)
	{
		printf ("Da xay ra loi");
		GetLastError();
		return;
	}
	else 
		printf ("Created. \nProcess link: %s  \nPID:%d", link, pi.dwProcessId);
	if (time == -1)
		printf ("\nCreated FOREGROUND Process");
	else
	{
		if (time == 0)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else
		{
			if (WaitForSingleObject(pi.hProcess, time * 1000) != WAIT_OBJECT_0)
				TerminateProcess(pi.hProcess, 0);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		printf ("\nProcess with PID: %d is terminated", pi.dwProcessId);
	}
}

void Help ()
{
	printf ("\nTinyShell ho tro cac lenh sau:");
	printf ("\n\t date: Lay ngay hien tai.");
	printf ("\n\t time: Lay gio hien tai.");	
	printf ("\n\t list: Danh sach process.");
	printf ("\n\t terminatePID: Tat process qua PID.");
	printf ("\n\t terminateName: Tat process qua ten.");
	printf ("\n\t run: Chay chuong trinh.");
	printf ("\n\t clrsrc: Xoa man hinh.");
	printf ("\n\t exit: Thoat");
}

int main()
{
	printf ("***********************************************************************"); 
	printf ("\n\t\t\t WELCOME TO TinyShell\n");
	printf ("***********************************************************************\n\n");
	// Help();
	char choice[30];
	do
	{
		printf ("\n\nTinyShell\\> "); 
		fflush (stdin);
		gets (choice);
		copyright();
		if (strcmp(choice, "date") == 0) {
			GetTime(1);
		} else if (strcmp(choice, "time") == 0) {
			GetTime(2);
		} else if (strcmp(choice, "list") == 0) {
			List();
		} else if (strcmp(choice, "terminatePID") == 0) {
			TerminateProcessViaPID();
		} else if (strcmp(choice, "terminateName") == 0) {
			TerminateProcessViaName();
		} else if (strcmp(choice, "run") == 0) {
			Process();
		} else if (strcmp(choice, "exit") == 0) {
			printf ("Goodbye from NQAShell");
			return 0;
		} else if (strcmp(choice, "help") == 0) {
			Help();
		} else if (strcmp(choice, "clrsrc") == 0) {
			system ("cls");
		} else {
			printf ("TinyShell khong ho tro lenh nay.");
		}
	} while (strcmp(choice, "exit") != 0);
}
