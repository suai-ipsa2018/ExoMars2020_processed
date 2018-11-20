#include "Ma_MISS.h"


SC_HAS_PROCESS(Ma_MISS);
Ma_MISS::Ma_MISS(size_t logical_address, size_t psize, size_t bit, sc_time delay_between_bytes, bool verbose_, sc_module_name mn) :
	Node(mn, logical_address, psize, delay_between_bytes, bit, verbose_)
{
	SC_THREAD(gen_thread);
	SC_THREAD(printing_thread);
}

void Ma_MISS::gen_thread()
{
	while (true)
	{
		Packet p;
		p << 39 << logical_address;
		if (verbose) std::cout << "psize_" << name() << " = " << psize << std::endl;

		for (size_t i = 0; i < psize; i++)
			p << rand();

		send(p);

		wait(100, SC_US);
	}
}

void Ma_MISS::printing_thread()
{
	while (true)
	{
		wait(port->data_written_event()); // idles when no data enters
		std::cout << sc_time_stamp() << " " << name() << " printing_thread receiving packet :" << std::endl;
		Packet p;
		get_packet(p);
	}
}
