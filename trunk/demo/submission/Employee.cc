/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Employee.h"

std::ostream &operator<< (std::ostream &o, Employee const &e)
{
        o << "Employee [" << e.firstname << ", " << e.lastname << "]";
        return o;
}
