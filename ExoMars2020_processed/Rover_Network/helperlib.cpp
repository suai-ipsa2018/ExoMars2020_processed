#include "helperlib.h"

std::string formatted_time_stamp()
{
	unsigned short fs(0), ps(0), ns(0), us(0), ms(0), s(0), m(0), h(0);
	uint64 val = sc_time_stamp().value()*(1e-12 / sc_get_time_resolution().to_seconds());
	ps = val % 1000; val /= 1000;
	ns = val % 1000; val /= 1000;
	us = val % 1000; val /= 1000;
	ms = val % 1000; val /= 1000;
	s = val % 60; val /= 60;
	h = val % 60; val /= 60;

	std::string tstr;
	if (h) tstr += std::to_string(h) + "h ";
	if (m) tstr += std::to_string(m) + "m ";
	if (s) tstr += std::to_string(s) + "s ";
	if (ms) tstr += std::to_string(ms) + "ms ";
	if (us) tstr += std::to_string(us) + "us ";
	if (ns) tstr += std::to_string(ns) + "ns ";
	if (ps) tstr += std::to_string(ps) + "ps ";

	if (tstr.empty()) tstr += "0s ";

	return tstr;
}
