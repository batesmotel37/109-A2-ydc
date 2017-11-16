#include <iostream>
#include "UnitTest++/UnitTest++.h"

#include "ubigint.h"
#include <climits>

using namespace std;
//The Main function that triggers the tests to be run
int main(int, const char *[]){
  cout << "ubigint test runner" << endl;
	return UnitTest::RunAllTests();
}

SUITE(ubigint_Test_Suite){

  //Check that two basic ubigints are the same
  TEST(basic_Comparison){
    ubigint test;
    ubigint test2;
    CHECK(test == test2);
  }

  //check that two valued ubigints are the same
  TEST(basic_long_comparison){
   ubigint testUBigInt(5);
   ubigint testUBigInt2(5);
   CHECK(testUBigInt == testUBigInt2);
  }

  //check that two different valued ubigints are NOT the same
  TEST(basic_long_comparison_NOT_EQUAL){
   ubigint testUBigInt(5);
   ubigint testUBigInt2(10);
   CHECK(!(testUBigInt == testUBigInt2));
  }

  //Check that a ubigint made from a string will equal one made from a long
  TEST(basic_string_instantiation){
    ubigint testStringbigint("5");
    ubigint testLongbigint(5);
    CHECK(testStringbigint == testLongbigint);
  }

  //Check that two ubigints from strings are equal
  TEST(basic_string_comparison){
    ubigint test1("25");
    ubigint test2("25");

    CHECK(test1 == test2);
  }

  //Check that a double digit string will equal a double digit long
  TEST(basic_string_double_digits){
    ubigint testString1("10");
    ubigint testLong1(10);
    CHECK(testString1 == testLong1);
  }

  //Check system long bounds
  TEST(signed_long_bounds){
    long testLong = LONG_MAX;
    CHECK(testLong == LONG_MAX);
    testLong++; //CAUSES OVERFLOW
    CHECK(testLong == LONG_MIN); //THIS IS TRUE BECAUSE OF OVERFLOW
    CHECK(LONG_MAX != LONG_MIN);
  }

  TEST(unsigned_long_bounds){
    unsigned long testLong = ULONG_MAX;
    testLong++;
    CHECK(LONG_MAX < ULONG_MAX);
    CHECK(testLong == 0);
  }

  //check that two numbers added together equal a third
  TEST(ubigint_add_check){
    ubigint one("1");
    ubigint two("2");
    ubigint three("3");

    ubigint added = one + two;

    CHECK(added == three);
    CHECK(!(added == two));
  }

  //check that two numbers subtracted equal a third
  TEST(ubigint_subtract_check){
    ubigint three("3");
    ubigint two("2");
    ubigint one("1");

    ubigint subtracted = three - two;
    CHECK(subtracted == one);
    CHECK(!(subtracted == two));
  }

  TEST(ubigint_large_subtraction_check){
    ubigint large("12521232646437347346346");
    ubigint subtractor("12123536346346");
    ubigint expected("12521232634313811000000");

    ubigint result = large - subtractor;
    CHECK(result == expected);

  }

  //check that a smaller number is compared as smalled than another
  TEST(ubigint_comparison_less_than){
    ubigint three("3");
    ubigint two("2");

    CHECK(two < three);
    CHECK(!(three < two));
  }


  //This should fail for long based bigint,
  //Pass for vector based bigint
  TEST(ubigint_unsigned_bounds_check){
    ubigint max(ULONG_MAX);
    ubigint one("1");
    ubigint zero("0");

    ubigint overflowed = max + one;
    CHECK(zero < max);
    CHECK(zero < one);
    CHECK(max < overflowed);
    CHECK(!(zero == overflowed));
  }

  
}
