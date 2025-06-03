/*  Copyright 2021 Neurostim Technologies LLC

The following Tables are used by the Adaptive Algorithm, defining the ADA plane.
This is a 32 Threshold X 32 Load Resistances plane. Emprical data has been acquired
at each of the 1024 points computing three values: (PulseDecay, DeliveredI, PulseVmax).
Functions critical to the Adaptive Algorithm: ComputeLoadR(ADAarray, Threshold, PulseDecay),
ComputeThreshold(ADAarray, LoadR, DeliveredI) perform linear Interpolation in this 3D space.
*/

// ThesholdTable has 32 10-bit Thresholds between 10V and 90V, linear
uint16_t ThresholdTable[32] =
{
  106,        // 10.0V
  133,        // 12.5V
  161,        // 15.2V
  188,        // 17.7V
  216,        // 20.4V
  243,        // 22.9V
  270,        // 25.5V
  298,        // 28.1V
  325,        // 30.6V
  352,        // 33.2V
  380,        // 35.8V
  407,        // 38.4V
  434,        // 40.9V
  462,        // 43.6V
  489,        // 46.1V
  517,        // 48.8V
  544,        // 51.3V
  571,        // 53.8V
  599,        // 56.5V
  626,        // 59.0V
  653,        // 61.6V
  681,        // 64.2V
  708,        // 66.8V
  735,        // 69.3V
  763,        // 72.0V
  790,        // 74.5V
  818,        // 77.1V
  845,        // 79.7V
  872,        // 82.2V
  900,        // 84.9V
  927,        // 87.4V
  954,        // 90.0V
}; 

// LoadRtable has 32 10-bit Load Resistances between 500 and 10K Ohms, logarithmic
uint16_t LoadRtable[32] =
{
    51,       //  498.5 Ohms
    56,       //  547.4 Ohms
    62,       //  606.1 Ohms
    68,       //  664.7 Ohms
    75,       //  733.1 Ohms
    83,       //  811.3 Ohms
    91,       //  889.5 Ohms
   100,       //  977.5 Ohms
   111,       // 1085.0 Ohms
   122,       // 1192.6 Ohms
   134,       // 1309.9 Ohms
   148,       // 1446.7 Ohms
   163,       // 1593.4 Ohms
   179,       // 1749.8 Ohms
   197,       // 1925.7 Ohms
   218,       // 2131.0 Ohms
   239,       // 2336.3 Ohms
   260,       // 2541.5 Ohms
   291,       // 2844.6 Ohms
   326,       // 3186.7 Ohms
   350,       // 3421.3 Ohms
   382,       // 3734.1 Ohms
   401,       // 3919.8 Ohms
   431,       // 4213.1 Ohms
   453,       // 4428.2 Ohms
   482,       // 4711.6 Ohms
   507,       // 4956.0 Ohms
   612,       // 5982.4 Ohms
   714,       // 6979.5 Ohms
   815,       // 7966.8 Ohms
   916,       // 8954.1 Ohms
  1018,       // 9951.1 Ohms
}; 

// ADAarray is a 3D space indexed by Threshold, LoadR and ValueSel
// ValueSel selects from three byte values: (PulseDecay, DeliveredI, PulseVmax)
uint8_t ADAarray[32][32][3] =   // indexed by Threshold, LoadR, ValueSel
{
  {    // ADA Table for Threshold  10.0V
   { 13,  32,  24, },      // Values for LoadR  498.5Ohms
   { 12,  30,  24, },      // Values for LoadR  547.4Ohms
   { 11,  28,  24, },      // Values for LoadR  606.1Ohms
   { 11,  26,  24, },      // Values for LoadR  664.7Ohms
   { 10,  25,  24, },      // Values for LoadR  733.1Ohms
   { 10,  23,  25, },      // Values for LoadR  811.3Ohms
   { 10,  21,  25, },      // Values for LoadR  889.5Ohms
   {  8,  20,  25, },      // Values for LoadR  977.5Ohms
   {  8,  19,  25, },      // Values for LoadR  1085.0Ohms
   {  8,  17,  25, },      // Values for LoadR  1192.6Ohms
   {  7,  16,  25, },      // Values for LoadR  1309.9Ohms
   {  7,  14,  25, },      // Values for LoadR  1446.7Ohms
   {  6,  13,  25, },      // Values for LoadR  1593.4Ohms
   {  6,  12,  25, },      // Values for LoadR  1749.8Ohms
   {  6,  11,  25, },      // Values for LoadR  1925.7Ohms
   {  6,  10,  26, },      // Values for LoadR  2131.0Ohms
   {  5,   9,  25, },      // Values for LoadR  2336.3Ohms
   {  5,   9,  25, },      // Values for LoadR  2541.5Ohms
   {  5,   8,  25, },      // Values for LoadR  2844.6Ohms
   {  5,   7,  26, },      // Values for LoadR  3186.7Ohms
   {  5,   7,  25, },      // Values for LoadR  3421.3Ohms
   {  4,   6,  26, },      // Values for LoadR  3734.1Ohms
   {  4,   6,  25, },      // Values for LoadR  3919.8Ohms
   {  4,   6,  26, },      // Values for LoadR  4213.1Ohms
   {  4,   5,  27, },      // Values for LoadR  4428.2Ohms
   {  3,   5,  26, },      // Values for LoadR  4711.6Ohms
   {  3,   5,  26, },      // Values for LoadR  4956.0Ohms
   {  3,   4,  26, },      // Values for LoadR  5982.4Ohms
   {  3,   3,  26, },      // Values for LoadR  6979.5Ohms
   {  3,   3,  26, },      // Values for LoadR  7966.8Ohms
   {  2,   3,  26, },      // Values for LoadR  8954.1Ohms
   {  2,   3,  26, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  12.5V
   { 16,  40,  30, },      // Values for LoadR  498.5Ohms
   { 15,  38,  30, },      // Values for LoadR  547.4Ohms
   { 14,  35,  30, },      // Values for LoadR  606.1Ohms
   { 13,  34,  30, },      // Values for LoadR  664.7Ohms
   { 13,  31,  30, },      // Values for LoadR  733.1Ohms
   { 12,  29,  31, },      // Values for LoadR  811.3Ohms
   { 11,  27,  32, },      // Values for LoadR  889.5Ohms
   { 10,  25,  30, },      // Values for LoadR  977.5Ohms
   { 10,  23,  31, },      // Values for LoadR  1085.0Ohms
   {  9,  21,  31, },      // Values for LoadR  1192.6Ohms
   {  9,  20,  31, },      // Values for LoadR  1309.9Ohms
   {  9,  18,  32, },      // Values for LoadR  1446.7Ohms
   {  8,  17,  31, },      // Values for LoadR  1593.4Ohms
   {  8,  15,  32, },      // Values for LoadR  1749.8Ohms
   {  7,  14,  32, },      // Values for LoadR  1925.7Ohms
   {  7,  13,  33, },      // Values for LoadR  2131.0Ohms
   {  6,  12,  32, },      // Values for LoadR  2336.3Ohms
   {  6,  11,  32, },      // Values for LoadR  2541.5Ohms
   {  6,  10,  33, },      // Values for LoadR  2844.6Ohms
   {  6,   9,  33, },      // Values for LoadR  3186.7Ohms
   {  5,   8,  32, },      // Values for LoadR  3421.3Ohms
   {  5,   8,  33, },      // Values for LoadR  3734.1Ohms
   {  5,   7,  33, },      // Values for LoadR  3919.8Ohms
   {  5,   7,  32, },      // Values for LoadR  4213.1Ohms
   {  5,   7,  32, },      // Values for LoadR  4428.2Ohms
   {  5,   6,  33, },      // Values for LoadR  4711.6Ohms
   {  4,   6,  32, },      // Values for LoadR  4956.0Ohms
   {  4,   5,  32, },      // Values for LoadR  5982.4Ohms
   {  3,   4,  32, },      // Values for LoadR  6979.5Ohms
   {  3,   4,  32, },      // Values for LoadR  7966.8Ohms
   {  2,   3,  32, },      // Values for LoadR  8954.1Ohms
   {  1,   3,  33, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  15.2V
   { 19,  49,  36, },      // Values for LoadR  498.5Ohms
   { 19,  46,  37, },      // Values for LoadR  547.4Ohms
   { 18,  43,  37, },      // Values for LoadR  606.1Ohms
   { 17,  40,  37, },      // Values for LoadR  664.7Ohms
   { 16,  37,  37, },      // Values for LoadR  733.1Ohms
   { 14,  34,  36, },      // Values for LoadR  811.3Ohms
   { 14,  32,  37, },      // Values for LoadR  889.5Ohms
   { 13,  30,  37, },      // Values for LoadR  977.5Ohms
   { 12,  27,  37, },      // Values for LoadR  1085.0Ohms
   { 11,  25,  37, },      // Values for LoadR  1192.6Ohms
   { 11,  24,  38, },      // Values for LoadR  1309.9Ohms
   { 10,  22,  38, },      // Values for LoadR  1446.7Ohms
   { 10,  20,  38, },      // Values for LoadR  1593.4Ohms
   { 10,  19,  39, },      // Values for LoadR  1749.8Ohms
   {  9,  17,  39, },      // Values for LoadR  1925.7Ohms
   {  9,  16,  39, },      // Values for LoadR  2131.0Ohms
   {  8,  14,  38, },      // Values for LoadR  2336.3Ohms
   {  8,  13,  39, },      // Values for LoadR  2541.5Ohms
   {  8,  12,  39, },      // Values for LoadR  2844.6Ohms
   {  7,  11,  39, },      // Values for LoadR  3186.7Ohms
   {  7,  10,  39, },      // Values for LoadR  3421.3Ohms
   {  7,   9,  40, },      // Values for LoadR  3734.1Ohms
   {  6,   9,  39, },      // Values for LoadR  3919.8Ohms
   {  6,   8,  39, },      // Values for LoadR  4213.1Ohms
   {  6,   8,  39, },      // Values for LoadR  4428.2Ohms
   {  5,   8,  39, },      // Values for LoadR  4711.6Ohms
   {  5,   7,  39, },      // Values for LoadR  4956.0Ohms
   {  5,   6,  39, },      // Values for LoadR  5982.4Ohms
   {  4,   5,  39, },      // Values for LoadR  6979.5Ohms
   {  4,   5,  39, },      // Values for LoadR  7966.8Ohms
   {  4,   4,  40, },      // Values for LoadR  8954.1Ohms
   {  4,   4,  39, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  17.7V
   { 24,  56,  43, },      // Values for LoadR  498.5Ohms
   { 22,  53,  43, },      // Values for LoadR  547.4Ohms
   { 21,  49,  43, },      // Values for LoadR  606.1Ohms
   { 20,  46,  43, },      // Values for LoadR  664.7Ohms
   { 19,  43,  43, },      // Values for LoadR  733.1Ohms
   { 18,  40,  44, },      // Values for LoadR  811.3Ohms
   { 17,  38,  44, },      // Values for LoadR  889.5Ohms
   { 16,  35,  44, },      // Values for LoadR  977.5Ohms
   { 16,  32,  45, },      // Values for LoadR  1085.0Ohms
   { 14,  30,  44, },      // Values for LoadR  1192.6Ohms
   { 13,  28,  44, },      // Values for LoadR  1309.9Ohms
   { 13,  26,  45, },      // Values for LoadR  1446.7Ohms
   { 12,  23,  45, },      // Values for LoadR  1593.4Ohms
   { 11,  22,  45, },      // Values for LoadR  1749.8Ohms
   { 10,  20,  45, },      // Values for LoadR  1925.7Ohms
   { 10,  18,  46, },      // Values for LoadR  2131.0Ohms
   {  9,  17,  45, },      // Values for LoadR  2336.3Ohms
   {  8,  16,  46, },      // Values for LoadR  2541.5Ohms
   {  8,  14,  45, },      // Values for LoadR  2844.6Ohms
   {  8,  13,  45, },      // Values for LoadR  3186.7Ohms
   {  7,  12,  45, },      // Values for LoadR  3421.3Ohms
   {  7,  11,  45, },      // Values for LoadR  3734.1Ohms
   {  7,  10,  46, },      // Values for LoadR  3919.8Ohms
   {  7,  10,  46, },      // Values for LoadR  4213.1Ohms
   {  7,   9,  46, },      // Values for LoadR  4428.2Ohms
   {  6,   9,  45, },      // Values for LoadR  4711.6Ohms
   {  5,   8,  45, },      // Values for LoadR  4956.0Ohms
   {  5,   7,  46, },      // Values for LoadR  5982.4Ohms
   {  5,   6,  46, },      // Values for LoadR  6979.5Ohms
   {  5,   5,  46, },      // Values for LoadR  7966.8Ohms
   {  5,   5,  45, },      // Values for LoadR  8954.1Ohms
   {  5,   4,  46, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  20.4V
   { 26,  64,  48, },      // Values for LoadR  498.5Ohms
   { 25,  60,  48, },      // Values for LoadR  547.4Ohms
   { 24,  56,  49, },      // Values for LoadR  606.1Ohms
   { 24,  53,  50, },      // Values for LoadR  664.7Ohms
   { 22,  50,  50, },      // Values for LoadR  733.1Ohms
   { 20,  46,  49, },      // Values for LoadR  811.3Ohms
   { 19,  43,  50, },      // Values for LoadR  889.5Ohms
   { 18,  40,  50, },      // Values for LoadR  977.5Ohms
   { 18,  37,  51, },      // Values for LoadR  1085.0Ohms
   { 16,  34,  50, },      // Values for LoadR  1192.6Ohms
   { 15,  32,  51, },      // Values for LoadR  1309.9Ohms
   { 15,  29,  52, },      // Values for LoadR  1446.7Ohms
   { 14,  27,  52, },      // Values for LoadR  1593.4Ohms
   { 12,  25,  51, },      // Values for LoadR  1749.8Ohms
   { 12,  23,  51, },      // Values for LoadR  1925.7Ohms
   { 11,  21,  52, },      // Values for LoadR  2131.0Ohms
   { 10,  19,  52, },      // Values for LoadR  2336.3Ohms
   {  9,  18,  52, },      // Values for LoadR  2541.5Ohms
   {  9,  16,  52, },      // Values for LoadR  2844.6Ohms
   {  9,  15,  52, },      // Values for LoadR  3186.7Ohms
   {  9,  14,  52, },      // Values for LoadR  3421.3Ohms
   {  8,  13,  53, },      // Values for LoadR  3734.1Ohms
   {  8,  12,  53, },      // Values for LoadR  3919.8Ohms
   {  8,  11,  52, },      // Values for LoadR  4213.1Ohms
   {  7,  11,  52, },      // Values for LoadR  4428.2Ohms
   {  7,  10,  53, },      // Values for LoadR  4711.6Ohms
   {  7,  10,  52, },      // Values for LoadR  4956.0Ohms
   {  6,   8,  52, },      // Values for LoadR  5982.4Ohms
   {  6,   7,  53, },      // Values for LoadR  6979.5Ohms
   {  6,   6,  53, },      // Values for LoadR  7966.8Ohms
   {  6,   6,  53, },      // Values for LoadR  8954.1Ohms
   {  6,   5,  53, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  22.9V
   { 31,  72,  55, },      // Values for LoadR  498.5Ohms
   { 29,  68,  55, },      // Values for LoadR  547.4Ohms
   { 27,  63,  55, },      // Values for LoadR  606.1Ohms
   { 26,  59,  55, },      // Values for LoadR  664.7Ohms
   { 25,  56,  56, },      // Values for LoadR  733.1Ohms
   { 23,  52,  56, },      // Values for LoadR  811.3Ohms
   { 21,  48,  56, },      // Values for LoadR  889.5Ohms
   { 20,  45,  56, },      // Values for LoadR  977.5Ohms
   { 19,  41,  56, },      // Values for LoadR  1085.0Ohms
   { 18,  39,  57, },      // Values for LoadR  1192.6Ohms
   { 17,  35,  57, },      // Values for LoadR  1309.9Ohms
   { 16,  33,  57, },      // Values for LoadR  1446.7Ohms
   { 15,  30,  57, },      // Values for LoadR  1593.4Ohms
   { 14,  28,  57, },      // Values for LoadR  1749.8Ohms
   { 13,  26,  58, },      // Values for LoadR  1925.7Ohms
   { 13,  23,  58, },      // Values for LoadR  2131.0Ohms
   { 12,  22,  58, },      // Values for LoadR  2336.3Ohms
   { 11,  20,  58, },      // Values for LoadR  2541.5Ohms
   { 11,  18,  58, },      // Values for LoadR  2844.6Ohms
   { 10,  16,  58, },      // Values for LoadR  3186.7Ohms
   {  9,  15,  59, },      // Values for LoadR  3421.3Ohms
   {  9,  14,  59, },      // Values for LoadR  3734.1Ohms
   {  9,  13,  59, },      // Values for LoadR  3919.8Ohms
   {  9,  13,  59, },      // Values for LoadR  4213.1Ohms
   {  9,  12,  59, },      // Values for LoadR  4428.2Ohms
   {  8,  11,  59, },      // Values for LoadR  4711.6Ohms
   {  8,  11,  59, },      // Values for LoadR  4956.0Ohms
   {  7,   9,  59, },      // Values for LoadR  5982.4Ohms
   {  7,   8,  59, },      // Values for LoadR  6979.5Ohms
   {  6,   7,  59, },      // Values for LoadR  7966.8Ohms
   {  6,   6,  59, },      // Values for LoadR  8954.1Ohms
   {  6,   6,  59, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  25.5V
   { 35,  79,  61, },      // Values for LoadR  498.5Ohms
   { 33,  75,  61, },      // Values for LoadR  547.4Ohms
   { 31,  70,  61, },      // Values for LoadR  606.1Ohms
   { 29,  65,  61, },      // Values for LoadR  664.7Ohms
   { 28,  61,  62, },      // Values for LoadR  733.1Ohms
   { 26,  57,  62, },      // Values for LoadR  811.3Ohms
   { 24,  53,  62, },      // Values for LoadR  889.5Ohms
   { 23,  49,  62, },      // Values for LoadR  977.5Ohms
   { 22,  46,  63, },      // Values for LoadR  1085.0Ohms
   { 21,  42,  63, },      // Values for LoadR  1192.6Ohms
   { 20,  40,  64, },      // Values for LoadR  1309.9Ohms
   { 18,  37,  64, },      // Values for LoadR  1446.7Ohms
   { 18,  33,  64, },      // Values for LoadR  1593.4Ohms
   { 16,  31,  64, },      // Values for LoadR  1749.8Ohms
   { 15,  28,  64, },      // Values for LoadR  1925.7Ohms
   { 14,  26,  64, },      // Values for LoadR  2131.0Ohms
   { 13,  24,  65, },      // Values for LoadR  2336.3Ohms
   { 12,  22,  65, },      // Values for LoadR  2541.5Ohms
   { 11,  20,  65, },      // Values for LoadR  2844.6Ohms
   { 11,  18,  65, },      // Values for LoadR  3186.7Ohms
   {  9,  17,  65, },      // Values for LoadR  3421.3Ohms
   {  9,  16,  66, },      // Values for LoadR  3734.1Ohms
   {  9,  15,  65, },      // Values for LoadR  3919.8Ohms
   {  9,  14,  65, },      // Values for LoadR  4213.1Ohms
   {  9,  13,  66, },      // Values for LoadR  4428.2Ohms
   {  9,  13,  66, },      // Values for LoadR  4711.6Ohms
   {  9,  12,  66, },      // Values for LoadR  4956.0Ohms
   {  9,  10,  66, },      // Values for LoadR  5982.4Ohms
   {  9,   9,  65, },      // Values for LoadR  6979.5Ohms
   {  8,   8,  66, },      // Values for LoadR  7966.8Ohms
   {  8,   7,  66, },      // Values for LoadR  8954.1Ohms
   {  7,   6,  66, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  28.1V
   { 37,  86,  66, },      // Values for LoadR  498.5Ohms
   { 36,  81,  66, },      // Values for LoadR  547.4Ohms
   { 34,  76,  67, },      // Values for LoadR  606.1Ohms
   { 32,  72,  67, },      // Values for LoadR  664.7Ohms
   { 31,  67,  68, },      // Values for LoadR  733.1Ohms
   { 29,  63,  68, },      // Values for LoadR  811.3Ohms
   { 28,  58,  69, },      // Values for LoadR  889.5Ohms
   { 26,  55,  69, },      // Values for LoadR  977.5Ohms
   { 24,  50,  69, },      // Values for LoadR  1085.0Ohms
   { 24,  47,  70, },      // Values for LoadR  1192.6Ohms
   { 22,  43,  70, },      // Values for LoadR  1309.9Ohms
   { 20,  40,  70, },      // Values for LoadR  1446.7Ohms
   { 19,  37,  70, },      // Values for LoadR  1593.4Ohms
   { 19,  34,  71, },      // Values for LoadR  1749.8Ohms
   { 17,  31,  71, },      // Values for LoadR  1925.7Ohms
   { 16,  29,  71, },      // Values for LoadR  2131.0Ohms
   { 15,  26,  71, },      // Values for LoadR  2336.3Ohms
   { 13,  24,  72, },      // Values for LoadR  2541.5Ohms
   { 12,  22,  71, },      // Values for LoadR  2844.6Ohms
   { 12,  20,  72, },      // Values for LoadR  3186.7Ohms
   { 10,  19,  72, },      // Values for LoadR  3421.3Ohms
   { 10,  17,  71, },      // Values for LoadR  3734.1Ohms
   { 10,  16,  72, },      // Values for LoadR  3919.8Ohms
   {  9,  15,  72, },      // Values for LoadR  4213.1Ohms
   {  9,  15,  72, },      // Values for LoadR  4428.2Ohms
   {  9,  14,  72, },      // Values for LoadR  4711.6Ohms
   {  9,  13,  72, },      // Values for LoadR  4956.0Ohms
   {  9,  11,  73, },      // Values for LoadR  5982.4Ohms
   {  9,  10,  73, },      // Values for LoadR  6979.5Ohms
   {  9,   8,  73, },      // Values for LoadR  7966.8Ohms
   {  8,   8,  72, },      // Values for LoadR  8954.1Ohms
   {  8,   7,  72, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  30.6V
   { 41,  93,  72, },      // Values for LoadR  498.5Ohms
   { 40,  88,  73, },      // Values for LoadR  547.4Ohms
   { 38,  83,  74, },      // Values for LoadR  606.1Ohms
   { 36,  78,  74, },      // Values for LoadR  664.7Ohms
   { 35,  73,  75, },      // Values for LoadR  733.1Ohms
   { 32,  68,  74, },      // Values for LoadR  811.3Ohms
   { 31,  63,  75, },      // Values for LoadR  889.5Ohms
   { 30,  59,  76, },      // Values for LoadR  977.5Ohms
   { 27,  54,  75, },      // Values for LoadR  1085.0Ohms
   { 26,  51,  76, },      // Values for LoadR  1192.6Ohms
   { 24,  46,  76, },      // Values for LoadR  1309.9Ohms
   { 23,  43,  77, },      // Values for LoadR  1446.7Ohms
   { 22,  39,  77, },      // Values for LoadR  1593.4Ohms
   { 21,  37,  78, },      // Values for LoadR  1749.8Ohms
   { 19,  34,  77, },      // Values for LoadR  1925.7Ohms
   { 19,  31,  78, },      // Values for LoadR  2131.0Ohms
   { 17,  28,  77, },      // Values for LoadR  2336.3Ohms
   { 15,  26,  77, },      // Values for LoadR  2541.5Ohms
   { 15,  24,  78, },      // Values for LoadR  2844.6Ohms
   { 15,  22,  78, },      // Values for LoadR  3186.7Ohms
   { 14,  20,  77, },      // Values for LoadR  3421.3Ohms
   { 14,  19,  78, },      // Values for LoadR  3734.1Ohms
   { 14,  18,  79, },      // Values for LoadR  3919.8Ohms
   { 13,  17,  79, },      // Values for LoadR  4213.1Ohms
   { 12,  16,  78, },      // Values for LoadR  4428.2Ohms
   { 10,  15,  79, },      // Values for LoadR  4711.6Ohms
   { 10,  14,  79, },      // Values for LoadR  4956.0Ohms
   { 10,  12,  79, },      // Values for LoadR  5982.4Ohms
   { 10,  10,  79, },      // Values for LoadR  6979.5Ohms
   { 10,   9,  79, },      // Values for LoadR  7966.8Ohms
   {  8,   8,  78, },      // Values for LoadR  8954.1Ohms
   {  8,   7,  79, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  33.2V
   { 46, 100,  78, },      // Values for LoadR  498.5Ohms
   { 44,  94,  79, },      // Values for LoadR  547.4Ohms
   { 42,  89,  79, },      // Values for LoadR  606.1Ohms
   { 40,  84,  80, },      // Values for LoadR  664.7Ohms
   { 38,  79,  81, },      // Values for LoadR  733.1Ohms
   { 36,  73,  81, },      // Values for LoadR  811.3Ohms
   { 34,  69,  82, },      // Values for LoadR  889.5Ohms
   { 32,  64,  82, },      // Values for LoadR  977.5Ohms
   { 30,  58,  82, },      // Values for LoadR  1085.0Ohms
   { 28,  54,  82, },      // Values for LoadR  1192.6Ohms
   { 27,  51,  83, },      // Values for LoadR  1309.9Ohms
   { 25,  47,  83, },      // Values for LoadR  1446.7Ohms
   { 24,  43,  83, },      // Values for LoadR  1593.4Ohms
   { 23,  40,  84, },      // Values for LoadR  1749.8Ohms
   { 22,  37,  84, },      // Values for LoadR  1925.7Ohms
   { 19,  33,  83, },      // Values for LoadR  2131.0Ohms
   { 19,  31,  84, },      // Values for LoadR  2336.3Ohms
   { 18,  29,  85, },      // Values for LoadR  2541.5Ohms
   { 17,  26,  84, },      // Values for LoadR  2844.6Ohms
   { 16,  23,  84, },      // Values for LoadR  3186.7Ohms
   { 15,  22,  84, },      // Values for LoadR  3421.3Ohms
   { 15,  20,  84, },      // Values for LoadR  3734.1Ohms
   { 15,  19,  84, },      // Values for LoadR  3919.8Ohms
   { 14,  18,  85, },      // Values for LoadR  4213.1Ohms
   { 14,  17,  85, },      // Values for LoadR  4428.2Ohms
   { 13,  16,  85, },      // Values for LoadR  4711.6Ohms
   { 13,  15,  85, },      // Values for LoadR  4956.0Ohms
   { 12,  13,  86, },      // Values for LoadR  5982.4Ohms
   { 11,  11,  86, },      // Values for LoadR  6979.5Ohms
   { 10,  10,  85, },      // Values for LoadR  7966.8Ohms
   { 10,   9,  86, },      // Values for LoadR  8954.1Ohms
   {  9,   8,  85, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  35.8V
   { 50, 107,  84, },      // Values for LoadR  498.5Ohms
   { 48, 101,  84, },      // Values for LoadR  547.4Ohms
   { 46,  94,  85, },      // Values for LoadR  606.1Ohms
   { 44,  89,  86, },      // Values for LoadR  664.7Ohms
   { 41,  83,  86, },      // Values for LoadR  733.1Ohms
   { 39,  78,  87, },      // Values for LoadR  811.3Ohms
   { 37,  72,  87, },      // Values for LoadR  889.5Ohms
   { 35,  68,  88, },      // Values for LoadR  977.5Ohms
   { 34,  63,  89, },      // Values for LoadR  1085.0Ohms
   { 31,  58,  88, },      // Values for LoadR  1192.6Ohms
   { 29,  54,  89, },      // Values for LoadR  1309.9Ohms
   { 27,  49,  89, },      // Values for LoadR  1446.7Ohms
   { 27,  46,  90, },      // Values for LoadR  1593.4Ohms
   { 25,  43,  90, },      // Values for LoadR  1749.8Ohms
   { 24,  39,  90, },      // Values for LoadR  1925.7Ohms
   { 22,  36,  91, },      // Values for LoadR  2131.0Ohms
   { 19,  33,  91, },      // Values for LoadR  2336.3Ohms
   { 19,  30,  90, },      // Values for LoadR  2541.5Ohms
   { 18,  28,  91, },      // Values for LoadR  2844.6Ohms
   { 18,  25,  91, },      // Values for LoadR  3186.7Ohms
   { 16,  23,  91, },      // Values for LoadR  3421.3Ohms
   { 16,  22,  91, },      // Values for LoadR  3734.1Ohms
   { 16,  21,  91, },      // Values for LoadR  3919.8Ohms
   { 15,  19,  91, },      // Values for LoadR  4213.1Ohms
   { 14,  18,  91, },      // Values for LoadR  4428.2Ohms
   { 14,  17,  91, },      // Values for LoadR  4711.6Ohms
   { 14,  16,  91, },      // Values for LoadR  4956.0Ohms
   { 12,  14,  91, },      // Values for LoadR  5982.4Ohms
   { 11,  12,  91, },      // Values for LoadR  6979.5Ohms
   { 11,  11,  92, },      // Values for LoadR  7966.8Ohms
   { 11,   9,  92, },      // Values for LoadR  8954.1Ohms
   { 10,   9,  92, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  38.4V
   { 60, 110,  89, },      // Values for LoadR  498.5Ohms
   { 60, 105,  91, },      // Values for LoadR  547.4Ohms
   { 53,  99,  91, },      // Values for LoadR  606.1Ohms
   { 47,  94,  91, },      // Values for LoadR  664.7Ohms
   { 44,  88,  91, },      // Values for LoadR  733.1Ohms
   { 42,  81,  92, },      // Values for LoadR  811.3Ohms
   { 40,  76,  92, },      // Values for LoadR  889.5Ohms
   { 38,  72,  93, },      // Values for LoadR  977.5Ohms
   { 35,  66,  93, },      // Values for LoadR  1085.0Ohms
   { 33,  61,  93, },      // Values for LoadR  1192.6Ohms
   { 32,  57,  95, },      // Values for LoadR  1309.9Ohms
   { 30,  53,  95, },      // Values for LoadR  1446.7Ohms
   { 29,  48,  95, },      // Values for LoadR  1593.4Ohms
   { 27,  45,  96, },      // Values for LoadR  1749.8Ohms
   { 25,  42,  96, },      // Values for LoadR  1925.7Ohms
   { 24,  38,  96, },      // Values for LoadR  2131.0Ohms
   { 22,  35,  95, },      // Values for LoadR  2336.3Ohms
   { 20,  32,  96, },      // Values for LoadR  2541.5Ohms
   { 19,  29,  97, },      // Values for LoadR  2844.6Ohms
   { 19,  26,  96, },      // Values for LoadR  3186.7Ohms
   { 18,  25,  96, },      // Values for LoadR  3421.3Ohms
   { 18,  23,  97, },      // Values for LoadR  3734.1Ohms
   { 18,  22,  97, },      // Values for LoadR  3919.8Ohms
   { 17,  20,  97, },      // Values for LoadR  4213.1Ohms
   { 16,  20,  97, },      // Values for LoadR  4428.2Ohms
   { 15,  18,  97, },      // Values for LoadR  4711.6Ohms
   { 15,  18,  98, },      // Values for LoadR  4956.0Ohms
   { 13,  15,  98, },      // Values for LoadR  5982.4Ohms
   { 13,  13,  99, },      // Values for LoadR  6979.5Ohms
   { 12,  11,  98, },      // Values for LoadR  7966.8Ohms
   { 11,  10,  98, },      // Values for LoadR  8954.1Ohms
   { 11,   9, 100, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  40.9V
   { 62, 118,  95, },      // Values for LoadR  498.5Ohms
   { 61, 112,  96, },      // Values for LoadR  547.4Ohms
   { 54, 105,  97, },      // Values for LoadR  606.1Ohms
   { 52, 100,  98, },      // Values for LoadR  664.7Ohms
   { 50,  93,  99, },      // Values for LoadR  733.1Ohms
   { 47,  87,  99, },      // Values for LoadR  811.3Ohms
   { 45,  82, 100, },      // Values for LoadR  889.5Ohms
   { 43,  76, 100, },      // Values for LoadR  977.5Ohms
   { 40,  71, 101, },      // Values for LoadR  1085.0Ohms
   { 38,  65, 102, },      // Values for LoadR  1192.6Ohms
   { 35,  61, 101, },      // Values for LoadR  1309.9Ohms
   { 32,  56, 101, },      // Values for LoadR  1446.7Ohms
   { 31,  52, 102, },      // Values for LoadR  1593.4Ohms
   { 29,  48, 102, },      // Values for LoadR  1749.8Ohms
   { 28,  44, 103, },      // Values for LoadR  1925.7Ohms
   { 27,  41, 104, },      // Values for LoadR  2131.0Ohms
   { 25,  37, 103, },      // Values for LoadR  2336.3Ohms
   { 24,  35, 103, },      // Values for LoadR  2541.5Ohms
   { 23,  32, 104, },      // Values for LoadR  2844.6Ohms
   { 22,  28, 104, },      // Values for LoadR  3186.7Ohms
   { 21,  27, 104, },      // Values for LoadR  3421.3Ohms
   { 20,  25, 104, },      // Values for LoadR  3734.1Ohms
   { 19,  23, 104, },      // Values for LoadR  3919.8Ohms
   { 18,  22, 104, },      // Values for LoadR  4213.1Ohms
   { 17,  21, 104, },      // Values for LoadR  4428.2Ohms
   { 16,  20, 105, },      // Values for LoadR  4711.6Ohms
   { 15,  19, 105, },      // Values for LoadR  4956.0Ohms
   { 14,  16, 104, },      // Values for LoadR  5982.4Ohms
   { 13,  14, 105, },      // Values for LoadR  6979.5Ohms
   { 12,  12, 105, },      // Values for LoadR  7966.8Ohms
   { 11,  11, 105, },      // Values for LoadR  8954.1Ohms
   { 11,  10, 105, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  43.6V0
   { 64, 125, 102, },      // Values for LoadR  498.5Ohms
   { 62, 119, 103, },      // Values for LoadR  547.4Ohms
   { 59, 112, 104, },      // Values for LoadR  606.1Ohms
   { 56, 105, 104, },      // Values for LoadR  664.7Ohms
   { 54,  99, 105, },      // Values for LoadR  733.1Ohms
   { 51,  92, 105, },      // Values for LoadR  811.3Ohms
   { 50,  86, 106, },      // Values for LoadR  889.5Ohms
   { 48,  80, 107, },      // Values for LoadR  977.5Ohms
   { 43,  74, 107, },      // Values for LoadR  1085.0Ohms
   { 41,  69, 108, },      // Values for LoadR  1192.6Ohms
   { 38,  65, 108, },      // Values for LoadR  1309.9Ohms
   { 36,  59, 108, },      // Values for LoadR  1446.7Ohms
   { 36,  55, 109, },      // Values for LoadR  1593.4Ohms
   { 36,  48, 109, },      // Values for LoadR  1749.8Ohms
   { 36,  47, 109, },      // Values for LoadR  1925.7Ohms
   { 34,  43, 110, },      // Values for LoadR  2131.0Ohms
   { 27,  39, 111, },      // Values for LoadR  2336.3Ohms
   { 26,  37, 110, },      // Values for LoadR  2541.5Ohms
   { 25,  33, 110, },      // Values for LoadR  2844.6Ohms
   { 24,  30, 111, },      // Values for LoadR  3186.7Ohms
   { 23,  28, 111, },      // Values for LoadR  3421.3Ohms
   { 22,  26, 112, },      // Values for LoadR  3734.1Ohms
   { 21,  25, 111, },      // Values for LoadR  3919.8Ohms
   { 20,  23, 111, },      // Values for LoadR  4213.1Ohms
   { 19,  22, 111, },      // Values for LoadR  4428.2Ohms
   { 18,  21, 111, },      // Values for LoadR  4711.6Ohms
   { 18,  20, 111, },      // Values for LoadR  4956.0Ohms
   { 17,  17, 112, },      // Values for LoadR  5982.4Ohms
   { 16,  14, 112, },      // Values for LoadR  6979.5Ohms
   { 15,  13, 112, },      // Values for LoadR  7966.8Ohms
   { 15,  11, 113, },      // Values for LoadR  8954.1Ohms
   { 14,  10, 113, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  46.1V
   { 69, 131, 108, },      // Values for LoadR  498.5Ohms
   { 66, 124, 109, },      // Values for LoadR  547.4Ohms
   { 64, 118, 111, },      // Values for LoadR  606.1Ohms
   { 61, 110, 111, },      // Values for LoadR  664.7Ohms
   { 58, 104, 112, },      // Values for LoadR  733.1Ohms
   { 55,  97, 112, },      // Values for LoadR  811.3Ohms
   { 53,  90, 113, },      // Values for LoadR  889.5Ohms
   { 49,  85, 113, },      // Values for LoadR  977.5Ohms
   { 47,  78, 114, },      // Values for LoadR  1085.0Ohms
   { 44,  73, 114, },      // Values for LoadR  1192.6Ohms
   { 43,  68, 115, },      // Values for LoadR  1309.9Ohms
   { 40,  63, 115, },      // Values for LoadR  1446.7Ohms
   { 38,  55, 115, },      // Values for LoadR  1593.4Ohms
   { 38,  51, 115, },      // Values for LoadR  1749.8Ohms
   { 36,  49, 116, },      // Values for LoadR  1925.7Ohms
   { 34,  45, 116, },      // Values for LoadR  2131.0Ohms
   { 29,  42, 116, },      // Values for LoadR  2336.3Ohms
   { 26,  39, 117, },      // Values for LoadR  2541.5Ohms
   { 26,  35, 117, },      // Values for LoadR  2844.6Ohms
   { 25,  32, 117, },      // Values for LoadR  3186.7Ohms
   { 25,  30, 117, },      // Values for LoadR  3421.3Ohms
   { 25,  27, 117, },      // Values for LoadR  3734.1Ohms
   { 25,  25, 117, },      // Values for LoadR  3919.8Ohms
   { 25,  24, 117, },      // Values for LoadR  4213.1Ohms
   { 25,  23, 118, },      // Values for LoadR  4428.2Ohms
   { 20,  22, 118, },      // Values for LoadR  4711.6Ohms
   { 20,  21, 118, },      // Values for LoadR  4956.0Ohms
   { 19,  17, 119, },      // Values for LoadR  5982.4Ohms
   { 17,  15, 119, },      // Values for LoadR  6979.5Ohms
   { 16,  13, 118, },      // Values for LoadR  7966.8Ohms
   { 16,  12, 119, },      // Values for LoadR  8954.1Ohms
   { 14,  11, 118, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  48.8V
   { 73, 137, 114, },      // Values for LoadR  498.5Ohms
   { 71, 129, 115, },      // Values for LoadR  547.4Ohms
   { 68, 123, 116, },      // Values for LoadR  606.1Ohms
   { 66, 116, 118, },      // Values for LoadR  664.7Ohms
   { 63, 109, 119, },      // Values for LoadR  733.1Ohms
   { 59, 101, 118, },      // Values for LoadR  811.3Ohms
   { 56,  95, 119, },      // Values for LoadR  889.5Ohms
   { 53,  89, 119, },      // Values for LoadR  977.5Ohms
   { 51,  82, 121, },      // Values for LoadR  1085.0Ohms
   { 48,  77, 120, },      // Values for LoadR  1192.6Ohms
   { 45,  71, 121, },      // Values for LoadR  1309.9Ohms
   { 43,  66, 122, },      // Values for LoadR  1446.7Ohms
   { 41,  61, 122, },      // Values for LoadR  1593.4Ohms
   { 38,  56, 122, },      // Values for LoadR  1749.8Ohms
   { 37,  52, 123, },      // Values for LoadR  1925.7Ohms
   { 34,  48, 123, },      // Values for LoadR  2131.0Ohms
   { 31,  44, 123, },      // Values for LoadR  2336.3Ohms
   { 28,  41, 124, },      // Values for LoadR  2541.5Ohms
   { 26,  37, 124, },      // Values for LoadR  2844.6Ohms
   { 25,  33, 124, },      // Values for LoadR  3186.7Ohms
   { 25,  31, 124, },      // Values for LoadR  3421.3Ohms
   { 25,  29, 125, },      // Values for LoadR  3734.1Ohms
   { 25,  27, 124, },      // Values for LoadR  3919.8Ohms
   { 25,  26, 124, },      // Values for LoadR  4213.1Ohms
   { 25,  25, 126, },      // Values for LoadR  4428.2Ohms
   { 22,  23, 125, },      // Values for LoadR  4711.6Ohms
   { 21,  22, 125, },      // Values for LoadR  4956.0Ohms
   { 20,  19, 125, },      // Values for LoadR  5982.4Ohms
   { 18,  16, 125, },      // Values for LoadR  6979.5Ohms
   { 17,  14, 126, },      // Values for LoadR  7966.8Ohms
   { 16,  13, 125, },      // Values for LoadR  8954.1Ohms
   { 16,  12, 125, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  51.3V
   { 79, 141, 120, },      // Values for LoadR  498.5Ohms
   { 76, 134, 121, },      // Values for LoadR  547.4Ohms
   { 73, 128, 123, },      // Values for LoadR  606.1Ohms
   { 70, 120, 123, },      // Values for LoadR  664.7Ohms
   { 66, 112, 123, },      // Values for LoadR  733.1Ohms
   { 63, 106, 125, },      // Values for LoadR  811.3Ohms
   { 60,  99, 125, },      // Values for LoadR  889.5Ohms
   { 57,  93, 126, },      // Values for LoadR  977.5Ohms
   { 54,  85, 126, },      // Values for LoadR  1085.0Ohms
   { 52,  80, 127, },      // Values for LoadR  1192.6Ohms
   { 49,  75, 129, },      // Values for LoadR  1309.9Ohms
   { 46,  69, 129, },      // Values for LoadR  1446.7Ohms
   { 43,  63, 129, },      // Values for LoadR  1593.4Ohms
   { 41,  59, 130, },      // Values for LoadR  1749.8Ohms
   { 40,  54, 131, },      // Values for LoadR  1925.7Ohms
   { 36,  50, 130, },      // Values for LoadR  2131.0Ohms
   { 34,  44, 131, },      // Values for LoadR  2336.3Ohms
   { 30,  42, 131, },      // Values for LoadR  2541.5Ohms
   { 27,  39, 131, },      // Values for LoadR  2844.6Ohms
   { 27,  35, 131, },      // Values for LoadR  3186.7Ohms
   { 26,  33, 131, },      // Values for LoadR  3421.3Ohms
   { 26,  30, 132, },      // Values for LoadR  3734.1Ohms
   { 26,  29, 132, },      // Values for LoadR  3919.8Ohms
   { 25,  27, 132, },      // Values for LoadR  4213.1Ohms
   { 25,  26, 132, },      // Values for LoadR  4428.2Ohms
   { 23,  25, 132, },      // Values for LoadR  4711.6Ohms
   { 22,  23, 132, },      // Values for LoadR  4956.0Ohms
   { 21,  20, 132, },      // Values for LoadR  5982.4Ohms
   { 20,  17, 133, },      // Values for LoadR  6979.5Ohms
   { 20,  15, 134, },      // Values for LoadR  7966.8Ohms
   { 19,  13, 133, },      // Values for LoadR  8954.1Ohms
   { 18,  12, 133, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  53.8V
   { 84, 146, 126, },      // Values for LoadR  498.5Ohms
   { 82, 140, 129, },      // Values for LoadR  547.4Ohms
   { 78, 132, 130, },      // Values for LoadR  606.1Ohms
   { 74, 125, 130, },      // Values for LoadR  664.7Ohms
   { 71, 118, 131, },      // Values for LoadR  733.1Ohms
   { 67, 109, 131, },      // Values for LoadR  811.3Ohms
   { 64, 103, 132, },      // Values for LoadR  889.5Ohms
   { 60,  96, 132, },      // Values for LoadR  977.5Ohms
   { 58,  89, 133, },      // Values for LoadR  1085.0Ohms
   { 54,  84, 134, },      // Values for LoadR  1192.6Ohms
   { 52,  77, 134, },      // Values for LoadR  1309.9Ohms
   { 49,  71, 135, },      // Values for LoadR  1446.7Ohms
   { 48,  66, 136, },      // Values for LoadR  1593.4Ohms
   { 43,  61, 135, },      // Values for LoadR  1749.8Ohms
   { 41,  56, 136, },      // Values for LoadR  1925.7Ohms
   { 39,  52, 136, },      // Values for LoadR  2131.0Ohms
   { 37,  48, 136, },      // Values for LoadR  2336.3Ohms
   { 32,  45, 137, },      // Values for LoadR  2541.5Ohms
   { 28,  40, 138, },      // Values for LoadR  2844.6Ohms
   { 28,  36, 138, },      // Values for LoadR  3186.7Ohms
   { 28,  34, 138, },      // Values for LoadR  3421.3Ohms
   { 28,  32, 139, },      // Values for LoadR  3734.1Ohms
   { 28,  30, 139, },      // Values for LoadR  3919.8Ohms
   { 27,  28, 139, },      // Values for LoadR  4213.1Ohms
   { 27,  27, 140, },      // Values for LoadR  4428.2Ohms
   { 26,  26, 138, },      // Values for LoadR  4711.6Ohms
   { 26,  24, 139, },      // Values for LoadR  4956.0Ohms
   { 23,  21, 140, },      // Values for LoadR  5982.4Ohms
   { 22,  18, 139, },      // Values for LoadR  6979.5Ohms
   { 22,  16, 139, },      // Values for LoadR  7966.8Ohms
   { 19,  14, 139, },      // Values for LoadR  8954.1Ohms
   { 19,  13, 140, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  56.5V
   { 89, 151, 133, },      // Values for LoadR  498.5Ohms
   { 87, 144, 135, },      // Values for LoadR  547.4Ohms
   { 81, 136, 134, },      // Values for LoadR  606.1Ohms
   { 79, 129, 136, },      // Values for LoadR  664.7Ohms
   { 76, 121, 137, },      // Values for LoadR  733.1Ohms
   { 72, 113, 137, },      // Values for LoadR  811.3Ohms
   { 69, 106, 138, },      // Values for LoadR  889.5Ohms
   { 64, 100, 138, },      // Values for LoadR  977.5Ohms
   { 62,  93, 140, },      // Values for LoadR  1085.0Ohms
   { 58,  86, 140, },      // Values for LoadR  1192.6Ohms
   { 55,  80, 140, },      // Values for LoadR  1309.9Ohms
   { 52,  75, 141, },      // Values for LoadR  1446.7Ohms
   { 49,  69, 141, },      // Values for LoadR  1593.4Ohms
   { 48,  64, 142, },      // Values for LoadR  1749.8Ohms
   { 45,  59, 143, },      // Values for LoadR  1925.7Ohms
   { 42,  54, 143, },      // Values for LoadR  2131.0Ohms
   { 40,  50, 144, },      // Values for LoadR  2336.3Ohms
   { 34,  47, 145, },      // Values for LoadR  2541.5Ohms
   { 29,  42, 144, },      // Values for LoadR  2844.6Ohms
   { 29,  38, 144, },      // Values for LoadR  3186.7Ohms
   { 29,  36, 144, },      // Values for LoadR  3421.3Ohms
   { 29,  33, 145, },      // Values for LoadR  3734.1Ohms
   { 29,  32, 145, },      // Values for LoadR  3919.8Ohms
   { 29,  30, 146, },      // Values for LoadR  4213.1Ohms
   { 28,  28, 145, },      // Values for LoadR  4428.2Ohms
   { 28,  27, 146, },      // Values for LoadR  4711.6Ohms
   { 27,  26, 146, },      // Values for LoadR  4956.0Ohms
   { 25,  21, 145, },      // Values for LoadR  5982.4Ohms
   { 23,  18, 145, },      // Values for LoadR  6979.5Ohms
   { 22,  16, 146, },      // Values for LoadR  7966.8Ohms
   { 21,  15, 146, },      // Values for LoadR  8954.1Ohms
   { 20,  13, 146, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  59.0V
   { 94, 157, 139, },      // Values for LoadR  498.5Ohms
   { 91, 149, 140, },      // Values for LoadR  547.4Ohms
   { 87, 140, 141, },      // Values for LoadR  606.1Ohms
   { 85, 134, 143, },      // Values for LoadR  664.7Ohms
   { 80, 125, 143, },      // Values for LoadR  733.1Ohms
   { 77, 118, 144, },      // Values for LoadR  811.3Ohms
   { 73, 110, 144, },      // Values for LoadR  889.5Ohms
   { 71, 104, 147, },      // Values for LoadR  977.5Ohms
   { 66,  96, 146, },      // Values for LoadR  1085.0Ohms
   { 62,  89, 146, },      // Values for LoadR  1192.6Ohms
   { 59,  84, 147, },      // Values for LoadR  1309.9Ohms
   { 56,  77, 147, },      // Values for LoadR  1446.7Ohms
   { 53,  71, 147, },      // Values for LoadR  1593.4Ohms
   { 51,  66, 148, },      // Values for LoadR  1749.8Ohms
   { 47,  61, 148, },      // Values for LoadR  1925.7Ohms
   { 46,  56, 150, },      // Values for LoadR  2131.0Ohms
   { 43,  52, 150, },      // Values for LoadR  2336.3Ohms
   { 37,  48, 150, },      // Values for LoadR  2541.5Ohms
   { 31,  44, 150, },      // Values for LoadR  2844.6Ohms
   { 31,  40, 151, },      // Values for LoadR  3186.7Ohms
   { 31,  37, 151, },      // Values for LoadR  3421.3Ohms
   { 31,  34, 151, },      // Values for LoadR  3734.1Ohms
   { 31,  33, 151, },      // Values for LoadR  3919.8Ohms
   { 31,  31, 152, },      // Values for LoadR  4213.1Ohms
   { 31,  29, 152, },      // Values for LoadR  4428.2Ohms
   { 30,  28, 151, },      // Values for LoadR  4711.6Ohms
   { 29,  26, 152, },      // Values for LoadR  4956.0Ohms
   { 27,  22, 152, },      // Values for LoadR  5982.4Ohms
   { 26,  19, 152, },      // Values for LoadR  6979.5Ohms
   { 24,  17, 152, },      // Values for LoadR  7966.8Ohms
   { 23,  15, 153, },      // Values for LoadR  8954.1Ohms
   { 23,  14, 153, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  61.6V
   {100, 161, 145, },      // Values for LoadR  498.5Ohms
   { 98, 154, 147, },      // Values for LoadR  547.4Ohms
   { 93, 146, 148, },      // Values for LoadR  606.1Ohms
   { 88, 137, 147, },      // Values for LoadR  664.7Ohms
   { 85, 130, 149, },      // Values for LoadR  733.1Ohms
   { 82, 122, 151, },      // Values for LoadR  811.3Ohms
   { 78, 114, 151, },      // Values for LoadR  889.5Ohms
   { 74, 107, 152, },      // Values for LoadR  977.5Ohms
   { 69,  98, 150, },      // Values for LoadR  1085.0Ohms
   { 67,  93, 153, },      // Values for LoadR  1192.6Ohms
   { 63,  86, 152, },      // Values for LoadR  1309.9Ohms
   { 60,  80, 153, },      // Values for LoadR  1446.7Ohms
   { 56,  74, 154, },      // Values for LoadR  1593.4Ohms
   { 53,  69, 154, },      // Values for LoadR  1749.8Ohms
   { 51,  63, 155, },      // Values for LoadR  1925.7Ohms
   { 49,  58, 156, },      // Values for LoadR  2131.0Ohms
   { 46,  54, 156, },      // Values for LoadR  2336.3Ohms
   { 40,  50, 156, },      // Values for LoadR  2541.5Ohms
   { 39,  46, 157, },      // Values for LoadR  2844.6Ohms
   { 38,  41, 156, },      // Values for LoadR  3186.7Ohms
   { 38,  39, 157, },      // Values for LoadR  3421.3Ohms
   { 35,  36, 157, },      // Values for LoadR  3734.1Ohms
   { 34,  34, 157, },      // Values for LoadR  3919.8Ohms
   { 34,  32, 159, },      // Values for LoadR  4213.1Ohms
   { 33,  31, 158, },      // Values for LoadR  4428.2Ohms
   { 32,  29, 158, },      // Values for LoadR  4711.6Ohms
   { 31,  28, 158, },      // Values for LoadR  4956.0Ohms
   { 30,  23, 159, },      // Values for LoadR  5982.4Ohms
   { 28,  20, 160, },      // Values for LoadR  6979.5Ohms
   { 25,  18, 159, },      // Values for LoadR  7966.8Ohms
   { 25,  16, 159, },      // Values for LoadR  8954.1Ohms
   { 24,  14, 160, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  64.2V
   {106, 166, 151, },      // Values for LoadR  498.5Ohms
   {102, 158, 152, },      // Values for LoadR  547.4Ohms
   { 98, 149, 153, },      // Values for LoadR  606.1Ohms
   { 95, 142, 155, },      // Values for LoadR  664.7Ohms
   { 90, 133, 155, },      // Values for LoadR  733.1Ohms
   { 85, 125, 155, },      // Values for LoadR  811.3Ohms
   { 82, 118, 157, },      // Values for LoadR  889.5Ohms
   { 78, 110, 157, },      // Values for LoadR  977.5Ohms
   { 75, 102, 158, },      // Values for LoadR  1085.0Ohms
   { 70,  95, 158, },      // Values for LoadR  1192.6Ohms
   { 68,  89, 160, },      // Values for LoadR  1309.9Ohms
   { 63,  83, 160, },      // Values for LoadR  1446.7Ohms
   { 61,  77, 161, },      // Values for LoadR  1593.4Ohms
   { 57,  71, 161, },      // Values for LoadR  1749.8Ohms
   { 55,  66, 162, },      // Values for LoadR  1925.7Ohms
   { 52,  61, 163, },      // Values for LoadR  2131.0Ohms
   { 49,  56, 162, },      // Values for LoadR  2336.3Ohms
   { 43,  52, 162, },      // Values for LoadR  2541.5Ohms
   { 39,  47, 161, },      // Values for LoadR  2844.6Ohms
   { 39,  43, 165, },      // Values for LoadR  3186.7Ohms
   { 39,  40, 163, },      // Values for LoadR  3421.3Ohms
   { 38,  37, 164, },      // Values for LoadR  3734.1Ohms
   { 38,  36, 165, },      // Values for LoadR  3919.8Ohms
   { 36,  33, 164, },      // Values for LoadR  4213.1Ohms
   { 35,  32, 163, },      // Values for LoadR  4428.2Ohms
   { 33,  30, 164, },      // Values for LoadR  4711.6Ohms
   { 33,  29, 165, },      // Values for LoadR  4956.0Ohms
   { 31,  24, 164, },      // Values for LoadR  5982.4Ohms
   { 29,  21, 165, },      // Values for LoadR  6979.5Ohms
   { 28,  18, 166, },      // Values for LoadR  7966.8Ohms
   { 27,  16, 166, },      // Values for LoadR  8954.1Ohms
   { 26,  15, 167, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  66.8V
   {112, 170, 158, },      // Values for LoadR  498.5Ohms
   {108, 163, 159, },      // Values for LoadR  547.4Ohms
   {103, 153, 159, },      // Values for LoadR  606.1Ohms
   { 99, 144, 159, },      // Values for LoadR  664.7Ohms
   { 95, 137, 161, },      // Values for LoadR  733.1Ohms
   { 91, 129, 162, },      // Values for LoadR  811.3Ohms
   { 87, 122, 163, },      // Values for LoadR  889.5Ohms
   { 83, 114, 164, },      // Values for LoadR  977.5Ohms
   { 78, 106, 164, },      // Values for LoadR  1085.0Ohms
   { 75,  99, 165, },      // Values for LoadR  1192.6Ohms
   { 72,  92, 166, },      // Values for LoadR  1309.9Ohms
   { 67,  85, 166, },      // Values for LoadR  1446.7Ohms
   { 64,  79, 167, },      // Values for LoadR  1593.4Ohms
   { 62,  73, 168, },      // Values for LoadR  1749.8Ohms
   { 58,  68, 168, },      // Values for LoadR  1925.7Ohms
   { 54,  62, 168, },      // Values for LoadR  2131.0Ohms
   { 52,  58, 169, },      // Values for LoadR  2336.3Ohms
   { 46,  54, 168, },      // Values for LoadR  2541.5Ohms
   { 45,  49, 170, },      // Values for LoadR  2844.6Ohms
   { 44,  44, 170, },      // Values for LoadR  3186.7Ohms
   { 43,  42, 171, },      // Values for LoadR  3421.3Ohms
   { 40,  38, 170, },      // Values for LoadR  3734.1Ohms
   { 40,  37, 170, },      // Values for LoadR  3919.8Ohms
   { 39,  34, 171, },      // Values for LoadR  4213.1Ohms
   { 37,  33, 170, },      // Values for LoadR  4428.2Ohms
   { 37,  31, 171, },      // Values for LoadR  4711.6Ohms
   { 36,  30, 172, },      // Values for LoadR  4956.0Ohms
   { 33,  25, 172, },      // Values for LoadR  5982.4Ohms
   { 31,  22, 172, },      // Values for LoadR  6979.5Ohms
   { 30,  19, 172, },      // Values for LoadR  7966.8Ohms
   { 28,  17, 172, },      // Values for LoadR  8954.1Ohms
   { 27,  15, 172, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  69.3V
   {115, 174, 162, },      // Values for LoadR  498.5Ohms
   {113, 166, 164, },      // Values for LoadR  547.4Ohms
   {108, 157, 165, },      // Values for LoadR  606.1Ohms
   {104, 149, 166, },      // Values for LoadR  664.7Ohms
   { 99, 141, 167, },      // Values for LoadR  733.1Ohms
   { 96, 132, 168, },      // Values for LoadR  811.3Ohms
   { 92, 125, 169, },      // Values for LoadR  889.5Ohms
   { 88, 117, 170, },      // Values for LoadR  977.5Ohms
   { 83, 108, 170, },      // Values for LoadR  1085.0Ohms
   { 79, 102, 172, },      // Values for LoadR  1192.6Ohms
   { 76,  95, 173, },      // Values for LoadR  1309.9Ohms
   { 72,  88, 173, },      // Values for LoadR  1446.7Ohms
   { 68,  82, 174, },      // Values for LoadR  1593.4Ohms
   { 65,  76, 174, },      // Values for LoadR  1749.8Ohms
   { 61,  70, 174, },      // Values for LoadR  1925.7Ohms
   { 59,  65, 176, },      // Values for LoadR  2131.0Ohms
   { 55,  60, 175, },      // Values for LoadR  2336.3Ohms
   { 46,  56, 176, },      // Values for LoadR  2541.5Ohms
   { 45,  51, 177, },      // Values for LoadR  2844.6Ohms
   { 45,  46, 176, },      // Values for LoadR  3186.7Ohms
   { 45,  43, 176, },      // Values for LoadR  3421.3Ohms
   { 44,  40, 177, },      // Values for LoadR  3734.1Ohms
   { 43,  38, 177, },      // Values for LoadR  3919.8Ohms
   { 41,  35, 177, },      // Values for LoadR  4213.1Ohms
   { 40,  34, 177, },      // Values for LoadR  4428.2Ohms
   { 39,  32, 178, },      // Values for LoadR  4711.6Ohms
   { 38,  31, 177, },      // Values for LoadR  4956.0Ohms
   { 35,  26, 178, },      // Values for LoadR  5982.4Ohms
   { 34,  22, 179, },      // Values for LoadR  6979.5Ohms
   { 31,  20, 178, },      // Values for LoadR  7966.8Ohms
   { 30,  18, 178, },      // Values for LoadR  8954.1Ohms
   { 29,  16, 179, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  72.0V
   {123, 178, 169, },      // Values for LoadR  498.5Ohms
   {119, 170, 170, },      // Values for LoadR  547.4Ohms
   {114, 161, 171, },      // Values for LoadR  606.1Ohms
   {110, 153, 172, },      // Values for LoadR  664.7Ohms
   {105, 144, 174, },      // Values for LoadR  733.1Ohms
   {100, 136, 174, },      // Values for LoadR  811.3Ohms
   { 96, 128, 176, },      // Values for LoadR  889.5Ohms
   { 92, 120, 176, },      // Values for LoadR  977.5Ohms
   { 90, 113, 179, },      // Values for LoadR  1085.0Ohms
   { 83, 104, 177, },      // Values for LoadR  1192.6Ohms
   { 79,  98, 178, },      // Values for LoadR  1309.9Ohms
   { 76,  90, 179, },      // Values for LoadR  1446.7Ohms
   { 72,  84, 180, },      // Values for LoadR  1593.4Ohms
   { 69,  78, 181, },      // Values for LoadR  1749.8Ohms
   { 66,  72, 181, },      // Values for LoadR  1925.7Ohms
   { 61,  66, 180, },      // Values for LoadR  2131.0Ohms
   { 59,  61, 181, },      // Values for LoadR  2336.3Ohms
   { 46,  57, 182, },      // Values for LoadR  2541.5Ohms
   { 31,  52, 182, },      // Values for LoadR  2844.6Ohms
   { 35,  47, 183, },      // Values for LoadR  3186.7Ohms
   { 37,  44, 183, },      // Values for LoadR  3421.3Ohms
   { 39,  41, 183, },      // Values for LoadR  3734.1Ohms
   { 40,  39, 183, },      // Values for LoadR  3919.8Ohms
   { 40,  37, 184, },      // Values for LoadR  4213.1Ohms
   { 40,  35, 184, },      // Values for LoadR  4428.2Ohms
   { 40,  33, 184, },      // Values for LoadR  4711.6Ohms
   { 40,  32, 184, },      // Values for LoadR  4956.0Ohms
   { 39,  27, 186, },      // Values for LoadR  5982.4Ohms
   { 36,  23, 185, },      // Values for LoadR  6979.5Ohms
   { 34,  20, 186, },      // Values for LoadR  7966.8Ohms
   { 32,  18, 185, },      // Values for LoadR  8954.1Ohms
   { 30,  16, 185, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  74.5V
   {128, 181, 174, },      // Values for LoadR  498.5Ohms
   {124, 174, 176, },      // Values for LoadR  547.4Ohms
   {119, 165, 177, },      // Values for LoadR  606.1Ohms
   {115, 157, 178, },      // Values for LoadR  664.7Ohms
   {109, 147, 179, },      // Values for LoadR  733.1Ohms
   {104, 139, 179, },      // Values for LoadR  811.3Ohms
   {101, 131, 181, },      // Values for LoadR  889.5Ohms
   { 96, 123, 181, },      // Values for LoadR  977.5Ohms
   { 92, 115, 183, },      // Values for LoadR  1085.0Ohms
   { 88, 107, 184, },      // Values for LoadR  1192.6Ohms
   { 84, 100, 184, },      // Values for LoadR  1309.9Ohms
   { 80,  93, 185, },      // Values for LoadR  1446.7Ohms
   { 76,  86, 186, },      // Values for LoadR  1593.4Ohms
   { 71,  80, 186, },      // Values for LoadR  1749.8Ohms
   { 69,  74, 187, },      // Values for LoadR  1925.7Ohms
   { 65,  68, 187, },      // Values for LoadR  2131.0Ohms
   { 62,  63, 187, },      // Values for LoadR  2336.3Ohms
   { 54,  59, 188, },      // Values for LoadR  2541.5Ohms
   { 54,  54, 189, },      // Values for LoadR  2844.6Ohms
   { 53,  49, 189, },      // Values for LoadR  3186.7Ohms
   { 41,  45, 189, },      // Values for LoadR  3421.3Ohms
   { 41,  42, 189, },      // Values for LoadR  3734.1Ohms
   { 41,  40, 190, },      // Values for LoadR  3919.8Ohms
   { 41,  38, 191, },      // Values for LoadR  4213.1Ohms
   { 41,  36, 191, },      // Values for LoadR  4428.2Ohms
   { 41,  34, 190, },      // Values for LoadR  4711.6Ohms
   { 41,  33, 190, },      // Values for LoadR  4956.0Ohms
   { 40,  27, 191, },      // Values for LoadR  5982.4Ohms
   { 37,  24, 191, },      // Values for LoadR  6979.5Ohms
   { 36,  21, 192, },      // Values for LoadR  7966.8Ohms
   { 34,  19, 191, },      // Values for LoadR  8954.1Ohms
   { 32,  17, 192, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  77.1V
   {133, 186, 180, },      // Values for LoadR  498.5Ohms
   {130, 178, 182, },      // Values for LoadR  547.4Ohms
   {124, 169, 183, },      // Values for LoadR  606.1Ohms
   {120, 160, 184, },      // Values for LoadR  664.7Ohms
   {120, 151, 185, },      // Values for LoadR  733.1Ohms
   {110, 143, 186, },      // Values for LoadR  811.3Ohms
   {107, 135, 188, },      // Values for LoadR  889.5Ohms
   {102, 127, 189, },      // Values for LoadR  977.5Ohms
   { 98, 118, 190, },      // Values for LoadR  1085.0Ohms
   { 92, 109, 190, },      // Values for LoadR  1192.6Ohms
   { 88, 103, 190, },      // Values for LoadR  1309.9Ohms
   { 84,  95, 191, },      // Values for LoadR  1446.7Ohms
   { 80,  89, 192, },      // Values for LoadR  1593.4Ohms
   { 76,  82, 193, },      // Values for LoadR  1749.8Ohms
   { 72,  76, 193, },      // Values for LoadR  1925.7Ohms
   { 69,  70, 194, },      // Values for LoadR  2131.0Ohms
   { 66,  65, 194, },      // Values for LoadR  2336.3Ohms
   { 54,  61, 195, },      // Values for LoadR  2541.5Ohms
   { 54,  55, 195, },      // Values for LoadR  2844.6Ohms
   { 54,  50, 196, },      // Values for LoadR  3186.7Ohms
   { 53,  47, 196, },      // Values for LoadR  3421.3Ohms
   { 52,  43, 196, },      // Values for LoadR  3734.1Ohms
   { 51,  42, 197, },      // Values for LoadR  3919.8Ohms
   { 49,  39, 196, },      // Values for LoadR  4213.1Ohms
   { 48,  37, 197, },      // Values for LoadR  4428.2Ohms
   { 47,  35, 197, },      // Values for LoadR  4711.6Ohms
   { 45,  34, 197, },      // Values for LoadR  4956.0Ohms
   { 42,  28, 198, },      // Values for LoadR  5982.4Ohms
   { 40,  25, 198, },      // Values for LoadR  6979.5Ohms
   { 37,  22, 198, },      // Values for LoadR  7966.8Ohms
   { 36,  19, 199, },      // Values for LoadR  8954.1Ohms
   { 34,  18, 198, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  79.7V
   {139, 190, 186, },      // Values for LoadR  498.5Ohms
   {135, 181, 187, },      // Values for LoadR  547.4Ohms
   {129, 172, 188, },      // Values for LoadR  606.1Ohms
   {125, 164, 190, },      // Values for LoadR  664.7Ohms
   {121, 154, 191, },      // Values for LoadR  733.1Ohms
   {115, 145, 192, },      // Values for LoadR  811.3Ohms
   {112, 137, 194, },      // Values for LoadR  889.5Ohms
   {107, 129, 195, },      // Values for LoadR  977.5Ohms
   {102, 120, 195, },      // Values for LoadR  1085.0Ohms
   { 96, 112, 194, },      // Values for LoadR  1192.6Ohms
   { 93, 106, 197, },      // Values for LoadR  1309.9Ohms
   { 88,  98, 198, },      // Values for LoadR  1446.7Ohms
   { 84,  91, 199, },      // Values for LoadR  1593.4Ohms
   { 79,  85, 198, },      // Values for LoadR  1749.8Ohms
   { 76,  78, 199, },      // Values for LoadR  1925.7Ohms
   { 73,  72, 200, },      // Values for LoadR  2131.0Ohms
   { 69,  67, 201, },      // Values for LoadR  2336.3Ohms
   { 62,  63, 201, },      // Values for LoadR  2541.5Ohms
   { 60,  57, 202, },      // Values for LoadR  2844.6Ohms
   { 59,  51, 202, },      // Values for LoadR  3186.7Ohms
   { 57,  48, 203, },      // Values for LoadR  3421.3Ohms
   { 55,  45, 203, },      // Values for LoadR  3734.1Ohms
   { 54,  43, 203, },      // Values for LoadR  3919.8Ohms
   { 52,  40, 202, },      // Values for LoadR  4213.1Ohms
   { 51,  38, 204, },      // Values for LoadR  4428.2Ohms
   { 49,  35, 203, },      // Values for LoadR  4711.6Ohms
   { 48,  34, 203, },      // Values for LoadR  4956.0Ohms
   { 44,  29, 204, },      // Values for LoadR  5982.4Ohms
   { 41,  25, 204, },      // Values for LoadR  6979.5Ohms
   { 40,  22, 204, },      // Values for LoadR  7966.8Ohms
   { 39,  20, 205, },      // Values for LoadR  8954.1Ohms
   { 37,  18, 205, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  82.2V
   {144, 192, 191, },      // Values for LoadR  498.5Ohms
   {141, 184, 193, },      // Values for LoadR  547.4Ohms
   {134, 175, 193, },      // Values for LoadR  606.1Ohms
   {130, 166, 195, },      // Values for LoadR  664.7Ohms
   {126, 156, 197, },      // Values for LoadR  733.1Ohms
   {121, 149, 198, },      // Values for LoadR  811.3Ohms
   {117, 141, 200, },      // Values for LoadR  889.5Ohms
   {112, 133, 201, },      // Values for LoadR  977.5Ohms
   {106, 123, 201, },      // Values for LoadR  1085.0Ohms
   {101, 115, 201, },      // Values for LoadR  1192.6Ohms
   { 96, 108, 202, },      // Values for LoadR  1309.9Ohms
   { 92, 101, 204, },      // Values for LoadR  1446.7Ohms
   { 88,  93, 204, },      // Values for LoadR  1593.4Ohms
   { 83,  87, 204, },      // Values for LoadR  1749.8Ohms
   { 81,  80, 206, },      // Values for LoadR  1925.7Ohms
   { 77,  74, 207, },      // Values for LoadR  2131.0Ohms
   { 72,  69, 206, },      // Values for LoadR  2336.3Ohms
   { 65,  64, 207, },      // Values for LoadR  2541.5Ohms
   { 60,  58, 206, },      // Values for LoadR  2844.6Ohms
   { 60,  53, 208, },      // Values for LoadR  3186.7Ohms
   { 59,  50, 209, },      // Values for LoadR  3421.3Ohms
   { 58,  46, 209, },      // Values for LoadR  3734.1Ohms
   { 56,  44, 208, },      // Values for LoadR  3919.8Ohms
   { 55,  41, 209, },      // Values for LoadR  4213.1Ohms
   { 55,  39, 210, },      // Values for LoadR  4428.2Ohms
   { 52,  37, 210, },      // Values for LoadR  4711.6Ohms
   { 51,  35, 210, },      // Values for LoadR  4956.0Ohms
   { 47,  30, 210, },      // Values for LoadR  5982.4Ohms
   { 45,  26, 211, },      // Values for LoadR  6979.5Ohms
   { 41,  23, 210, },      // Values for LoadR  7966.8Ohms
   { 39,  21, 210, },      // Values for LoadR  8954.1Ohms
   { 38,  19, 211, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  84.9V
   {144, 192, 191, },      // Values for LoadR  498.5Ohms
   {141, 184, 193, },      // Values for LoadR  547.4Ohms
   {135, 175, 194, },      // Values for LoadR  606.1Ohms
   {130, 166, 195, },      // Values for LoadR  664.7Ohms
   {127, 157, 197, },      // Values for LoadR  733.1Ohms
   {121, 149, 198, },      // Values for LoadR  811.3Ohms
   {117, 141, 200, },      // Values for LoadR  889.5Ohms
   {112, 133, 201, },      // Values for LoadR  977.5Ohms
   {107, 123, 202, },      // Values for LoadR  1085.0Ohms
   {102, 116, 203, },      // Values for LoadR  1192.6Ohms
   { 98, 108, 204, },      // Values for LoadR  1309.9Ohms
   { 94, 101, 206, },      // Values for LoadR  1446.7Ohms
   { 89,  94, 206, },      // Values for LoadR  1593.4Ohms
   { 86,  87, 207, },      // Values for LoadR  1749.8Ohms
   { 81,  81, 208, },      // Values for LoadR  1925.7Ohms
   { 78,  75, 209, },      // Values for LoadR  2131.0Ohms
   { 75,  69, 210, },      // Values for LoadR  2336.3Ohms
   { 68,  65, 210, },      // Values for LoadR  2541.5Ohms
   { 66,  59, 211, },      // Values for LoadR  2844.6Ohms
   { 64,  53, 211, },      // Values for LoadR  3186.7Ohms
   { 62,  50, 212, },      // Values for LoadR  3421.3Ohms
   { 61,  47, 213, },      // Values for LoadR  3734.1Ohms
   { 57,  45, 212, },      // Values for LoadR  3919.8Ohms
   { 56,  42, 213, },      // Values for LoadR  4213.1Ohms
   { 56,  40, 214, },      // Values for LoadR  4428.2Ohms
   { 54,  38, 214, },      // Values for LoadR  4711.6Ohms
   { 52,  36, 214, },      // Values for LoadR  4956.0Ohms
   { 48,  30, 214, },      // Values for LoadR  5982.4Ohms
   { 45,  26, 215, },      // Values for LoadR  6979.5Ohms
   { 43,  23, 216, },      // Values for LoadR  7966.8Ohms
   { 42,  21, 216, },      // Values for LoadR  8954.1Ohms
   { 40,  19, 217, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  87.4V
   {145, 198, 191, },      // Values for LoadR  498.5Ohms
   {141, 184, 193, },      // Values for LoadR  547.4Ohms
   {136, 175, 194, },      // Values for LoadR  606.1Ohms
   {131, 166, 195, },      // Values for LoadR  664.7Ohms
   {127, 157, 197, },      // Values for LoadR  733.1Ohms
   {121, 149, 198, },      // Values for LoadR  811.3Ohms
   {117, 145, 208, },      // Values for LoadR  889.5Ohms
   {113, 137, 211, },      // Values for LoadR  977.5Ohms
   {108, 123, 202, },      // Values for LoadR  1085.0Ohms
   {103, 120, 203, },      // Values for LoadR  1192.6Ohms
   { 99, 108, 204, },      // Values for LoadR  1309.9Ohms
   { 94, 101, 214, },      // Values for LoadR  1446.7Ohms
   { 89,  94, 206, },      // Values for LoadR  1593.4Ohms
   { 85,  87, 207, },      // Values for LoadR  1749.8Ohms
   { 82,  81, 208, },      // Values for LoadR  1925.7Ohms
   { 78,  75, 209, },      // Values for LoadR  2131.0Ohms
   { 75,  69, 218, },      // Values for LoadR  2336.3Ohms
   { 73,  65, 210, },      // Values for LoadR  2541.5Ohms
   { 69,  59, 211, },      // Values for LoadR  2844.6Ohms
   { 65,  53, 212, },      // Values for LoadR  3186.7Ohms
   { 65,  50, 220, },      // Values for LoadR  3421.3Ohms
   { 65,  47, 221, },      // Values for LoadR  3734.1Ohms
   { 58,  45, 212, },      // Values for LoadR  3919.8Ohms
   { 58,  42, 213, },      // Values for LoadR  4213.1Ohms
   { 58,  40, 214, },      // Values for LoadR  4428.2Ohms
   { 58,  38, 222, },      // Values for LoadR  4711.6Ohms
   { 52,  36, 214, },      // Values for LoadR  4956.0Ohms
   { 48,  30, 214, },      // Values for LoadR  5982.4Ohms
   { 46,  26, 215, },      // Values for LoadR  6979.5Ohms
   { 43,  23, 224, },      // Values for LoadR  7966.8Ohms
   { 41,  21, 223, },      // Values for LoadR  8954.1Ohms
   { 40,  19, 224, },      // Values for LoadR  9951.1Ohms
  },
  {    // ADA Table for Threshold  90.0V
   {163, 204, 211, },      // Values for LoadR  498.5Ohms
   {158, 195, 211, },      // Values for LoadR  547.4Ohms
   {158, 186, 213, },      // Values for LoadR  606.1Ohms
   {158, 172, 214, },      // Values for LoadR  664.7Ohms
   {158, 164, 216, },      // Values for LoadR  733.1Ohms
   {154, 149, 217, },      // Values for LoadR  811.3Ohms
   {133, 149, 217, },      // Values for LoadR  889.5Ohms
   {130, 142, 221, },      // Values for LoadR  977.5Ohms
   {124, 123, 220, },      // Values for LoadR  1085.0Ohms
   {123, 123, 221, },      // Values for LoadR  1192.6Ohms
   {122, 108, 223, },      // Values for LoadR  1309.9Ohms
   {121, 101, 223, },      // Values for LoadR  1446.7Ohms
   {117,  96, 224, },      // Values for LoadR  1593.4Ohms
   { 97,  93, 224, },      // Values for LoadR  1749.8Ohms
   { 93,  86, 225, },      // Values for LoadR  1925.7Ohms
   { 89,  80, 226, },      // Values for LoadR  2131.0Ohms
   { 84,  74, 226, },      // Values for LoadR  2336.3Ohms
   { 81,  69, 226, },      // Values for LoadR  2541.5Ohms
   { 77,  63, 228, },      // Values for LoadR  2844.6Ohms
   { 72,  57, 228, },      // Values for LoadR  3186.7Ohms
   { 70,  53, 228, },      // Values for LoadR  3421.3Ohms
   { 68,  50, 229, },      // Values for LoadR  3734.1Ohms
   { 66,  47, 229, },      // Values for LoadR  3919.8Ohms
   { 64,  45, 230, },      // Values for LoadR  4213.1Ohms
   { 62,  42, 228, },      // Values for LoadR  4428.2Ohms
   { 61,  40, 229, },      // Values for LoadR  4711.6Ohms
   { 59,  38, 229, },      // Values for LoadR  4956.0Ohms
   { 55,  32, 229, },      // Values for LoadR  5982.4Ohms
   { 53,  28, 231, },      // Values for LoadR  6979.5Ohms
   { 50,  25, 231, },      // Values for LoadR  7966.8Ohms
   { 47,  22, 230, },      // Values for LoadR  8954.1Ohms
   { 45,  20, 231, },      // Values for LoadR  9951.1Ohms
  },
};
