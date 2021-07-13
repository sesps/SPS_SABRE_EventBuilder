#ifndef COMPASS_HIT_H
#define COMPASS_HIT_H

struct CompassHit {
  UShort_t board = 400;
  UShort_t channel = 400;
  ULong64_t timestamp = 0;
  UShort_t lgate = 0;
  UShort_t sgate = 0;
  UInt_t flags = 0;
  UInt_t Ns = 0;
};

#endif
