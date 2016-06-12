// Copyright 2016 Geoffrey Lawrence Viola

#ifndef NMEALIB_NMEABUILDER_HPP
#define NMEALIB_NMEABUILDER_HPP

#include <cstdint>
#include <string>
#include "gga_fix_quality.hpp"

std::string build_gga(uint8_t utc_hour, uint8_t utc_minute, double utc_seconds,
                      double latitude_degrees, double longitude_degrees,
                      GgaFixQuality fix_quality, uint16_t num_satellites,
                      double hdop, double altitude_m, double geoid_height);
std::string build_vtg(double true_track_made_good_ned_degrees,
                      double ground_velocity_mps);

#endif // NMEALIB_NMEABUILDER_HPP
