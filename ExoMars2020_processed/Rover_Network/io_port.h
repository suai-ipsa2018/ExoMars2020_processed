#pragma once

#include <random>
#include "systemc.h"

/** @file io_port.h
 * @brief Declares the custom bidirectional port io_port, as well as the io_channel and the io_if associated
 */

class io_if : virtual public sc_interface /// Simple inout interface, tracking the last writer by its id (see io_port)
{
public:
    virtual void write(const sc_uint<16>& n, size_t id) = 0; //!< Writes new data to the channel
    virtual sc_uint<16> read() = 0; //!< Reads data from the channel
    virtual size_t get_wid() = 0; //!< Used to access the last writer Id
    virtual const sc_event& data_written_event() const = 0; //!< Returns a reference to an event triggered when data is written
    virtual const sc_event& data_read_event() const = 0; //!< Returns a reference to an event triggered when data is read

	bool can_read{ false }, can_write{ true };
};


class io_channel : public io_if, public sc_prim_channel /// channel using io_if
{
public:
	io_channel(sc_module_name mn, sc_time transmission_time_=sc_time(1./48e6, SC_SEC), int	error_frequency_=0);
    friend void sc_trace(sc_trace_file *_f, const io_channel& object, std::string name_file);
private:
    sc_uint<16> cur_d, //!< Current data
               new_d; //!< New data written, updated at the end of the current delta cycle
    size_t cur_wid, //!< Current last writer id
           new_wid; //!< New last writer id
    sc_event e_read, //!< Event triggered when data is read
             e_write; //!< Event triggered when data is written
	sc_time transmission_time;
	int error_frequency;

	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> value_dist{ 256, 65534 }, dist{ 0, error_frequency };

    /**
     * @brief write Writes new data to the channel
     * @param n Data byte
     * @param id Id of the port requesting a write
     */
    virtual void write(const sc_uint<16>& n, size_t id);

    /**
     * @brief get_wid Used to access the last writer Id
     * @return Last writer Id
     */
    virtual size_t get_wid();

    /**
     * @brief read Reads data from the channel
     * @return Data byte
     */
    virtual sc_uint<16> read();

    virtual const sc_event& default_event() const; //!< Returns a reference to an event triggered when data is written (alias for data_written_event, useful for static sensitivity)
    virtual const sc_event& data_written_event() const; //!< Returns a reference to an event triggered when data is written
    virtual const sc_event& data_read_event() const; //!< Returns a reference to an event triggered when data is read

    /**
     * @brief sc_trace Overload of sc_trace, so an object of type io_channel can be traced
     * @param _f trace_file pointer
     * @param object io_channel instance
     * @param name_file Name of the signal
     */
    virtual void update(); //!< Called at the end of a delta cycle, if request_update has been called
};

class io_port : public sc_port<io_if, 1, SC_ONE_OR_MORE_BOUND> /// Port allowing for safe bidirectional communication on a single channel
{
public:
    io_port(); //!< Default constructor

    /**
     * @brief io_port Constructor allowing for port's name customization (can be useful for debugging)
     * @param mn Port's name
     */
    io_port(sc_module_name mn);

    /**
     * @brief read Read data from an io_channel connected to this ports. Data can only be read once, waits if data has already been read. Also waits if last data has been written by this port
     * @return Data buffered in an io_channel;
     */
    sc_uint<16> read();

    /**
     * @brief write Writes data to an io_channel. Waits if last data hasn't been read yet
     * @param n Data byte
     */
    void write(const sc_uint<16>& n);

    /**
     * @brief sc_trace Overload of sc_trace, so an object of type io_port can be traced
     * @param _f trace_file pointer
     * @param object io_port instance
     * @param name_file Name of the signal
     */

    /**
     * @brief sc_trace Allows for tracing the content passing through a port
     * @param _f Pointer to a vcd trace file
     * @param object Instance of io_port
     * @param name_file Name of the signal, displayed in a waveform viewer
     */
    friend void sc_trace(sc_trace_file *_f, const io_port& object, std::string name_file);

private:
    static size_t n_ports; //!< Static natural integer, keeping track of the number of ports of this kind created
    size_t port_id; //!< Id of an instance of this class. Used to distinguish one port from another and thus ensure transmission safety
    sc_uint<16> byte_in; //!< Used to access the value contained in the channel from NetworkUnit to trace it in a VCD file, through friend function sc_trace
    sc_uint<16> byte_out; //!< Used to access the value contained in the channel from NetworkUnit to trace it in a VCD file, through friend function sc_trace
};
