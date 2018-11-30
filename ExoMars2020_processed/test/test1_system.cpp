#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "lancement d'un .exe " << std::endl;
	system("set CL = /DNETWORK_PART#2");
	system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe" ..\ExoMars2020_Network\ExoMars2020_Network.vcxproj /t:Build /p:OutputPath=..\..\Executable_folder\;TargetName=..\..\Executable_folder\partie_rover2;Configuration=Debug;Platform=x86)cmd");



	return 0;
}