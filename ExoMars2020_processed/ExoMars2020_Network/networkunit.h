#pragma once
#include <memory>
#include <systemc.h>

#include "io_port.h"
#include "switchunit.h"

#include "helperlib.h"


#ifndef NETWORK_PART
#define NETWORK_PART 0
#endif

/** @file networkunit.h
 * @brief Declares the network module, using all the other modules
 */

class NetworkUnit : sc_module /// Top-level module, creating all sub-modules from the simple network simulated
{
public:
    /**
     * @param mn Module's name
     * @param psize Packet size
     * @param speed Speed of the network, in bit/s
     */
    NetworkUnit(sc_module_name mn, double speed=24e6, bool verbose=true);
	~NetworkUnit();

private:
	sc_time delay_between_bytes;
    
	std::vector<std::unique_ptr<Node>> instruments;
	std::vector<std::unique_ptr<io_channel>> channels;

	SwitchUnit router;

    double network_speed; //!< Keeps track of the network's speed

	sc_trace_file* tf;
	sqlite3* db;
};