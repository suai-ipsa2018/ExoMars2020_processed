#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),
	m_PanCam(33, 8, 10, delay_between_bytes, verbose),
	m_NavCam(34, 8, 8,delay_between_bytes, verbose),
	m_ISEM(35, 8, 16, delay_between_bytes, verbose),

	pu("PrintUnit", speed, verbose),
	router("router", 32, verbose),
	network_speed(speed)
{
	tf = sc_create_vcd_trace_file("traces/mast_network");
	sc_trace(tf, PanCam_channel, "PanCam_channel");
	sc_trace(tf, NavCam_channel, "NavCam_channel");
	sc_trace(tf, ISEM_channel, "ISEM_channel");
	sc_trace(tf, PrintUnit_channel, "PrintUnit_channel");

	router.connect(m_PanCam, PanCam_channel);
	router.connect(m_NavCam, NavCam_channel);
	router.connect(m_ISEM, ISEM_channel);

	router.connect(pu, PrintUnit_channel);
	router.connections_done();
}