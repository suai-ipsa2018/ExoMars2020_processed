#include "switchunit.h"

SwitchMatrix::SwitchMatrix(size_t n_ports) : n(224), m(n_ports), M(256*n_ports)
{
	for (size_t i = 0; i < 32; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			M[i*m + j] = (i == j) ? 1 : 0;
		}
	}
}

SwitchMatrix::SwitchMatrix(const std::vector<bool> &A, size_t n_logical_addresses, size_t n_ports) : n(n_logical_addresses), m(n_ports),
M((32+n_logical_addresses)*n_ports)
{
    // Same thing as above, but using an initializer_list instead of a C array
    // Following is identical as above
    for (size_t i=0 ; i<n+32 ; i++)
    {
        for (size_t j=0 ; j<m ; j++)
        {
            M[i*m + j] = (i==j) ? 1 : 0;
        }
    }

    for (size_t i=0 ; i<n ; i++)
    {
        for (size_t j=0 ; j<m ; j++)
        {
            M[(i+32)*m + j] = A[i*m + j];
        }
    }
}


void SwitchMatrix::add_port()
{
	std::vector<bool>::iterator b;
	for (size_t pos = m; pos < (n + 32)*(m+1); pos += m+1)
	{
		b = M.begin();
		M.insert(b + pos, (pos == (m+1)*(m+1)-1) ? 1 : 0);
	}
	m++;
}

bool SwitchMatrix::is_connected(size_t _port)
{
	bool r(0);
	for (size_t i =32; i < n+32; i++)
		r |= (*this)(i, _port);
	return r;
}

bool SwitchMatrix::operator()(size_t i, size_t j) const
{
    return M[i*m + j];
}

void SwitchMatrix::set_logical_route(size_t i, size_t j)
{
	M[i*m + j] = 1;
}

size_t SwitchMatrix::get_n() const
{
    return n;
}

size_t SwitchMatrix::get_m() const
{
    return m;
}

std::ostream& operator<<(std::ostream& flux, const SwitchMatrix& M)
{
	flux << "Out Port\t";
	for (size_t j = 0; j < M.m; j++)
		flux << std::setw(2) << j << ' ';
	flux << "\nAddress";
	flux << std::endl;
	for (size_t i = 0; i < M.n + 32; i++)
	{
		flux << i << "\t\t";
		for (size_t j = 0; j < M.m; j++)
			flux << std::setw(2) << M(i, j) << ' ';
		flux << std::endl;
	}
	return flux;
}

waitable_mutex::waitable_mutex() : sc_mutex() {}

const sc_event& waitable_mutex::unlock_event() const
{
    return m_free;
}


SC_HAS_PROCESS(SwitchUnit);
SwitchUnit::SwitchUnit(sc_module_name mn, size_t n_ports_, bool verbose)
    : sc_module(mn), sm(n_ports_), n_ports(n_ports_)
{
	ports = std::vector<io_port>(n_ports);
	out_ports_access = std::vector<waitable_mutex>(n_ports);

	ports[0](unused_channel); i_port++;
}

void SwitchUnit::connections_done()
{
	for (; i_port < n_ports; i_port++)
	{
		ports[i_port](unused_channel);
	}
	if (verbose)
	{
		std::cout << "Switch Matrix of " << name() << ":" << std::endl;
		std::cout << sm << std::endl;
	}

	SC_THREAD(init_thread);
}
SwitchUnit::~SwitchUnit()
{
}

void SwitchUnit::connect(Node & n, io_channel &c)
{
	sm.set_logical_route(n.get_logical_address(), i_port);

	n.port(c);
	ports[i_port](c); i_port++;
}

void SwitchUnit::init_thread()
{
    wait(SC_ZERO_TIME); // Preempts this thread, so that everything is settled in the simulation before spawning the threads
    std::cout << sc_time_stamp() << " " << name() << " spawning port_processing threads" << std::endl;
    for (size_t i=0 ; i<n_ports ; i++)
    {
		if (sm.is_connected(i))
			sc_spawn(sc_bind(&SwitchUnit::port_processing, this, i)); // Spawns port_processing threads, passing as argument i, the id of the port they will manage
    }
}

void SwitchUnit::port_processing(size_t in_port_number)
{
    std::cout << sc_time_stamp() << " " << name() << " port_processing spawned to handle in_port " << in_port_number << std::endl;
    sc_uint<16> byte, address, sender_address;
    size_t out_port_number, n_ports_at_address;
    while (true)
    {
        byte = 0;
        n_ports_at_address = 0; // Will be used to count the number of ports usable to reach the logical address of the receiver of the packet
        sc_event_or_list compound_event;

// Header gathering
        address = ports[in_port_number].read();
        sender_address = ports[in_port_number].read();

// Select port
        std::cout << sc_time_stamp() << " " << name() << "'s in_port " << in_port_number << " selecting port" << std::endl;

        // List all ports usuable to reach the logical address given
        for (out_port_number=0 ; out_port_number < n_ports ; out_port_number++) // Goes through the line 'address' of the switch matrix
        {
            if (sm(address, out_port_number)) // If the port is valid for the address
            {
                compound_event = compound_event | out_ports_access[out_port_number].unlock_event(); // Add the unlock event of the current port's corresponding mutex
                n_ports_at_address++; // Counts the number of usable ports
            }
        }
        if (!n_ports_at_address) // If there is no port (line address is full of zeros
        {
            std::cout << sc_time_stamp() << ' ' << name() << "'s in_port " << in_port_number << "\33[1m" << " didn't find any out_port for address " << address << " packet will be discarded..." << "\33[0m" << std::endl;
            while (!ports[in_port_number].read().and_reduce()); // Discards the packet
        }

        // Actual selection of port
        for (out_port_number=0 ; !sm(address, out_port_number) || out_ports_access[out_port_number].trylock() ; out_port_number++) // Cycle through all ports, while looking if the port can be used to reach address, and if the port can be locked
        {
            if (out_port_number==n_ports) // If all ports have been examined, we need to wait for an unlock
            {
                out_port_number = 0; // Reinitialisation
                wait(compound_event); // Wait until one of the useable ports has been unlocked
            }
        }

        std::cout << sc_time_stamp() << " " << name() << "'s in_port " << in_port_number << "\33[1m" << " locked out_port " << out_port_number << "\33[0m" << ", ready to send packet" << std::endl;

        // route header
        ports[out_port_number].write(address);
        ports[out_port_number].write(sender_address);

        // route data
        while (!byte.and_reduce())
        {
            byte = ports[in_port_number].read();
            ports[out_port_number].write(byte);
            wait(SC_ZERO_TIME);
        }
        out_ports_access[out_port_number].unlock();
        std::cout << sc_time_stamp() << " " << name() << "'s in_port " << in_port_number << "\33[1m" << " unlocked out_port " << out_port_number << "\33[0m" << std::endl;
    }
}

std::string SwitchUnit::name()
{
    std::string unitname(sc_module::name());
    return "\33[38;5;45m" + unitname + "\33[0m";
}
