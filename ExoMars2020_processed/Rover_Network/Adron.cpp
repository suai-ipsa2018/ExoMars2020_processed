#include "Adron.h"


SC_HAS_PROCESS(Adron);
Adron::Adron(size_t logical_address, size_t psize, size_t bit, sc_time delay_between_bytes, bool verbose_, sc_module_name mn) :
	Node(mn, logical_address, psize, delay_between_bytes, bit, verbose_)
{
	SC_THREAD(gen_thread);
	SC_THREAD(printing_thread);
}


void Adron::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 32 << logical_address;
		if (verbose) std::cout << "psize_" << name() << " = " << psize << std::endl;
		for (size_t i = 0; i < psize; i++)
			p << rand();

		send(p);

		wait(100, SC_US);
	}
}

void Adron::printing_thread()
{
	while (true)
	{
		wait(port->data_written_event()); // idles when no data enters
		Packet p;
		get_packet(p);
	}
}