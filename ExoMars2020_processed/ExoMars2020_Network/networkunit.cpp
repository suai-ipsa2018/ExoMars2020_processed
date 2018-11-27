#include "networkunit.h"


NetworkUnit::NetworkUnit(sc_module_name mn, double speed, bool verbose) :
	sc_module(mn),
	delay_between_bytes(sc_time(1. / speed, SC_SEC)),

	router("router", 32, verbose),
	network_speed(speed)
{
	ConfigLoader cfg("config/Network.cfg");

#if NETWORK_PART == 0
	std::map<std::string, int> addresses = cfg.get_first_part_la();
	std::vector<std::array<int, 2>> traffic_map = cfg.get_first_part();

	addresses.insert(cfg.get_second_part_la().begin(), cfg.get_second_part_la().end());
	traffic_map.insert(traffic_map.end(), cfg.get_second_part().begin(), cfg.get_second_part().end());
#elif NETWORK_PART == 1
	std::map<std::string, int> addresses = cfg.get_first_part_la();
	std::vector<std::array<int, 2>> traffic_map = cfg.get_first_part();
#elif NETWORK_PART == 2
	std::map<std::string, int> addresses = cfg.get_second_part_la();
	std::vector<std::array<int, 2>> traffic_map = cfg.get_second_part();
#endif

	tf = sc_create_vcd_trace_file("traces/network");
	sqlite3_open("logs/Network.db", &db);

	instruments.reserve(addresses.size());
	channels.reserve(addresses.size());
	for (auto me : addresses)
	{
		instruments.push_back(std::make_unique<Node>(me.first.c_str(), me.second, 16, delay_between_bytes, 8, verbose));
		channels.push_back(std::make_unique<io_channel>((me.first + "_channel").c_str()));
		router.connect(*instruments.back(), *channels.back());

		sc_trace(tf, *channels.back(), channels.back()->basename());
		instruments.back()->init_db(db);
	}
	router.connections_done();
	for (std::unique_ptr<Node> &i : instruments)
	{
		for (auto pair : traffic_map)
			if (pair[0] == i->get_logical_address())
				i->add_destination(pair[1]);
	}
}

NetworkUnit::~NetworkUnit()
{
	sqlite3_close(db);
}
