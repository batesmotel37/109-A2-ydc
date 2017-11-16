// $Id: ubigint.h,v 1.6 2015-07-11 22:04:04-07 - - $

#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

#include "debug.h"
#include "rel_ops.h"

class ubigint {
   friend ostream& operator<< (ostream&, const ubigint&);
   private:
      using quot_rem = pair<ubigint,ubigint>;
      using unumber = unsigned long;
      unumber uvalue {};
      using udigit_t=unsigned char;
      using ubigvalue_t=vector<udigit_t>;
      ubigvalue_t ubig_value;
      quot_rem divide (const ubigint&) const;
      void multiply_by_2();
      void divide_by_2();
   public:

      //
      // Extra ctors to make ubigints.
      //
      ubigint() = default; // Need default ctor as well.
      ubigint (unsigned long);
      ubigint (const string&);
      unsigned long to_unsigned_long() const;

      //
      // Basic add/sub operators.
      //
      ubigint operator+ (const ubigint&) const;
      ubigint operator- (const ubigint&) const;

      //
      // Extended operators implemented with add/sub.
      //
      ubigint operator* (const ubigint&) const;
      ubigint operator/ (const ubigint&) const;
      ubigint operator% (const ubigint&) const;

      //
      // Comparison operators.
      //
      bool operator== (const ubigint&) const;
      bool operator<  (const ubigint&) const;

      void trim_zeros();
};

#endif

