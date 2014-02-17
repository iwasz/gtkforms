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

struct Employee {
        ctr__ (void)
        std::string prp_ (firstname)
        std::string prp_ (lastname)
        std::string prp_ (city)
        end_ (Employee)
};

typedef std::vector <Employee *> EmployeeVector;
col_ (EmployeeVector)

#endif /* EMPLOYEE_H_ */
