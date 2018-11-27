#include "io_port.h"


io_channel::io_channel(sc_module_name mn, sc_time transmission_time_, int error_frequency_) : sc_prim_channel((const char*)mn), transmission_time(transmission_time_), error_frequency(error_frequency_)
{
	rng.seed(std::random_device()());
}

void io_channel::write(const sc_uint<16>& n, size_t id)
{
	new_d = n; // New data written
	//if (std::string(basename()) == "Adron_channel") std::cout << "From adron channel: " << n << std::endl;
	if (error_frequency > 0 && new_d > 256 && new_d < sc_uint<16>(-1))
	{
		if (!dist(rng))
		{
			std::cout << sc_time_stamp() << " " << name() << " simulated transmission error ! Byte changed from  " << new_d << " to ";
			new_d = value_dist(rng);
			std::cout << new_d << std::endl;
		}
	}
    new_wid = id; // New id also
    request_update(); // Request an update, for the end of the delta cycle, to set the current data as the new data
}

size_t io_channel::get_wid()
{
    return cur_wid;
}

sc_uint<16> io_channel::read()
{
    e_read.notify(SC_ZERO_TIME);
    return cur_d;
}

const sc_event& io_channel::default_event() const { return e_write; }
const sc_event& io_channel::data_written_event() const { return e_write; }
const sc_event& io_channel::data_read_event() const { return e_read; }

void io_channel::update()
{
    if (cur_wid != new_wid) cur_wid = new_wid;
    if (cur_d != new_d) cur_d = new_d;

    e_write.notify(transmission_time);
	can_read = true;
}


size_t io_port::n_ports=0;

io_port::io_port() : port_id(n_ports)
{
    n_ports++;
}
io_port::io_port(sc_module_name mn) : sc_port<io_if, 1, SC_ONE_OR_MORE_BOUND>(mn), port_id(n_ports)
{
    n_ports++;
}

sc_uint<16> io_port::read()
{
    while (!(*this)->can_read || (*this)->get_wid() == port_id) // Waits if the channel bound is not locked for writing (no write since the last read), or if this port was the last to write something in the channel
        wait((*this)->data_written_event());

    byte_in = (*this)->read(); // Reads from the channel
    (*this)->can_read = false; // Unlocks the channel so that a new write can occur
	(*this)->can_write = true;

    return byte_in;
}

void io_port::write(const sc_uint<16>& n)
{
    while (!(*this)->can_write) // Waits if the channel bound is locked (no read since the last write)
        wait((*this)->data_read_event());

	(*this)->can_write = false;
    byte_out = n;
    (*this)->write(byte_out, port_id); // Writes to the channel
}

void sc_trace(sc_trace_file *_f, const io_channel& object, std::string name_file)
{
    sc_trace(_f, object.cur_d, name_file+"_data");
    sc_trace(_f, object.cur_wid, name_file+"_wid");
}

void sc_trace(sc_trace_file *_f, const io_port& object, std::string name_file)
{
    sc_trace(_f, object.byte_in, name_file+"_in");
	sc_trace(_f, object.byte_out, name_file + "_out");
}
