#pragma once
#include "Node.h"
class Adron : public Node
{
public:
	Adron(size_t logical_address, size_t psize, size_t bit, sc_time delay_between_bytes = sc_time(1. / 24e6, SC_SEC), bool verbose_ = false, sc_module_name mn="Adron");
private:
	void gen_thread();
	void printing_thread();
};

