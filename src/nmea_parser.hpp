// Copyright 2016 Geoffrey Lawrence Viola

#include <string>

struct AvrMessageData
{
  enum AvrFixQuality
  {
    INVALID = 0,
    GPS,
    RTK_FLOAT,
    RTK_FIX,
    DGPS
  };

  inline AvrMessageData()
      : valid(false)
  {
  }
  inline AvrMessageData(double const in_timestamp, double const in_yaw,
                        double const in_tilt, double const in_range,
                        AvrFixQuality fix_quality, double const in_pdop,
                        uint16_t num_satellites)
      : valid(true)
      , timestamp(in_timestamp)
      , yaw(in_yaw)
      , tilt(in_tilt)
      , range(in_range)
      , fixQuality(fix_quality)
      , pdop(in_pdop)
      , numSatellites(num_satellites)
  {
  }

  bool valid;
  double timestamp;
  double yaw;
  double tilt;
  double range;
  AvrFixQuality fixQuality;
  double pdop;
  uint16_t numSatellites;
};

struct GgaMessageData
{
  enum FixQuality
  {
    INVALID = 0,
    GPS_FIX,
    DGPS_FIX,
    PPS_FIX,
    REAL_TIME_KINEMATIC,
    FLOAT_RTK,
    DEAD_RECKONING,
    MANUAL_INPUT,
    SIMULATION
  };

  GgaMessageData()
      : valid(false)
      , timeSinceLastDgpsValid(false)
      , dgpdStationIDValid(false)
  {
  }

  GgaMessageData(double const in_timestamp, double const in_latitude,
                 double const in_longitude, FixQuality const fix_quality,
                 uint16_t const num_satellites, double const in_hdop,
                 double const in_altitude, double const geoid_height)
      : valid(true)
      , timestamp(in_timestamp)
      , latitude(in_latitude)
      , longitude(in_longitude)
      , fixQuality(fix_quality)
      , numSatellites(num_satellites)
      , hdop(in_hdop)
      , altitude(in_altitude)
      , geoidHeight(geoid_height)
      , timeSinceLastDgpsValid(false)
      , dgpdStationIDValid(false)
  {
  }

  inline void SetTimeSinceLastDgps(double const time_since_last_dgps)
  {
    this->timeSinceLastDgpsValid = true;
    this->timeSinceLastDgps = time_since_last_dgps;
  }

  inline void SetDgpsStationID(uint16_t const dgps_station_id)
  {
    this->dgpdStationIDValid = true;
    this->dgpdStationID = dgps_station_id;
  }

  bool valid;
  double timestamp;
  double latitude;
  double longitude;
  FixQuality fixQuality;
  uint16_t numSatellites;
  double hdop;
  double altitude;
  double geoidHeight;
  bool timeSinceLastDgpsValid;
  double timeSinceLastDgps;
  bool dgpdStationIDValid;
  uint16_t dgpdStationID;
};

struct VtgMessageData
{
  inline VtgMessageData()
      : valid(false)
  {
  }

  inline VtgMessageData(double const true_track_made_good,
                        bool const magnetic_track_made_good_valid,
                        double const magnetic_track_made_good,
                        double const ground_speed_knots,
                        double const ground_speed_kph)
      : valid(true)
      , trueTrackMadeGood(true_track_made_good)
      , magneticTrackMadeGoodValid(magnetic_track_made_good_valid)
      , magneticTrackMadeGood(magnetic_track_made_good)
      , groundSpeedKnots(ground_speed_knots)
      , groundSpeedKph(ground_speed_kph)
  {
  }

  inline void SetMagneticTrackMadeGood(double const magnetic_track_made_good)
  {
    this->magneticTrackMadeGoodValid = true;
    this->magneticTrackMadeGood = magnetic_track_made_good;
  }

  bool valid;
  double trueTrackMadeGood;
  bool magneticTrackMadeGoodValid;
  double magneticTrackMadeGood;
  double groundSpeedKnots;
  double groundSpeedKph;
};

AvrMessageData parse_avr(std::string const &message);
GgaMessageData parse_gga(std::string const &message);
VtgMessageData parse_vtg(std::string const &message);
