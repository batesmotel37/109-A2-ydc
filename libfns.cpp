// $Id: libfns.cpp,v 1.1 2015-07-12 04:51:19-07 - - $

#include "libfns.h"

//
// This algorithm would be more efficient with operators
// *=, /=2, and is_odd.  But we leave it here.
//

//returns the value of base_arg raised to the powre of exponent_arg
//
//function and implmentation provided by Prof. Mackey
//but dependent on implementation of methods I completed in bigint.cpp
//and ubigint.cpp
bigint pow (const bigint& base_arg, const bigint& exponent_arg) {
   bigint base (base_arg);
   bigint exponent (exponent_arg);
   static const bigint ZERO ("0");
   static const bigint ONE ("1");
   static const bigint TWO ("2");
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   if (base == ZERO) return ZERO;
   bigint result = ONE;
   if (exponent < ZERO) {
      base = ONE / base;
      exponent = - exponent;
   }
   while (exponent > ZERO) {
      if (exponent % TWO == ONE) {
         result = result * base;
         exponent = exponent - 1;
      }else {
         base = base * base;
         exponent = exponent / 2;
      }
   }
   DEBUGF ('^', "result = " << result);
   return result;
}

