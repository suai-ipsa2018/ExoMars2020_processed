#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),
	/*m_PanCam(33, 8, 10, delay_between_bytes, verbose),
	m_NavCam(34, 8, 8,delay_between_bytes, verbose),
	m_ISEM(35, 8, 16, delay_between_bytes, verbose),
	m_CLUPI(36, 8, 14, delay_between_bytes, verbose),
	m_Drill(37, 8, 8, delay_between_bytes, verbose),*/
	m_Adron(38, 8, 9, delay_between_bytes, verbose),
	/*m_WISDOM(39, 8, 16, delay_between_bytes, verbose),
	m_Ma_MISS(40, 8, 8, delay_between_bytes, verbose),
	m_MicrOmega(41, 8, 8, delay_between_bytes, verbose),
	m_RLS(42, 8, 8, delay_between_bytes, verbose),
	m_MOMA(43, 8, 8, delay_between_bytes, verbose),*/

	pu("PrintUnit", speed, verbose),
	router("router", 32, verbose),
	network_speed(speed)
{
	tf = sc_create_vcd_trace_file("traces/network");
	sc_trace(tf, PanCam_channel, "PanCam_channel");
	sc_trace(tf, NavCam_channel, "NavCam_channel");
	sc_trace(tf, ISEM_channel, "ISEM_channel");
	sc_trace(tf, CLUPI_channel, "CLUPI_channel");
	sc_trace(tf, Drill_channel, "Drill_channel");
	sc_trace(tf, Adron_channel, "Adron_channel");
	sc_trace(tf, WISDOM_channel, "WISDOM_channel");
	sc_trace(tf, Ma_MISS_channel, "Ma_MISS_channel");
	sc_trace(tf, MicrOmega_channel, "MicrOmega_channel");
	sc_trace(tf, RLS_channel, "RLS_channel");
	sc_trace(tf, MOMA_channel, "MOMA_channel");

	sc_trace(tf, PrintUnit_channel, "PrintUnit_channel");


	//sc_trace(tf, m_Adron.port, "Adron_port");
	//sc_trace(tf, m_WISDOM.port, "WISDOM_port");

	sc_trace(tf, pu.port, "PrintUnit_port");



	/*router.connect(m_PanCam, PanCam_channel);
	router.connect(m_NavCam, NavCam_channel);
	router.connect(m_ISEM, ISEM_channel);
	router.connect(m_CLUPI, CLUPI_channel);
	router.connect(m_Drill, Drill_channel);*/
	router.connect(m_Adron, Adron_channel);
	/*router.connect(m_WISDOM, WISDOM_channel);
	router.connect(m_Ma_MISS, Ma_MISS_channel);
	router.connect(m_MicrOmega, MicrOmega_channel);
	router.connect(m_RLS, RLS_channel);
	router.connect(m_MOMA, MOMA_channel);*/

	router.connect(pu, PrintUnit_channel);
	router.connections_done();
	sc_trace(tf, router.ports[1], "router_port_adron");
}