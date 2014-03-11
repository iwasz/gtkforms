/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include "ReflectionMacros.h"
#include <string>
#include <vector>
#include <ostream>

struct Employee {
        ctr__ (void)
        std::string prp_ (firstname)
        std::string prp_ (lastname)
        std::string prp_ (city)
        std::string prp_ (imagePath)
        end_ (Employee)
};

typedef std::vector <Employee *> EmployeeVector;
col_ (EmployeeVector)

std::ostream &operator<< (std::ostream &o, Employee const &);

#endif /* EMPLOYEE_H_ */
