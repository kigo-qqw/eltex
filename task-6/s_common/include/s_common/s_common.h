#pragma once

#if __STDC_VERSION__ >= 202311L
  #define NULLPTR nullptr
  #define CONSTEXPR constexpr
#else
  #define NULLPTR NULL
  #define CONSTEXPR
#endif
