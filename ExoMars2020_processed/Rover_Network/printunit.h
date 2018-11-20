#pragma once

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "Node.h"

/** @file printunit.h
 * @brief Declares the output module
 */

class PrintUnit : public Node /// A sink, printing every packet it recives on its port
{
public:
    /**
     * @param mn Module's name
     */
    PrintUnit(sc_module_name mn, double speed=24e6, bool verbose_=true);

private:
    void printing_thread(); //!< Thread in charge of reading from the port as much as possible, computing crc, and sending back ack packets
    std::string name(); //!< Little function to color sc_module's name() result
    sc_time delay_between_bytes; //!< Delay between two consecutive bytes, calculated with the network's speed

	bool verbose;
};