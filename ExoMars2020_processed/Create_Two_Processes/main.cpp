//#include <Windows.h>
//#include <iostream>
//#include <omp.h>
//
//int main(int argc, char** argv)
//{
//	HANDLE hProcess = NULL, hThread = NULL;
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//	DWORD dwProcessId = 0, dwThreadId = 0;
//
//	ZeroMemory(&si, sizeof(si));
//	ZeroMemory(&pi, sizeof(pi));
//
//	BOOL bCreateProcess = NULL;
//
//	omp_set_num_threads(2);
//	#pragma omp parallel
//	{
//		if (omp_get_thread_num() == 0) //On créé un process de l'éxécutable Mast_Network.exe
//		{
//			bCreateProcess = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Mast_Network.exe",
//				NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
//		}
//		else if (omp_get_thread_num() == 1) //On créé un process de l'éxécutable Rover_Network.exe
//		{
//			bCreateProcess = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Rover_Network.exe",
//				NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
//		}
//
//		if (bCreateProcess == FALSE)
//		{
//			std::cout << "The Creation of the process has failed & Error Num - " << GetLastError() << std::endl;
//		}
//		std::cout << "Success to create the process" << std::endl;
//		std::cout << "Process ID   ---> " << pi.dwProcessId << std::endl;
//		std::cout << "Thread ID    ---> " << pi.dwThreadId << std::endl;
//		std::cout << "GetProcessID ---> " << GetProcessId(pi.hProcess) << std::endl;
//		std::cout << "GetThreadID  ---> " << GetThreadId(pi.hThread) << std::endl;
//
//		WaitForSingleObject(pi.hProcess, INFINITE);
//
//		CloseHandle(pi.hThread);
//		CloseHandle(pi.hProcess);
//
//		system("PAUSE");
//	}
//	return 0;
//}