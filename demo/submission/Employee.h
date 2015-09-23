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
        REFLECTION_CONSTRUCTOR_ (void)
        std::string REFLECTION_FIELD_VALUE_INPLACE (firstname)
        std::string REFLECTION_FIELD_VALUE_INPLACE (lastname)
        std::string REFLECTION_FIELD_VALUE_INPLACE (city)
        std::string REFLECTION_FIELD_VALUE_INPLACE (imagePath)
        REFLECTION_END (Employee)
};

typedef std::vector <Employee *> EmployeeVector;
REFLECTION_COLLECTION (EmployeeVector)

std::ostream &operator<< (std::ostream &o, Employee const &);

#endif /* EMPLOYEE_H_ */
