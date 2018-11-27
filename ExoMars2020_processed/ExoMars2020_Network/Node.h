#pragma once
#include <random>
#include <sstream>
#include <sqlite3.h>

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "io_port.h"
#include "Packet.h"
#include "helperlib.h"

class Node : public sc_module
{
public:
	io_port port;
protected:
	sc_uint<16> logical_address;
	sc_time delay_between_bytes;
	std::vector<sc_uint<16>> destinations;

	size_t bit;
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist{ 0, (int)(pow(2,Node::bit) - 1) };
	size_t psize;

	ofstream logfile;
	sqlite3* db;
	bool verbose;
private:
	sc_mutex send_mutex, recv_mutex;
	sc_event packet_reception, ack_reception;

	std::vector<Packet> packet_queue, ack_queue;
public:
	Node(sc_module_name mn, const sc_uint<16> &_logical_address, const size_t &_psize, sc_time _delay_between_bytes, size_t _bit = 8, bool _verbose = false);
	virtual ~Node();
  
	sc_uint<16>& get_logical_address();
	void add_destination(const sc_uint<16>& d);
	void init_db(sqlite3* _db);
protected:
	void send(Packet &p);
	unsigned rand();
	void get_packet(Packet &p);
private:
	void send_raw(Packet &p);
	void send_ack(size_t dest, bool state);
	sc_time recv_raw(Packet &p);
	void receiver_daemon();
	void sending_daemon(const sc_uint<16> to);
};
