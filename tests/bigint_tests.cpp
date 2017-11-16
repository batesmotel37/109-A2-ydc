#include <iostream>
#include "UnitTest++/UnitTest++.h"

#include "bigint.h"
#include <climits>
#include "libfns.h"

using namespace std;
//The Main function that triggers the tests to be run
int main(int, const char *[]){
  cout << "bigint test runner" << endl;
	return UnitTest::RunAllTests();
}

SUITE(bigint_Test_Suite){

  //Check that two basic bigints are the same
    //check basic constructors
  TEST(basic_Comparison){
    bigint test;
    bigint test2;
    CHECK(test == test2);
  }

  //check that two valued bigints are the same
  //check long constructor
  TEST(basic_long_comparison){
   bigint testBigint(5);
   bigint testBigint2(5);
   CHECK(testBigint == testBigint2);
  }

  //check that two different valued bigints are NOT the same
  //check == comparison operator
  TEST(basic_long_comparison_NOT_EQUAL){
   bigint testBigint(5);
   bigint testBigint2(10);
   CHECK(!(testBigint == testBigint2));
  }

  //Check that a bigint made from a string will equal one made from a long
  //check string constructor against long ctor
  TEST(basic_string_instantiation){
    bigint testStringbigint("5");
    bigint testLongbigint(5);
    CHECK(testStringbigint == testLongbigint);
  }

  //check string ctor
  TEST(basic_string_comparison){
    bigint test1("25");
    bigint test2("25");

    CHECK(test1 == test2);
  }

  //Check that a double digit string will equal a double digit long
  //check multiple digit strings
  TEST(basic_string_double_digits){
    bigint testString1("10");
    bigint testLong1(10);
    CHECK(testString1 == testLong1);
  }

  
  //Check system long bounds
  TEST(signed_bounds_check){
    long testLong = LONG_MAX;
    CHECK(testLong == LONG_MAX);
    testLong++; //CAUSES OVERFLOW
    CHECK(testLong == LONG_MIN); //THIS IS TRUE BECAUSE OF OVERFLOW
    CHECK(LONG_MAX != LONG_MIN);
  }

  //Check overflow
  TEST(unsigned_long_bounds){
    unsigned long testLong = ULONG_MAX;
    testLong++;
    CHECK(LONG_MAX < ULONG_MAX);
    CHECK(testLong == 0);
  }

  //Check that a negative number can be created:
  //From a long
  TEST(bigint_negative_number){
      bigint test1("1");
      bigint test2("0");
      bigint test3("_1");
      CHECK(test2 < test1);
      CHECK(test3 < test2);
      CHECK(test3 < test1);
  }

  //Check that a negative number can be made from a string:
  TEST(bigint_negative_string){
    bigint negative1(-1);
    bigint negative1String("_1");
    bigint zero("0");
cout<<negative1<<endl;
    CHECK(negative1 < zero);
    CHECK(negative1String < zero);
    CHECK(negative1String == negative1);

  }

  //check that two numbers added together equal a third
  TEST(bigint_add_check){
    bigint one("1");
    bigint two("2");
    bigint three(3);
    bigint threeString("3");
    
    bigint added = one + two;
    CHECK(added == three);
    CHECK(added == threeString);
    CHECK(!(added == two));
  }

  //check that a smaller number is compared as smalled than another
  TEST(bigint_comparison_less_than){
    bigint three("3");
    bigint two("2");

    CHECK(two < three);
    CHECK(!(three < two));
  }

  
  //check that two numbers subtracted equal a third
  TEST(bigint_subtract_check){
    bigint three("3");
    bigint two("2");
    bigint one("1");
    
    bigint subtracted = three - two;
    CHECK(subtracted == one);
    CHECK(!(subtracted == two));
  }

  TEST(bigint_subtract_across_zero){
      bigint one("1");
      bigint two("2");
      bigint zero("0");
      bigint minus1("_1");
      bigint subtracted = one - two;

      CHECK(subtracted < zero);
      CHECK(subtracted == minus1);
  }

  TEST(bigint_negative_comparison){
      bigint minus1("_1");
      bigint minus2("_2");
      CHECK(minus2 < minus1);

  }
  

  //check that a number overflows
  //This is broken for a long based bigint,
  //Should not be for a vector based bigint;
  TEST(bigint_bounds_check){
    bigint max(LONG_MAX);
    bigint one("1");
    bigint min(LONG_MIN);
//cout<<LONG_MAX<<endl;
//cout<<LONG_MIN<<endl;
//cout<<max<<endl;
//cout<<min<<endl;
    bigint overflowed = max + one;
//cout<<overflowed<<endl;
    CHECK(min < overflowed);
    CHECK(!(min == overflowed));
  }

  //This should fail for long based bigint,
  //Pass for vector based bigint
  TEST(bigint_bounds_check_unsigned){
    bigint max(ULONG_MAX);
    bigint one("1");
    bigint zero("0");
cout<<max<<endl;
cout<<ULONG_MAX<<endl;
    bigint overflowed = max + one;
    CHECK(zero < max);
    CHECK(zero < one);
    CHECK(max < overflowed);
    CHECK(!(zero == overflowed));
  }

  //TEST IN THE OTHER DIRECTION:
  TEST(bigint_negative_overflow_check){
      bigint max(LONG_MAX);
      bigint min(LONG_MIN);
      bigint one("1");
      bigint zero("0");
//cout<<max<<endl;
//cout<<min<<endl;
      CHECK(max > min);
      CHECK(min < max);
      CHECK(one < max);
      CHECK(one > min);

      bigint maybe_overflow = min - one;

      CHECK(maybe_overflow < min);
      CHECK(maybe_overflow < max);
      CHECK(maybe_overflow < one);
      CHECK(!(maybe_overflow == min));
  }

  //Check multiplication works
  TEST(bigint_multiplication_test){
      bigint one("1");
      bigint ten("10");
      bigint oneHundred("100");

      bigint mult10 = one * ten;
      CHECK(mult10 == ten);
      bigint multTwice = mult10 * ten;
      CHECK(multTwice == oneHundred);
  }

  //Check big multiplication works:
  TEST(bigint_multiplication_across_bounds){
      bigint bound(LONG_MAX);
      bigint two("2");

      bigint additionResult = bound + bound;
      bigint result = bound * two;

      CHECK(additionResult == result);
  }

  TEST(bigint_large_mult){
    bigint first("23563262362367");
    bigint second("23536326");
    bigint expected("554592624584199843642");

    bigint mult = first * second;
    CHECK(expected == mult);

  }


  //Check pow:
  TEST(bigint_power_check){
      bigint two("2");
      bigint three("3");
      bigint four("4");
      bigint eight("8");
      bigint nine("9");

      bigint twoPowTwo = pow(two,two);
      CHECK(four == twoPowTwo);
      bigint twoPowThree = pow(two,three);
      CHECK(twoPowThree == eight);
      bigint threePowTwo = pow(three,two);
      CHECK(threePowTwo == nine);
  }

  TEST(bigint_large_power_check){
      bigint aNum("1255121242134124");
      bigint power("3");
      bigint expect("1977229308522879579190277317851851872791058624");

      bigint result = pow(aNum,power);
      CHECK(result == expect);

  }

  
  
  //Check Modulo:
  TEST(bigint_modulo_check){
      bigint aNum("235235532");
      bigint modBy("235325");
      bigint expectedResult("145857");

      bigint result = aNum % modBy;
      CHECK(result == expectedResult);          
  }
  
  //Check Division:
  TEST(bigint_div_check){
      bigint fifteen("15");
      bigint three("3");
      bigint five("5");

      bigint result = fifteen / five;
      CHECK(result == three);
  }

  TEST(bigint_div_check_bignum){
      bigint aNum("2356126732678899999");
      bigint divBy("2");
      bigint expect("1178063366339449999");

      bigint result = aNum / divBy;
      CHECK(result == expect);
      
  }
  

  
}
