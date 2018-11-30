#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <Windows.h>

int main(int argc, const char** argv) 
{
	system("/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/MSBuild.exe ");

	unsigned num_cpus = std::thread::hardware_concurrency();
	std::cout << "Launching " << num_cpus << " threads\n";

	// A mutex ensures orderly access to std::cout from multiple threads.
	std::mutex iomutex;
	std::vector<std::thread> threads(num_cpus);
	for (unsigned i = 0; i < num_cpus; ++i) 
	{
		threads[i] = std::thread([&iomutex, i] 
		{
			{
				// Use a lexical scope and lock_guard to safely lock the mutex only for
				// the duration of std::cout usage.
				std::lock_guard<std::mutex> iolock(iomutex);
				std::cout << "Thread #" << i << " is running" << std::endl;
			}

			// Simulate important work done by the tread by sleeping for a bit...
			//std::this_thread::sleep_for(std::chrono::milliseconds(200));
			
			HANDLE hProcess_1 = NULL, hThread_1 = NULL;
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			DWORD dwProcessId_1 = 0, dwThreadId_1 = 0;

			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));

			BOOL bCreateProcess_1 = NULL;

			if (i == 0)
			{
				bCreateProcess_1 = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Mast_Network.exe",
					NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			}
			else if (i == 1)
			{
				bCreateProcess_1 = CreateProcess("D:\\IPSA_cours_ex_etc\\AERO4_SUAI\\ExoMars2020_processed\\ExoMars2020_processed\\Debug\\Rover_Network.exe",
					NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			}
			else
				std::cout << "No process to create " << std::endl;

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
		});
	}

	for (auto& t : threads) {
		t.join();
	}

	system("PAUSE");
	return 0;
}