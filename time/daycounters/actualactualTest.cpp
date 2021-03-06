/*
  Copyright (C) 2016 Ahmed Riza

  This file is part of MathFin.

  This program is free software: you  can redistribute it and/or modify it
  under the  terms of the GNU  General Public License as  published by the
  Free Software Foundation,  either version 3 of the License,  or (at your
  option) any later version.

  This  program  is distributed  in  the  hope  that  it will  be  useful,
  but  WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
  Public License for more details.

  You should have received a copy  of the GNU General Public License along
  with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <test/catch.hpp>
#include <base/error.hpp>
#include <time/daycounters/actualactual.hpp>

using namespace MathFin;

#define EPS 1.0E-12

TEST_CASE("Actual/Actual (AFB)", "[daycounters]") {
  // This example is from:
  // https://en.wikipedia.org/wiki/Day_count_convention#Actual.2FActual_AFB
  ActualActual dc(ActualActual::Convention::AFB);
  Date d11(10, Month::February, 1994);
  Date d21(30, Month::June, 1997);
  Time yf = dc.yearFraction(d11, d21, Date(), Date());
  double expected = 3.0 + 140.0/365.0;
  REQUIRE(yf - expected <= EPS);

  // http://www.isda.org/c_and_a/pdf/Euro-swap-memo-ISDA-1998.pdf
  Date d12(1, Month::November, 2003);
  Date d22(1, Month::May, 2004);
  yf = dc.yearFraction(d12, d22, Date(), Date());
  REQUIRE(yf - (188/366.0) <= EPS);

  Date d13(1, Month::February, 1999);
  Date d23(1, Month::July, 1999);
  yf = dc.yearFraction(d13, d23, Date(), Date());
  REQUIRE(yf - (150/365.0) <= EPS);

  Date d14(1, Month::July, 1999);
  Date d24(1, Month::July, 2000);
  yf = dc.yearFraction(d14, d24, Date(), Date());
  expected = 184/365.0 + 182/366.0;
  REQUIRE(yf - expected <= EPS);

  Date d15(15, Month::August, 2002);
  Date d25(15, Month::July, 2003);
  yf = dc.yearFraction(d15, d25, Date(), Date());
  REQUIRE(yf - 334/365.0 <= EPS);

  Date d16(15, Month::July, 2003);
  Date d26(15, Month::January, 2004);
  yf = dc.yearFraction(d16, d26, Date(), Date());
  REQUIRE(yf - 184/365.0 <= EPS);

  Date d17(30, Month::July, 1999);
  Date d27(30, Month::January, 2000);
  yf = dc.yearFraction(d17, d27, Date(), Date());
  REQUIRE(yf - 184/365.0 <= EPS);

  Date d18(30, Month::January, 2000);
  Date d28(30, Month::June, 2000);
  yf = dc.yearFraction(d18, d28, Date(), Date());
  REQUIRE(yf - 152/366.0 <= EPS);
}

TEST_CASE("Actual/Actual (ISDA)", "[daycounters]") {
  // Semin-annual payments
  ActualActual dc(ActualActual::Convention::ISDA);
  Date d11(1, Month::November, 2003);
  Date d21(1, Month::May, 2004);
  Time yf = dc.yearFraction(d11, d21, Date(), Date());
  REQUIRE(yf - (61/365.0 + 121/366.0) <= EPS);

  // Short first calcualtion period.
  // Annual payments
  Date d12(1, Month::February, 1999);
  Date d22(1, Month::July, 1999);
  yf = dc.yearFraction(d12, d22, Date(), Date());
  REQUIRE(yf - (150/365.0) <= EPS);

  Date d13(1, Month::July, 1999);
  Date d23(1, Month::July, 2000);
  yf = dc.yearFraction(d13, d23, Date(), Date());
  REQUIRE(yf - (184/365.0 + 182/366.0) <= EPS);

  // Long first calculation period.
  // Annual payments
  Date d14(15, Month::August, 2002);
  Date d24(15, Month::July, 2003);
  yf = dc.yearFraction(d14, d24, Date(), Date());
  REQUIRE(yf - 334/365.0 <= EPS);

  Date d15(15, Month::July, 2003);
  Date d25(15, Month::January, 2004);
  yf = dc.yearFraction(d15, d25, Date(), Date());
  REQUIRE(yf - (170/365.0 + 14/366.0)  <= EPS);

  // Short final calculation period
  // Semi-annual payments
  Date d16(30, Month::July, 1999);
  Date d26(30, Month::January, 2000);
  yf = dc.yearFraction(d16, d26, Date(), Date());
  REQUIRE(yf - (155/365.0 + 29/366.0) <= EPS);

  Date d17(30, Month::January, 2000);
  Date d27(30, Month::June, 2000);
  yf = dc.yearFraction(d17, d27, Date(), Date());
  REQUIRE(yf - (152/366.0) <= EPS);

  // Greater than 2 years across a leap year.
  Date d18(30, Month::December, 2010);
  Date d28(2, Month::January, 2013);
  yf = dc.yearFraction(d18, d28, Date(), Date());
  REQUIRE(yf - ( 367/365.0 + 366/366.0 + 1/365.0) <= EPS);
}

TEST_CASE("Actual/Actual (ISMA)", "[daycounters]") {
  // Semin-annual payments
  ActualActual dc(ActualActual::Convention::ISMA);
  Date d11(1, Month::November, 2003);
  Date d21(1, Month::May, 2004);
  Date d31(1, Month::November, 2003);
  Date d41(1, Month::May, 2004);
  Time yf = dc.yearFraction(d11, d21, d31, d41);
  REQUIRE(yf - 182/(182.0 * 2) <= EPS);

  // Short first calcualtion period.
  // Annual payments
  Date d12(1, Month::February, 1999);
  Date d22(1, Month::July, 1999);
  Date d32(1, Month::July, 1998);
  Date d42(1, Month::July, 1999);
  yf = dc.yearFraction(d12, d22, d32, d42);
  REQUIRE(yf - 150/(365.0 * 1) <= EPS);

  Date d13(1, Month::July, 1999);
  Date d23(1, Month::July, 2000);
  Date d33(1, Month::July, 1999);
  Date d43(1, Month::July, 2000);
  yf = dc.yearFraction(d13, d23, d33, d43);
  REQUIRE(yf - 366/(366.0 * 1) <= EPS);

  // Long first calculation period.
  // Annual payments
  Date d14(15, Month::August, 2002);
  Date d24(15, Month::July, 2003);
  Date d34(15, Month::January, 2003);
  Date d44(15, Month::July, 2003);
  yf = dc.yearFraction(d14, d24, d34, d44);
  REQUIRE(yf - (181/(181.0 * 2) + 153/(184.0 * 2)) <= EPS);

  Date d15(15, Month::July, 2003);
  Date d25(15, Month::January, 2004);
  Date d35(15, Month::July, 2003);
  Date d45(15, Month::January, 2004);
  yf = dc.yearFraction(d15, d25, d35, d45);
  REQUIRE(yf - 184/(184.0 * 2) <= EPS);

  // Short final calculation period
  // Semi-annual payments
  Date d16(30, Month::July, 1999);
  Date d26(30, Month::January, 2000);
  Date d36(30, Month::July, 1999);
  Date d46(30, Month::January, 2000);
  yf = dc.yearFraction(d16, d26, d36, d46);
  REQUIRE(yf - 184/(184.0* 2) <= EPS);

  Date d17(30, Month::January, 2000);
  Date d27(30, Month::June, 2000);
  Date d37(30, Month::January, 2000);
  Date d47(30, Month::July, 2000);
  yf = dc.yearFraction(d17, d27, d37, d47);
  REQUIRE(yf - 152/(182.0 * 2) <= EPS);

  // Long final calculation period
  // Quarterly payments
  Date d18(30, Month::November, 1999);
  Date d28(30, Month::April, 2000);
  Date d38(30, Month::November, 1999);
  Date d48(31, Month::May, 2000);
  yf = dc.yearFraction(d18, d28, d38, d48);
  REQUIRE(yf - (91/(91.0 * 4) + 61/(92.0 * 4)) <= EPS);
}
