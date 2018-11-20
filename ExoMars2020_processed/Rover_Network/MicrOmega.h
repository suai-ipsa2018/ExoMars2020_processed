#pragma once
#include "Node.h"
class MicrOmega : public Node
{
public:
	MicrOmega(size_t logical_address, size_t psize, size_t bit, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), bool verbose = false, sc_module_name mn="MicrOmega");

private:
	void gen_thread();
};

