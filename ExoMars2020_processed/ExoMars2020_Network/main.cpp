#include <iostream>
#include <systemc.h>
#include "networkunit.h"
#include "switchunit.h"

int sc_main(int argc, char *argv[])
{
	bool verbose(false);
	for (int i = 1; i < argc; i++)
		if (std::string(argv[i]) == "-v") verbose = true;
	

    NetworkUnit nu("nu", 24e6, verbose);
    sc_start(300, SC_US); // Starts simulation for 300 us

    return 0;

	// set CL = /DNETWORK_PART#2
	// msbuild Network.vcxproj /p:Configuration=Release;Platform=x86;TargetName=xxxxxxxxx
}

