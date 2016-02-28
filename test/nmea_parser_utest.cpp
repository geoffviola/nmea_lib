// Copyright 2016 Geoffrey Lawrence Viola

#include "nmea_parser.hpp"
#include <gtest/gtest.h>

TEST(NmeaParser, parseInvalidAvr)
{
  AvrMessageData const parsed_message(parse_avr("junk"));
  EXPECT_FALSE(parsed_message.valid);
}

TEST(NmeaParser, parseValidAvr)
{
  AvrMessageData const parsed_message(parse_avr(
      "$PTNL,AVR,181059.6,+149.4688,Yaw,+0.0134,Tilt,,,60.191,3,2.5,6*00"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(181059.6, parsed_message.timestamp);
  EXPECT_DOUBLE_EQ(149.4688, parsed_message.yaw);
  EXPECT_DOUBLE_EQ(0.0134, parsed_message.tilt);
  EXPECT_DOUBLE_EQ(60.191, parsed_message.range);
  EXPECT_EQ(AvrMessageData::RTK_FIX, parsed_message.fixQuality);
  EXPECT_DOUBLE_EQ(2.5, parsed_message.pdop);
  EXPECT_EQ(6U, parsed_message.numSatellites);
}

TEST(NmeaParser, parseInvalidGga)
{
  GgaMessageData const parsed_message(parse_gga("junk"));
  EXPECT_FALSE(parsed_message.valid);
}

TEST(NmeaParser, parseValidGgaNoTimeSinceLastDgpsNoDgpsStationIDValid)
{
  GgaMessageData const parsed_message(parse_gga(
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(123519.0, parsed_message.timestamp);
  EXPECT_DOUBLE_EQ(48.1173, parsed_message.latitude);
  EXPECT_DOUBLE_EQ(11.0 + (31.0 / 60.0), parsed_message.longitude);
  EXPECT_EQ(GgaMessageData::GPS_FIX, parsed_message.fixQuality);
  EXPECT_EQ(8U, parsed_message.numSatellites);
  EXPECT_DOUBLE_EQ(0.9, parsed_message.hdop);
  EXPECT_DOUBLE_EQ(545.4, parsed_message.altitude);
  EXPECT_DOUBLE_EQ(46.9, parsed_message.geoidHeight);
  EXPECT_FALSE(parsed_message.timeSinceLastDgpsValid);
  EXPECT_FALSE(parsed_message.dgpdStationIDValid);
}

TEST(NmeaParser, parseValidGgaTimeSinceLastDgpsNoDgpsStationIDValid)
{
  GgaMessageData const parsed_message(parse_gga(
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,5,*47"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(123519.0, parsed_message.timestamp);
  EXPECT_DOUBLE_EQ(48.1173, parsed_message.latitude);
  EXPECT_DOUBLE_EQ(11.0 + (31.0 / 60.0), parsed_message.longitude);
  EXPECT_EQ(GgaMessageData::GPS_FIX, parsed_message.fixQuality);
  EXPECT_EQ(8U, parsed_message.numSatellites);
  EXPECT_DOUBLE_EQ(0.9, parsed_message.hdop);
  EXPECT_DOUBLE_EQ(545.4, parsed_message.altitude);
  EXPECT_DOUBLE_EQ(46.9, parsed_message.geoidHeight);
  EXPECT_TRUE(parsed_message.timeSinceLastDgpsValid);
  EXPECT_DOUBLE_EQ(5.0, parsed_message.timeSinceLastDgps);
  EXPECT_FALSE(parsed_message.dgpdStationIDValid);
}

TEST(NmeaParser, parseValidGgaNoTimeSinceLastDgpsDgpsStationIDValid)
{
  GgaMessageData const parsed_message(parse_gga(
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,0001*47"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(123519.0, parsed_message.timestamp);
  EXPECT_DOUBLE_EQ(48.1173, parsed_message.latitude);
  EXPECT_DOUBLE_EQ(11.0 + (31.0 / 60.0), parsed_message.longitude);
  EXPECT_EQ(GgaMessageData::GPS_FIX, parsed_message.fixQuality);
  EXPECT_EQ(8U, parsed_message.numSatellites);
  EXPECT_DOUBLE_EQ(0.9, parsed_message.hdop);
  EXPECT_DOUBLE_EQ(545.4, parsed_message.altitude);
  EXPECT_DOUBLE_EQ(46.9, parsed_message.geoidHeight);
  EXPECT_FALSE(parsed_message.timeSinceLastDgpsValid);
  EXPECT_TRUE(parsed_message.dgpdStationIDValid);
  EXPECT_EQ(1U, parsed_message.dgpdStationID);
}

TEST(NmeaParser, parseInvalidVtg)
{
  VtgMessageData const parsed_message(parse_vtg("junk"));
  EXPECT_FALSE(parsed_message.valid);
}

TEST(NmeaParser, parseValidVtgMagneticTrackMadeGood)
{
  VtgMessageData const parsed_message(
      parse_vtg("$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(54.7, parsed_message.trueTrackMadeGood);
  EXPECT_TRUE(parsed_message.magneticTrackMadeGoodValid);
  EXPECT_DOUBLE_EQ(34.4, parsed_message.magneticTrackMadeGood);
  EXPECT_DOUBLE_EQ(5.5, parsed_message.groundSpeedKnots);
  EXPECT_DOUBLE_EQ(10.2, parsed_message.groundSpeedKph);
}

TEST(NmeaParser, parseValidVtgNoMagneticTrackMadeGood)
{
  VtgMessageData const parsed_message(
      parse_vtg("$GPVTG,054.7,T,,M,005.5,N,010.2,K*48"));
  EXPECT_TRUE(parsed_message.valid);
  EXPECT_DOUBLE_EQ(54.7, parsed_message.trueTrackMadeGood);
  EXPECT_FALSE(parsed_message.magneticTrackMadeGoodValid);
  EXPECT_DOUBLE_EQ(5.5, parsed_message.groundSpeedKnots);
  EXPECT_DOUBLE_EQ(10.2, parsed_message.groundSpeedKph);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
