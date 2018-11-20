#include "printunit.h"

SC_HAS_PROCESS(PrintUnit);
PrintUnit::PrintUnit(sc_module_name mn, double speed, bool verbose_) : Node(mn, 32, 10, sc_time(1.0 / speed, SC_SEC), 16, verbose_)
{
	std::cout << "Print unit verbose = " << verbose_ << std::endl;
    SC_THREAD(printing_thread);
}

void PrintUnit::printing_thread()
{
    while (true)
    {
        wait(port->data_written_event()); // idles when no data enters
        std::cout << sc_time_stamp() << " " << name() << " printing_thread receiving packet :" << std::endl;
		Packet p;
		get_packet(p);
    }
}


std::string PrintUnit::name()
{
    std::string unitname(sc_module::name());
    return "\33[38;5;36m" + unitname + "\33[0m";
}
