// Copyright 2016 Geoffrey Lawrence Viola

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "nmea_lib/nmea_builder.hpp"

using std::string;
using std::stringstream;
using std::fabs;
using std::floor;
using std::setw;
using std::setprecision;
using std::setfill;
using std::fixed;

string convert_to_nmea_degrees(double const angle_degrees,
                               bool const three_digits);
uint8_t calc_checksum(char const *const in, uint8_t const length);
string get_checksum_string(string const &in);

string convert_to_nmea_degrees(double const angle_degrees,
                               bool const three_digits)

{
  string output;
  double const pos_angle_degrees = fabs(angle_degrees);

  // Get the decimal value to use for the minutes
  double min_dec = pos_angle_degrees - floor(pos_angle_degrees);
  min_dec *= 60.0;

  int32_t angle_degrees_int;
  int32_t min_int;

  min_int = static_cast<int32_t>(floor(min_dec));
  min_dec = min_dec - floor(min_dec);
  angle_degrees_int = static_cast<int32_t>(floor(pos_angle_degrees));

  stringstream ss, min_int_ss, min_dec_ss;

  ss << fixed << setfill('0') << (three_digits ? setw(3) : setw(2))
     << angle_degrees_int;
  output = ss.str();
  min_int_ss << fixed << setfill('0') << std::setw(2) << min_int;
  output += min_int_ss.str();
  min_dec_ss << fixed << setw(8) << std::setprecision(8) << min_dec;
  string const tmp1 = min_dec_ss.str();
  string const tmp2 = tmp1.substr(1, tmp1.length() - 1);
  output += tmp2.c_str();

  return output;
}

uint8_t calc_checksum(char const *const in, uint8_t const length)
{
  uint8_t calculatedCheckSum = 0;
  for (uint16_t i = 1; i < length - 1; i++)
  {
    calculatedCheckSum = calculatedCheckSum ^ in[i];
  }
  return calculatedCheckSum;
}

string get_checksum_string(string const &in)
{
  string output;
  uint8_t checksum =
      calc_checksum(in.c_str(), static_cast<uint8_t>(in.length()));
  stringstream checksumSS;
  checksumSS << std::hex << std::setfill('0') << std::uppercase << std::setw(2)
             << static_cast<uint16_t>(checksum);
  output = checksumSS.str();
  return output;
}

string build_gga(uint8_t const utc_hour, uint8_t const utc_minute,
                 double const utc_seconds, double const latitude_degrees,
                 double const longitude_degrees,
                 GgaFixQuality const fix_quality, uint16_t const num_satellites,
                 double const hdop, double const altitude_m,
                 double const geoid_height)
{
  stringstream utcTime, fixType, numSats, hdop_ss, mslAlt, heightOfGeoid;
  string output = "$GPGGA,";
  // time
  utcTime << setfill('0') << setw(2) << static_cast<uint16_t>(utc_hour);
  utcTime << setfill('0') << setw(2) << static_cast<uint16_t>(utc_minute);
  utcTime << setfill('0') << fixed << setw(5) << setprecision(2) << utc_seconds;
  output += utcTime.str() + ",";
  // lat
  output += convert_to_nmea_degrees(latitude_degrees, false) + ",";
  if (latitude_degrees > 0)
  {
    output += "N,";
  }
  else
  {
    output += "S,";
  }

  // lon
  output += convert_to_nmea_degrees(longitude_degrees, true) + ",";
  if (longitude_degrees > 0)
  {
    output += "E,";
  }
  else
  {
    output += "W,";
  }

  // fix type
  fixType << static_cast<uint16_t>(fix_quality);
  output += fixType.str() + ",";

  // num sats
  numSats << num_satellites;
  output += numSats.str() + ",";

  // hdop
  hdop_ss << hdop;
  output += hdop_ss.str() + ",";

  // altitude
  mslAlt << setfill('0') << fixed << setw(6) << setprecision(3) << altitude_m;
  output += mslAlt.str() + ",M,";

  // height of geoid
  heightOfGeoid << fixed << setprecision(1) << geoid_height;
  output += heightOfGeoid.str() + ",M,";

  output += ",*";

  output += get_checksum_string(output);

  output += "\n";

  return output;
}

string build_vtg(double const true_track_made_good_ned_degrees,
                 double const ground_velocity_mps)
{
  string output = "$GPVTG,";

  stringstream trackSS, velocityKnotsSS, velocityKphSS;

  double const vel_knots = ground_velocity_mps * 1.94384;
  double const vel_kph = ground_velocity_mps * 3.6;

  // track angle (no mag angle)
  trackSS << std::setfill('0') << std::fixed << std::setw(5)
          << std::setprecision(1) << true_track_made_good_ned_degrees;
  output += trackSS.str();
  output += ",T,,M,";

  // vel knots
  velocityKnotsSS << std::fixed << std::setprecision(3) << vel_knots;
  output += velocityKnotsSS.str();
  output += ",N,";

  // vel kph
  velocityKphSS << std::fixed << std::setprecision(3) << vel_kph;
  output += velocityKphSS.str();
  output += ",K*";

  // checksum
  output += get_checksum_string(output);
  output += "\n";

  return output;
}
