#include "Flight.h"
#include <sstream>

Flight::Flight()
{
}

Flight::Flight(uint32 number, const std::string& takeoff_time, const std::string& start, const std::string& end, uint32 during_time, uint32 max_load, uint32 booked, std::string take_off, uint32 price)
	:number(number), takeoff_time(takeoff_time), start(start), end(end),
	during_time(during_time), max_load(max_load), booked(booked), take_off(take_off), price(price)
{
	hour = std::atoi(takeoff_time.substr(0, 2).c_str());
	minute = std::atoi(takeoff_time.substr(3).c_str());
	if (take_off == "ÊÇ") take_off_bool = true;
	else take_off_bool = false;
}

void Flight::init()
{
	hour = std::atoi(takeoff_time.substr(0, 2).c_str());
	minute = std::atoi(takeoff_time.substr(3).c_str());
	if (take_off == "ÊÇ") take_off_bool = true;
	else take_off_bool = false;
}

std::string Flight::formatInfo()
{
	std::stringstream ss;
	ss << number << '\t' << takeoff_time << '\t' << start << '\t' << end << '\t' << during_time << '\t' << max_load << '\t' << booked << '\t' << take_off << '\t' << price << std::endl;

	return ss.str();
}
