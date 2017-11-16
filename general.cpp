// $Id: general.cpp,v 1.1 2015-07-01 18:40:41-07 - - $

#include "general.h"

ydc_exn::ydc_exn (const string& what): runtime_error (what) {
}

string sys_info::execname_; // Must be initialized from main().
int sys_info::status_ = EXIT_SUCCESS;

void sys_info::execname (const string& argv0) {
   execname_ = argv0;
   cout << boolalpha;
   cerr << boolalpha;
   DEBUGF ('Y', "execname = " << execname_);
}

ostream& complain() {
   sys_info::status (EXIT_FAILURE);
   cerr << sys_info::execname() << ": ";
   return cerr;
}

