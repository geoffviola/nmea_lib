// Copyright 2016 Geoffrey Lawrence Viola

#include "gtest/gtest.h"
#include "nmea_builder.hpp"
#include <string>

using std::string;

TEST(NmeaBuilder, builGgaMsgNortheast)
{
  string const output = build_gga(12U, 35U, 19.0, 48.1173, 11.0 + (31.0 / 60.0),
                                  GGA_GPS, 8U, 0.9, 545.4, 46.9);

  EXPECT_EQ(
      "$GPGGA,123519.00,4807.03800000,N,01131.00000000,E,1,8,0.9,545.400,M,"
      "46.9,M,,*59\n",
      output);
}

TEST(NmeaStramGenerator, createGgaMsg_southwest)
{
  string const output = build_gga(0U, 0U, 0.04, -11.0, -22.0, GGA_RTK_FIXED,
                                  16U, 44.4, -33.0, -55.5);

  EXPECT_EQ("$GPGGA,000000.04,1100.00000000,S,"
            "02200.00000000,W,4,16,44.4,-33.000,"
            "M,-55.5,M,,*64\n",
            output);
}

TEST(NmeaStramGenerator, createVtgMsg_10mpsEast)
{
  string const output = build_vtg(90.0, 10.0);

  EXPECT_EQ("$GPVTG,090.0,T,,M,19.438,N,36.000,K*6B\n", output);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
