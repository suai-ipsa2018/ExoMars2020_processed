#pragma once
#include <iomanip>
#include "systemc.h"
#include "crc.h"

class Packet
{
	sc_uint<16> receiver_address;
	sc_uint<16> sender_address;
	sc_uint<16> checksum{ 0 };
	static const sc_uint<16> EOP;

	static const size_t header_size;

	size_t i{ 0 };

public:
	std::vector<sc_uint<16>> data;

	Packet();

	Packet& operator<<(const sc_uint<16> &f);
	Packet& operator>>(sc_uint<16> &f);
	explicit operator bool();
	sc_uint<16>& operator[](size_t index);

	const sc_uint<16>& get_receiver_address() const { return receiver_address; }
	void set_receiver_address(const sc_uint<16>& addr);
	const sc_uint<16>& get_sender_address() const { return sender_address; }
	void set_sender_address(const sc_uint<16>& addr);
	sc_uint<16>& get_crc() { return checksum; }

	size_t size();

	friend ostream& operator<<(ostream& flux, Packet &p);

	void reset();
};
