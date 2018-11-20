#pragma once

#include <iomanip>
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc.h"
#include "io_port.h"
#include "Node.h"

/** @file switchunit.h
 * @brief Declares the switch module, as well as a helper class to describe tables of commutation
 */

class SwitchMatrix /// Convenience class to describe SpaceWire router's routing tables.
/** SpaceWire router's routing tables (or commutation tables)
 *
 *
 */
{
public:
	SwitchMatrix(size_t n_ports);

    /**
     * @brief Constructs a SwitchMatrix of size (32+n, m), filled as an identity matrix. Logical addresses are filled using the intializer list il
     * @param n Number of rows
     * @param m Number of columns
     * @param il Initializer list of logical addresses, of length n*m
     */
    SwitchMatrix(const std::vector<bool> &A, size_t n, size_t m);

	void add_port();

	bool is_connected(size_t _port);

    /**
     * @brief operator () Returns a copy of the component in position (i, j) of the matrix
     * @param i Row index
     * @param j Column index
     * @return \f$m_{i,j}\f$ (can be modified, no influence on the matrix)
     */
    bool operator()(size_t i, size_t j) const;

	void set_logical_route(size_t i, size_t j);

    /**
     * @brief get_n Get number of rows
     * @return The number of rows
     */
    size_t get_n() const;

    /**
     * @brief get_m Get number of columns
     * @return The number of colmuns
     */
    size_t get_m() const;

    /**
     * @brief operator << Convenience function to output a SwitchMatrix to a stream
     * @param flux Output stream
     * @param M SwitchMatrix to output
     * @return flux, so that chaining is possible
     */
    friend std::ostream& operator<<(std::ostream& flux, const SwitchMatrix& M);

private:
    std::vector<bool> M; //!< Bidimensional array of booleans, stores the data of the matrix
    size_t n, //!< Number of rows
           m; //!< Number of columns
};

std::ostream& operator<<(std::ostream&, const SwitchMatrix&);


class waitable_mutex : virtual public sc_mutex /// Very simple class adding an unlock_event to regular sc_mutex
{
public:
    waitable_mutex();
    const sc_event& unlock_event() const; //!< Returns an event notified on the unlocking of this mutex
};

class SwitchUnit : sc_module /// Module modelling a SpaceWire router
{
public:
    /**
     * @brief Constructs a SwitchUnit respecting routing_matrix's specifications
     * @param mn Module's name
     * @param routing_matrix Routing matrix (aka Table of commutation)
     */
    SwitchUnit(sc_module_name mn, size_t n_ports_=32, bool verbose=true);
	 void connections_done();
	 ~SwitchUnit(); //!< Deallocates memory

	void connect(Node &n, io_channel &c);

    std::vector<io_port> ports; //!< Pointer to an array of ports, as much as required in the routing_matrix

private:
    void init_thread(); //!< First thread launched, spawns all the port_processing threads

    /**
     * @brief port_processing Thread designed to be spawned at the start of the simulation, processing packets arriving at port in_port_number
     * @param in_port_number Port to handle
     */
    void port_processing(size_t in_port_number);
    std::string name(); //!< Little function to color sc_module's name() result

    std::vector<waitable_mutex> out_ports_access; //!< Mutex to manage output ports, to be sure no data is overwritten

    SwitchMatrix sm; //!< SwitchMatrix caracterizing the SwitchUnit
    size_t n_ports; //!< Number of ports, deduced from sm
	size_t i_port{ 0 };
	io_channel unused_channel{ "unused_channel" };
	bool verbose;
};