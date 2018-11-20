#include "Node.h"

SC_HAS_PROCESS(Node);
Node::Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t & _psize, sc_time _delay_between_bytes, size_t _bit, bool _verbose)
	: sc_module(mn), logical_address(_logical_address), psize(_psize), delay_between_bytes(_delay_between_bytes), bit(_bit), verbose(_verbose),
	port((std::string((const char*)mn) + "_port").c_str()),
	logfile("logs/" + (std::string((const char*)mn) + ".log"))
{
	rng.seed(std::random_device()());
	logfile << "Node " << name() << " with logic address " << logical_address << ":" << std::endl;

	SC_THREAD(daemon);
}

Node::~Node()
{
	logfile.close();
}

sc_uint<16>& Node::get_logical_address()
{
	return logical_address;
}

void Node::send_raw(Packet & p)
{
	send_mutex.lock();
	logfile << formatted_time_stamp() << ' ' << name() << " sending packet of size " << p.size() << " to " << p.get_receiver_address() << std::endl;
	if (verbose) logfile << p << std::endl;
	p.reset();
	sc_uint<16> tmp;
	while (p >> tmp)
	{
		port.write(tmp);
		wait(delay_between_bytes);
	}
	send_mutex.unlock();
}

void Node::send_ack(size_t dest, bool state) // Function spawned to send an ack packet (avoids blocking the PrintUnit for this)
{
	Packet p;
	p << dest << logical_address << state;
	send_raw(p);
}

void Node::send(Packet &p)
{
	bool ack_received(false);
	send_raw(p); // Send the packet

	// Wait for confirmation
	while (!ack_received)
	{
		for (size_t i = 0; i < ack_queue.size(); i++)
		{
			if (ack_queue[i].get_sender_address() == p.get_receiver_address())
			{
				if (ack_queue[i][0])
				{
					std::cout << formatted_time_stamp() << ' ' << name() << " received ack, positive response !" << std::endl;
					ack_queue.erase(ack_queue.begin() + i);
				}
				else
				{
					std::cout << formatted_time_stamp() << ' ' << name() << " received ack, negative response, must send again" << std::endl;
					ack_queue.erase(ack_queue.begin() + i);
					send(p);
				}
				ack_received = true;
				break;
			}
		}
		if(!ack_received) wait(ack_reception);
	}
}

sc_time Node::recv_raw(Packet & p)
{
	p.reset();
	sc_uint<16> tmp(0);
	sc_time t0(sc_time_stamp());
	while (!tmp.and_reduce())
	{
		tmp = port.read();
		p << tmp;
	}
	sc_time t1(sc_time_stamp());
	return t1-t0;
}

void Node::daemon()
{
	sc_time t;
	while (true)
	{
		Packet p;
		t = recv_raw(p);

		if (p.size() > 1)
		{
			logfile << formatted_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << t.to_seconds() << "s" << std::endl;
			if (verbose) logfile << p << std::endl;
			std::cout << sc_time_stamp() << ' ' << name() << " received packet of size " << p.size() << " from " << p.get_sender_address() << " in " << t.to_seconds() << "s" << std::endl;
			if (verbose) std::cout << "\33[48;5;194;38;5;0m" << p << "\33[0m" << std::endl;

			if (p.get_crc())
			{
				std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;197m" << "WRONG CRC" << "\33[0m" << std::endl;
				sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 0)); // Spawns a thread to send an ack packet with data 0, signaling a transmission error
			}
			else
			{
				std::cout << sc_time_stamp() << " " << name() << " \33[1;38;5;40m" << "CORRECT CRC" << "\33[0m" << std::endl;
				sc_spawn(sc_bind(&Node::send_ack, this, p.get_sender_address(), 1)); // Spawns a thread to send an ack packet with data 1, signaling a successful transmission
			}
			packet_queue.push_back(p);
			packet_reception.notify(SC_ZERO_TIME);
		}
		else // In case of ack reception
		{
			if (verbose) std::cout << "ack received by " << name() << ": " << std::endl << p << std::endl;
			logfile << "ack received: " << std::endl << p << std::endl;
			ack_queue.push_back(p);
			ack_reception.notify(SC_ZERO_TIME);
		}
	}
}

unsigned Node::rand()
{
	return dist(rng);
}

void Node::get_packet(Packet &p)
{
	while (!packet_queue.size())
		wait(packet_reception);
	p = packet_queue[0];
	packet_queue.erase(packet_queue.begin());
}
