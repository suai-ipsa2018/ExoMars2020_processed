#include <Windows.h>
#include <iostream>

int main(int argc, char** argv)
{
	HANDLE hProcess_1 = NULL, hProcess_2 = NULL, hThread_1 = NULL, hThread_2 = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwProcessId_1 = 0, dwProcessId_2 = 0, dwThreadId_1 = 0, dwThreadId_2 = 0;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	BOOL bCreateProcess_1 = NULL, bCreateProcess_2 = NULL;


	bCreateProcess_1 = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Mast_Network.exe",
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if (bCreateProcess_1 == FALSE)
	{
		std::cout << "The Creation of the process has failed & Error Num - " << GetLastError() << std::endl;
	}
	std::cout << "Success to create the process" << std::endl;
	std::cout << "Process ID   ---> " << pi.dwProcessId << std::endl;
	std::cout << "Thread ID    ---> " << pi.dwThreadId << std::endl;
	std::cout << "GetProcessID ---> " << GetProcessId(pi.hProcess) << std::endl;
	std::cout << "GetThreadID  ---> " << GetThreadId(pi.hThread) << std::endl;

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	//system("PAUSE");

	bCreateProcess_2 = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Rover_Network.exe",
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if (bCreateProcess_2 == FALSE)
	{
		std::cout << "The Creation of the process has failed & Error Num - " << GetLastError() << std::endl;
	}
	std::cout << "Success to create the process" << std::endl;
	std::cout << "Process ID   ---> " << pi.dwProcessId << std::endl;
	std::cout << "Thread ID    ---> " << pi.dwThreadId << std::endl;
	std::cout << "GetProcessID ---> " << GetProcessId(pi.hProcess) << std::endl;
	std::cout << "GetThreadID  ---> " << GetThreadId(pi.hThread) << std::endl;

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	system("PAUSE");
	return 0;
}