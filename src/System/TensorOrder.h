#pragma once

#include "Vec.h"

/**
  @brief To get the static dimensionnality of objects

  Meaning of \c res
   - 0 -> scalar
   - 1 -> vector
   - 2 -> matrix
   - ...
   - -1 -> dynamic tensor order (not fixed during the compilation)
*/
template<class T> struct TensorOrder { static const int res = 0; };

template<class T> struct TensorOrder<std::vector<T>> { static const int res = 1; };
template<class T> struct TensorOrder<Vec        <T>> { static const int res = 1; };
