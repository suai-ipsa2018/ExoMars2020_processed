#pragma once
#include "systemc.h"

#include "PanCam.h"
#include "NavCam.h"
#include "ISEM.h"

#include "CLUPI.h"
#include "Drill.h"
#include "Adron.h"
#include "Wisdom.h"
#include "Ma_MISS.h"
#include "MicrOmega.h"
#include "RLS.h"
#include "MOMA.h"

#include "io_port.h"
#include "switchunit.h"
#include "printunit.h"

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

private:
	sc_time delay_between_bytes;
    
	/*µPanCam m_PanCam;
	NavCam m_NavCam;
	ISEM m_ISEM;

	CLUPI m_CLUPI;
	Drill m_Drill;*/
	Adron m_Adron;
	/*WISDOM m_WISDOM;
	Ma_MISS m_Ma_MISS;
	MicrOmega m_MicrOmega;
	RLS m_RLS;
	MOMA m_MOMA;*/

	PrintUnit pu;

	SwitchUnit router;

	io_channel PanCam_channel{ "PanCam_channel" },
		NavCam_channel{ "NavCam_channel" },
		ISEM_channel{ "ISEM_channel" },
		CLUPI_channel{ "CLUPI_channel" },
		Drill_channel{ "Drill_channel" },
		Adron_channel{ "Adron_channel" },
		WISDOM_channel{ "WISDOM_channel" },
		PrintUnit_channel{ "PrintUnit_channel" },
		Ma_MISS_channel{ "Ma_MISS_channel" },
		MicrOmega_channel{ "MicrOmega_channel" }, 
		RLS_channel{ "RLS_channel" },
		MOMA_channel{ "MOMA_channel" };

    double network_speed; //!< Keeps track of the network's speed

	sc_trace_file* tf;
};