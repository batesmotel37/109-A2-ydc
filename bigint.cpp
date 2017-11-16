// $Id: bigint.cpp,v 1.72 2015-07-12 04:57:57-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"

bigint::bigint (unsigned long that): uvalue (that),is_negative(false) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (long that): uvalue(that),is_negative(that<0){}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

/*
 * constructor based on string input
 */
bigint::bigint (const string& that) {
   auto itor = that.cbegin();
   is_negative = false;
   //check if the string has a '_', indicating a negative value
   //otherwise presume the value is positive
   if (itor != that.cend() and *itor == '_') {
      is_negative = true;
      ++itor;
   }
   //process the magnitude of the string in ubigint's constructor
   uvalue = that;
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint bigint::operator+() const {
   return *this;
}

bigint bigint::operator-() const {
   return {uvalue, not is_negative};
}

/*
 * returns the sum of this+that
 */
bigint bigint::operator+ (const bigint& that) const {
   ubigint result;
   /*
 * the value with the greater magnitude gives it's sign to the result
 * if the two are of equal magnitude then the sign is positive 
 * (b/c -0 doesn't exist)
 */
   bool result_negative = is_negative;
   if(uvalue > that.uvalue)
      result_negative = is_negative;
   else if(uvalue < that.uvalue)
      result_negative = that.is_negative;
   else if(is_negative != that.is_negative)
      result_negative = false;
   /*
 * when adding two values, if they have different signs, we are
 * essetially subtracting a number from another
 */
   if(is_negative != that.is_negative){
      //we need to make sure the greater value is the left operand
      //because ubigint::operator- expects the greater value to be on 
      //the left
      if(uvalue > that.uvalue){
         result = uvalue-that.uvalue;
      }
      else{
         result = that.uvalue-uvalue;
      }
   }
   //otherwise we can just add magnitudes normally
   else{
      result = uvalue + that.uvalue;
   }
   //join the magnitude (ubigint) with the sign (is_negative) to
   //seal the deal
   bigint big_result (result,result_negative);
   return big_result;
}

/*
 * returns the difference of this-that
 */
bigint bigint::operator- (const bigint& that) const {
   //a-b=a+(-b)
   bigint result = *this+(-(that));
   return result;
}

/*
 * returns the product of this*that
 */
bigint bigint::operator* (const bigint& that) const {
   //if this and that have the same sign the result with be positive
   //if they have different signs the result will be negative
   bool result_negative = false;
   if(is_negative != that.is_negative)
      result_negative = true;
   bigint result (uvalue * that.uvalue,result_negative);
   return result;
}

/*
 * operator to return the quotient of this/that
 */
bigint bigint::operator/ (const bigint& that) const {
   //division is essentially multiplication so the sign of the quotient
   //is determined in the same way as the operator*
   bool result_negative = false;
   if(is_negative != that.is_negative)
      result_negative = true;
   bigint result (uvalue / that.uvalue,result_negative);
   return result;
}

/*
 * returns the remainder of this/that
 */
bigint bigint::operator% (const bigint& that) const {
   //the remainder of division should be the same sign as its dividend
   //hence we make sure that the remainder of this has the same sign
   //as this
   bigint result (uvalue % that.uvalue,is_negative);
   return result;
}

/*
 * operator to check if two bigints are equal
 */
bool bigint::operator== (const bigint& that) const {
   //two values are equal if they have the same sign and the same 
   //magnitude
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

/*
 * operator to compare bigints
 */
bool bigint::operator< (const bigint& that) const {
   //if this is positive and that is positive then the value with 
   //the greater magnitude is greater
   if(!is_negative){
      if(!that.is_negative)
         return uvalue < that.uvalue;
      //if this is positive and that is negative then we know that
      //this is the greater value
      else
         return false;
   }
   //if this is negative and that is positive then that is greater
   else{
      if(!that.is_negative)
         return true;
      //if both are negative then the value with the LOWER magnitude
      //is the greater value, hence this is the opposite of the
      //previous option where both had the same sign
      else
         return uvalue > that.uvalue;
   }
}

long bigint::to_long() const {
   if (*this <= bigint (numeric_limits<long>::min())
    or *this > bigint (numeric_limits<long>::max()))
               throw range_error ("bigint_to_long: out of range");
   return 0;
}

/*
 * prints contents of bigint to out
 */
ostream& operator<< (ostream& out, const bigint& that) {
   //if the number is negative then we must print "-" to indicate such
   if(that.is_negative)
      out << '-';
   out << that.uvalue;
   return out;
}
