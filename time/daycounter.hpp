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

/*
  Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
  Copyright (C) 2003, 2004, 2005, 2006, 2007 StatPro Italia srl

  This file is part of QuantLib, a free-software/open-source library
  for financial quantitative analysts and developers - http://quantlib.org/

  QuantLib is free software: you can redistribute it and/or modify it
  under the terms of the QuantLib license.  You should have received a
  copy of the license along with this program; if not, please email
  <quantlib-dev@lists.sf.net>. The license is also available online at
  <http://quantlib.org/license.shtml>.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef MATHFIN_DAYCOUNTER_HPP
#define MATHFIN_DAYCOUNTER_HPP

#include <memory>
#include <string>
#include <time/date.hpp>
#include <base/error.hpp>

namespace MathFin {
  /** Day counter class.
   *
   * This class provides methods for determining the length of a time
   * period according to given market convention, both as a number
   * of days and as a year fraction.
   *
   * The Bridge pattern is used to provide the base behavior of the
   * day counter.
   * @ingroup datetime
   */
  class DayCounter {

  protected:
    // abstract base class for day counter implementations
    class Impl {
    public:
      virtual ~Impl() {}
      virtual std::string name() const = 0;
      // to be overloaded by more complex day counters
      virtual Date::serial_type dayCount(const Date& d1, const Date& d2) const {
        return (d2 - d1);
      }

      virtual Time yearFraction(
        const Date& d1,
        const Date& d2,
        const Date& refPeriodStart,
        const Date& refPeriodEnd) const = 0;
    };

    std::shared_ptr<Impl> impl_;

    /**
     * This constructor can be invoked by derived classes which
     * define a given implementation.
     */
    DayCounter(const std::shared_ptr<Impl>& impl) : impl_(impl) {}

  public:
    /**
     * The default constructor returns a day counter with a null
     * implementation, which is therefore unusable except as a
     * placeholder.
     */
    DayCounter() {}

    /** @name DayCounter interface
     * @{
     */

    /**
     * Returns whether or not the day counter is initialized
     */
    inline bool empty() const {
      return !impl_;
    }

    /**
     * Returns the name of the day counter.
     *
     * @warning This method is used for output and comparison between
     * day counters. It is <b>not</b> meant to be used for writing
     * switch-on-type code.
     */
    inline std::string name() const {
      MF_REQUIRE(impl_, "no implementation provided");
      return impl_->name();
    }

    /**
     * Returns the number of days between two dates.
     */
    inline Date::serial_type dayCount(const Date& d1, const Date& d2) const {
      MF_REQUIRE(impl_, "no implementation provided");
      return impl_->dayCount(d1,d2);
    }

    /**
     * Returns the period between two dates as a fraction of year.
     */
    inline Time yearFraction(const Date& d1, const Date& d2,
                      const Date& refPeriodStart = Date(),
                      const Date& refPeriodEnd = Date()) const {
      MF_REQUIRE(impl_, "no implementation provided");
      return impl_->yearFraction(d1, d2, refPeriodStart, refPeriodEnd);
    }

    /** @} */
  };

  /**
   * Comparison based on name.
   * Returns <tt>true</tt> iff the two day counters belong to the same
   * derived class.
   * @relates DayCounter
   */
  inline bool operator==(const DayCounter& d1, const DayCounter& d2) {
    return (d1.empty() && d2.empty())
      || (!d1.empty() && !d2.empty() && d1.name() == d2.name());
  }

  /**
   * @relates DayCounter
   */
  inline bool operator!=(const DayCounter& d1, const DayCounter& d2) {
    return !(d1 == d2);
  }

  /**
   * @relates DayCounter
   */
  inline std::ostream& operator<<(std::ostream& out, const DayCounter &d) {
    return out << d.name();
  }

}

#endif /* MATHFIN_DAYCOUNTER_HPP */
