// Copyright 2016 Geoffrey Lawrence Viola

#ifndef NMEALIB_GGAFIXQUALITY_HPP
#define NMEALIB_GGAFIXQUALITY_HPP

enum GgaFixQuality
{
  GGA_INVALID = 0,
  GGA_GPS,
  GGA_DGPS,
  GGA_PPS,
  GGA_RTK_FIXED,
  GGA_RTK_FLOAT,
  GGA_DEAD_RECKONING,
  GGA_MANUAL_INPUT,
  GGA_SIMULATION
};

#endif // NMEALIB_GGAFIXQUALITY_HPP