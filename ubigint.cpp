// $Id: ubigint.cpp,v 1.15 2015-07-12 05:25:18-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

/*
 * constructor that recieves longs
 */ 
ubigint::ubigint (unsigned long that): uvalue (that) {
   while(that != 0){
      ubig_value.push_back(that%10+'0');
      that/=10;
   }
}

/*
 * constructor that recieves strings
 */
ubigint::ubigint (const string& that) {
   auto itor = that.rbegin();//reverse iterate over the string
   for(; itor != that.rend(); ++itor){
      if(*itor != '_')//we don't want to add the thing which implied
                    //the negative, the ubigint is supposed to be
                    //strictly the magnitude of the value
         ubig_value.push_back(*itor);//append the char from the string
                                     //to the vector
                                     //
                                     //NOTE: b/c of this the lowest
                                     //magnitude digit in the vector
                                     //is at position 0, while the
                                     //most significant is at the end
                                     //so when we want to start from
                                     //the most significant we will 
                                     //reverse iterate
   }
   DEBUGF ('~', this << " -> " << uvalue)
}

/*
 * a function that trims leading zeros from our vectors
 * to help them print out neater and not become stupidly large
 */
void ubigint::trim_zeros(){
   //reverse iterate to start from most significant digit
   for(auto itor = ubig_value.crbegin(); itor != ubig_value.crend(); 
            ++itor){
      if(*itor == '0')//if the digit is a '0' then remove it
         ubig_value.pop_back();
      else
         break;//the moment we encounter our first non-zero, we know we
               //have eliminated all the leading zeros and there's
               //nothing left to trim
   }
   if(ubig_value.size() == 0)//if the value was all '0''s to begin, we
                           //need to make the value '0' so that we
                           //don't end up with an empty vector to
                           //perform math on
      ubig_value.push_back('0');
}

/*
 * returns the sum of this+that
 */
ubigint ubigint::operator+ (const ubigint& that) const {
   ubigvalue_t result;
   //we are going to add our values digit by digit which also
   //necessitates an extra variable to carry overflow
   unsigned char carry = 0;
   unsigned char digit;
   //In order to iterate digit by digit effectively, I need to know
   //which value is larger (it the two have the same number of digits)
   //then it shouldn't matter
   ubigvalue_t longer;
   ubigvalue_t shorter;
   if(ubig_value.size() > that.ubig_value.size()){
      shorter = that.ubig_value;
      longer = ubig_value;
   }
   else{
      shorter = ubig_value;
      longer = that.ubig_value;
   }
   //iterate over both vectors simultainously, starting from the least
   //significant digit
   auto that_itor = longer.begin();
   for(auto this_itor = shorter.begin(); this_itor != shorter.end()
            ; ++this_itor){
      digit = *this_itor+*that_itor-'0'+carry;//add each digit 
                                            //(subtract '0') to get rid
                                            //of the offset created by 
                                            //storing everything in 
                                            //char's, then add the carry
                                            //overflow from the previous
                                            //digit addition
      carry = 0;//reset carry
      //it the value of digit is larger than what can be stored in one
      //digit, we need to adjust for overflow, but decrementing digit
      //and incrementing carry
      if(digit > '9'){//at least 1 greater than '9' so it's 
                    //effectively usigned char {10+}
         digit-= 10;
         carry = 1;
      }
      result.push_back(digit);//push the new digit onto our result 
                              //vector
      ++that_itor;//be sure to iterate over both vectors simultainously
   }
   //since we have finished the shorter vector, we just need to add the
   //remaining values from the longer one. Essentially we are adding
   //these digits to the leading zeros of the shorter vector.
   for(; that_itor != longer.end(); ++that_itor)
   {
      digit = *that_itor+carry;//we need to keep carry around b/c it may
                             //still hold value from the last addition
                             //with shorter
      carry = 0;
      if(digit > '9'){
         digit-= 10;
         carry = 1;
      }
      result.push_back(digit);
   }
   if(carry > 0)//if we happen to have any final carry over, bump up 
              //result accordingly
      result.push_back('1');
   ubigint rtn;
   rtn.ubig_value = result;
   return rtn;//result;
}

/*
 * returns the differnce of this-that
 * PRE: this>=that
 */
ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   ubigint rtn;
   ubigvalue_t result;
   //we create digit and carry in the same way as operator+
   unsigned char carry = '0';//carry is now adjusted to '0' instead of 0
                           //b/c the math is complex enough that I had
                           //to do this
   unsigned char digit;
   //we need to find the greater and lesser value between this and that
   //just in case somehow we can ensure that this > that
   ubigvalue_t longer;
   ubigvalue_t shorter;
   if(*this > that){
      longer = ubig_value;
      shorter = that.ubig_value;
   }
   else{
      longer = that.ubig_value;
      shorter = ubig_value;
   }
   //iterate over both vectors subtracting the digits as we co
   auto that_itor = longer.begin();
   for(auto this_itor = shorter.begin(); this_itor != shorter.end()
            ; ++this_itor){
      digit = *that_itor-*this_itor+carry;
      carry = '0';
      //if the digit resulting from this is less than zero, we will
      //carry a 10 over from the next most significant digit
      //this takes the form of subtracting that digit by 1 when we 
      //come around for the next iteration
      //
      //this is also why we needed to establish longer and shorter
      if(digit < '0'){//at least 1 less than '0' so it's
                    //effectively unsigned char {0-}
         digit+= 10;
         carry-= 1;
      }
      result.push_back(digit);
      ++that_itor;
   }
   //after we finish subtracting the smaller value, continue bringing 
   //down the digits from longer, think of this as subtracting the 
   //leading zeros of shorter from longer
   for(; that_itor != longer.end(); ++that_itor){
      digit = *that_itor+carry-'0';
      carry = '0';
      if(digit < '0'){//we need to keep track of carry after this point
                    //just in case this has a cascading affect on the 
                    //final value
         digit+= 10;
         carry-= 1;
      }
      result.push_back(digit);
   }
   rtn.ubig_value = result;
   rtn.trim_zeros();//trim leading zeros
   return rtn;
}

unsigned long ubigint::to_unsigned_long() const {
   if (*this <= ubigint (numeric_limits<unsigned long>::min())
    or *this > ubigint (numeric_limits<unsigned long>::max()))
               throw range_error ("ubigint_to_unsigned long");
   return uvalue;
}

/*
 * returns the product of this*that
 */
ubigint ubigint::operator* (const ubigint& that) const {
   ubigint rtn;
   ubigvalue_t result;
   //we want to make sure that our result vector can hold as many 
   //digits as the product will have b/c we will be setting the
   //result value through vector.at() instead of appending
   for(size_t i = 0; i < ubig_value.size()+that.ubig_value.size(); ++i)
      result.push_back('0');
   //establish carry and digit
   unsigned char carry = 0;
   unsigned char digit;
   //we are going to insert values into our vector by tracking the
   //position at which the product of two digits in either vector
   //should go, we'll increment these variables along with the iterators
   int this_pos = 0;
   int that_pos = 0;
   //iterate over the vectors starting from the least significant digit
   for(auto this_itor = ubig_value.begin();
            this_itor != ubig_value.end(); ++this_itor){
      that_pos = 0;
      carry = 0;
      //iterate over the second factor in the same way as the first
      //
      //we are goint to multiply a single digit from the first factor
      //by the entrity of the second factor, digit by digit
      //until we have done this for every digit in the first factor
      for(auto that_itor = that.ubig_value.begin();
               that_itor != that.ubig_value.end(); ++that_itor){
         digit = result.at(this_pos+that_pos)+
               (*this_itor-'0')*(*that_itor-'0')+carry;//calculate the 
                                                       //product of the
                                                       //digits of the 
                                                       //two factors 
                                                       //plus the 
                                                       //carry-over from
                                                       //the last 
                                                       //calculation 
                                                       //plus whatever 
                                                       //value already 
                                                       //exists in the 
                                                       //position where 
                                                       //this product 
                                                       //should be 
                                                       //stored
         carry = (digit-'0')/10;//find if there is anything to
                                //carry-over
         result.at(this_pos+that_pos) = ((digit-'0')%10)+'0';//store the
                                                             //digit at 
                                                             //the place
                                                             //in the 
                                                             //result 
                                                             //vector 
                                                             //that it 
                                                             //should be
                                                             //based on 
                                                             //its 
                                                             //factors' 
                                                             //positios
         ++that_pos;
      }
      result.at(this_pos+that.ubig_value.size())+= carry;//always 
                                                         //account
                                                         //for any 
                                                         //leftover 
                                                         //carry-over
      ++this_pos;
   }
   rtn.ubig_value = result;
   rtn.trim_zeros();//trim zeros just in case
   return rtn;
}

/*
 * double this
 */
void ubigint::multiply_by_2() {
   *this = *this*2;
}

/*
 * half this
 * I tried calling this method by saying "*this=*this/2;" the way I had
 * with multiply_by_2(). But this lead to semi-recursive infinite loops
 */
void ubigint::divide_by_2() {
   //create variables for digits and carry-overflow
   unsigned char carry = '0';
   unsigned char digit = '0';
   ubigvalue_t result;
   //reverse iterate over the vector so we start with the most 
   //significant digit
   for(auto itor = ubig_value.rbegin(); itor != ubig_value.rend(); 
            ++itor){
      if(*itor < '2' && carry == '0'){//if there is no carryover and 
                                      //the iterator is pointing to a 
                                      //value thatis to small to be 
                                      //halved, we shift this value over
                                      //to carry-over and append a '0' 
                                      //to our result
         digit = '0';
         carry = *itor;
      }
      else{
         digit = (*itor-'0'+(carry-'0')*10)/2+'0';//we need to subtract 
                                                //'0' because all these
                                                //values are stored as
                                                //char's, not int's
                                                //this way we can do
                                                //arithmetic on them as
                                                //ints, before 
                                                //reconverting to char's
         carry = (*itor-'0'+(carry-'0')*10)%2+'0';
      }
      result.push_back(digit);
   }
   ubigvalue_t reverse;
   //result is actually in the reverse order of what we want,b/c we
   //can only append not prepend with vectors, so we reverse the vector
   //before returning
   while(result.size() > 0){
      reverse.push_back(result.back());
      result.pop_back();
   }
   ubig_value = reverse;
   trim_zeros();//trim zeros before finishing
}

/*
 * returns the quotient and remainder of this/that in the form of a 
 * pair
 *
 * uses the egyptian method of division as explained by Prof. Mackey in
 * /afs/cats.ucsc.edu/courses/cmps109-wm/Assignments/asg2-dc-bigint/
 * misc/division.cpp
 *
 * PRE: that!=0, we cannot divide by 0
 */
ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   if(that.ubig_value.front() == '0' && that.ubig_value.size() == 1)
      throw domain_error ("ubigint::divide: by 0");
   ubigint zero ("0");
   ubigint power_of_2 ("1");
   ubigint divisor = that; // right operand, divisor
   ubigint quotient ("0");
   ubigint remainder = *this; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

/*
 * returns the quotient of this/that using divide()
 */
ubigint ubigint::operator/ (const ubigint& that) const {
   return divide (that).first;
}

/*
 * returns the remainder of this/that using divide()
 */
ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

/*
 * checks if to ubigints are of equal magnitude
 */
bool ubigint::operator== (const ubigint& that) const {
   //if the two bigints are not of the same size, they can't be equal
   if(ubig_value.size() == that.ubig_value.size()){
      //iterate over each vector, if any digits are not equal then
      //the two bigints are not equal, otherwise they are equal
      auto that_itor = that.ubig_value.begin();
      for(auto this_itor = ubig_value.begin(); 
               this_itor != ubig_value.end() 
               && that_itor != that.ubig_value.end(); ++this_itor){
         if(*this_itor != *that_itor)
            return false;
         ++that_itor;
      }
      return true;
   }
   return false;
}

/*
 * compares the magnitudes of two bigints to find out which is less
 */
bool ubigint::operator< (const ubigint& that) const {
   size_t this_size = ubig_value.size();
   size_t that_size = that.ubig_value.size();
   //if the two bigints have different sizes, then the larger size 
   //must obviosuly be the larger value
   if(this_size > that_size)
      return false;
   else if(this_size < that_size)
      return true;
   //if the sizes are the same
   //we reverse iterate over each digit, from most to least significant
   //if one digit is larger than the other, than its bigint is larger
   //if they are equal, we keep iterating
   else{
      auto that_itor = that.ubig_value.rbegin();
      for(auto this_itor = ubig_value.rbegin();
               this_itor != ubig_value.rend()
               && that_itor != that.ubig_value.rend(); ++this_itor){
         if(*this_itor < *that_itor)
            return true;
         else if(*this_itor > *that_itor)
            return false;
         ++that_itor;
      }
      return false;
   }
}

/*
 * prints out the bigint represented by the vector
 */
ostream& operator<< (ostream& out, const ubigint& that) { 
   //since we want the most significant digit first, we reverse iterate
   for(auto itor = that.ubig_value.rbegin();
                  itor != that.ubig_value.rend(); ++itor){
      out << *itor;
   }
   return out;
}
