// $Id: bigint.h,v 1.25 2015-07-01 18:59:12-07 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

#include "debug.h"
#include "rel_ops.h"
#include "ubigint.h"

class bigint {
   friend ostream& operator<< (ostream&, const bigint&);
   private:
      ubigint uvalue;
      bool is_negative {false};
   public:

      //
      // Extra ctors to make bigints.
      //
      bigint() = default; // Needed or will be suppressed.
      bigint(long);
      bigint (unsigned long);
      bigint (const ubigint&, bool is_negative = false);
      explicit bigint (const string&);
      long to_long() const;

      //
      // Basic add/sub operators.
      //
      bigint operator+() const;
      bigint operator-() const;
      bigint operator+ (const bigint&) const;
      bigint operator- (const bigint&) const;
      bigint operator* (const bigint&) const;
      bigint operator/ (const bigint&) const;
      bigint operator% (const bigint&) const;

      //
      // Comparison operators.
      //
      bool operator== (const bigint&) const;
      bool operator<  (const bigint&) const;
};

bigint pow (const bigint& base, const bigint& exponent);

#endif

