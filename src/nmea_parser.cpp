// Copyright 2016 Geoffrey Lawrence Viola

#include <vector>
#include <tuple>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <string>
#include "nmea_lib/nmea_parser.hpp"

using std::string;
using std::vector;
using std::tuple;
using std::get;

static tuple<bool, double> parse_degrees_minutes_angle(string const &message);

AvrMessageData parse_avr(string const &message)
{
  static string const AVR_START("$PTNL,AVR,");
  AvrMessageData output;
  output.valid = false;

  if (AVR_START == message.substr(0, AVR_START.length()))
  {
    string::size_type previous_n = AVR_START.length();
    string::size_type next_n = message.find(",", previous_n);
    output.timestamp = stod(message.substr(previous_n, next_n - previous_n));
    ++next_n;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.yaw = stod(message.substr(previous_n, next_n - previous_n));
    next_n += 5U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.tilt = stod(message.substr(previous_n, next_n - previous_n));
    next_n += 8U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.range = stod(message.substr(previous_n, next_n - previous_n));
    next_n += 1U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.fixQuality = static_cast<AvrFixQuality>(
        stod(message.substr(previous_n, next_n - previous_n)));
    next_n += 1U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.pdop = stod(message.substr(previous_n, next_n - previous_n));
    next_n += 1U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.numSatellites = static_cast<uint16_t>(
        stoi(message.substr(previous_n, next_n - previous_n)));
    output.valid = true;
  }

  return output;
}

tuple<bool, double> parse_degrees_minutes_angle(string const &message)
{
  string::size_type period_location = message.find(".");
  bool const is_valid = period_location != message.length();
  double angle_degrees = 0.0;

  if (is_valid)
  {
    double const whole_degrees = stod(message.substr(0U, period_location - 2U));
    double const whole_minutes =
        stod(message.substr(period_location - 2U, period_location));
    string const double_minutes_str =
        message.substr(period_location + 1U, message.length());
    double const decimal_minutes =
        stod(double_minutes_str) / std::pow(10, double_minutes_str.size() - 1);
    angle_degrees = whole_degrees + (whole_minutes + decimal_minutes) / 60.0;
  }

  return tuple<bool, double>(is_valid, angle_degrees);
}

GgaMessageData parse_gga(string const &message)
{
  static string const GGA_START("$GPGGA,");
  GgaMessageData output;
  output.valid = false;
  if (GGA_START == message.substr(0, GGA_START.length()))
  {
    string::size_type previous_n = GGA_START.length();
    string::size_type next_n = message.find(",", previous_n);
    output.timestamp = stod(message.substr(previous_n, next_n - previous_n));
    previous_n = next_n;
    next_n = message.find(",", next_n + 1U);
    tuple<bool, double> const latitude_v_a(parse_degrees_minutes_angle(
        message.substr(previous_n + 1, next_n - previous_n)));
    if (get<0>(latitude_v_a))
    {
      previous_n = next_n;
      next_n = message.find(",", next_n + 1U);
      double const latitude_multiplier =
          ("N" == message.substr(previous_n + 1, next_n - previous_n - 1)
               ? 1.0
               : -1.0);
      output.latitude = get<1>(latitude_v_a) * latitude_multiplier;
      previous_n = next_n;
      next_n = message.find(",", next_n + 1U);
      tuple<bool, double> const longitude_v_a(parse_degrees_minutes_angle(
          message.substr(previous_n + 1, next_n - previous_n)));
      if (get<0>(longitude_v_a))
      {
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        double const longitude_multiplier =
            ("E" == message.substr(previous_n + 1, next_n - previous_n - 1)
                 ? 1.0
                 : -1.0);
        output.longitude = get<1>(longitude_v_a) * longitude_multiplier;
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        output.fixQuality = static_cast<GgaFixQuality>(
            stoi(message.substr(previous_n + 1, next_n - previous_n - 1)));
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        output.numSatellites =
            stoi(message.substr(previous_n + 1, next_n - previous_n - 1));
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        output.hdop =
            stod(message.substr(previous_n + 1, next_n - previous_n - 1));
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        output.altitude =
            stod(message.substr(previous_n + 1, next_n - previous_n - 1));
        next_n += 2U;
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        output.geoidHeight =
            stod(message.substr(previous_n + 1, next_n - previous_n - 1));
        next_n += 2U;
        previous_n = next_n;
        next_n = message.find(",", next_n + 1U);
        if (1 < next_n - previous_n)
        {
          output.SetTimeSinceLastDgps(
              stod(message.substr(previous_n + 1, next_n - previous_n - 1)));
        }
        previous_n = next_n;
        next_n = message.find("*", next_n + 1U);
        if (1 < next_n - previous_n)
        {
          output.SetDgpsStationID(static_cast<uint16_t>(
              stoi(message.substr(previous_n + 1, next_n - previous_n - 1))));
        }
        output.valid = true;
      }
    }
  }

  return output;
}

VtgMessageData parse_vtg(string const &message)
{
  static string const VTG_START("$GPVTG,");
  VtgMessageData output;
  output.valid = false;

  if (VTG_START == message.substr(0, VTG_START.length()))
  {
    string::size_type previous_n = VTG_START.length();
    string::size_type next_n = message.find(",", previous_n);
    output.trueTrackMadeGood =
        stod(message.substr(previous_n, next_n - previous_n));
    next_n += 3;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    if (1 < next_n - previous_n)
    {
      output.SetMagneticTrackMadeGood(
          stod(message.substr(previous_n, next_n - previous_n)));
    }
    next_n += 3U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.groundSpeedKnots =
        stod(message.substr(previous_n, next_n - previous_n));
    next_n += 3U;
    previous_n = next_n;
    next_n = message.find(",", next_n);
    output.groundSpeedKph =
        stod(message.substr(previous_n, next_n - previous_n));
    output.valid = true;
  }

  return output;
}
