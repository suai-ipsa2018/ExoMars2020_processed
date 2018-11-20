#include "WISDOM.h"


SC_HAS_PROCESS(WISDOM);
WISDOM::WISDOM(size_t logical_address, size_t psize, size_t bit, sc_time delay_between_bytes, bool verbose_, sc_module_name mn) :
	Node(mn, logical_address, psize, delay_between_bytes, bit, verbose_)
{
	SC_THREAD(gen_thread);
	SC_THREAD(printing_thread);
}


void WISDOM::gen_thread()
{
	while (true)
	{
		Packet p;

		p << 38 << logical_address;

		for (size_t i = 0; i < psize; i++)
			p << rand();

		send(p);		
		
		wait(100, SC_US);
	}
}

void WISDOM::printing_thread()
{
	while (true)
	{
		wait(port->data_written_event()); // idles when no data enters
		std::cout << sc_time_stamp() << " " << name() << " printing_thread receiving packet :" << std::endl;
		Packet p;
		get_packet(p);
	}
}
