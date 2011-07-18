/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TESTCONTROLLER_H_
#define TESTCONTROLLER_H_

#include <Reflection.h>
#include <K202.h>

class TestController {
public:

        __c (void)

        _m (digit) std::string digit (int digit);
        _m (plus) void plus ();
        _m (minus) void minus ();
        _m (multi) void multi ();
        _m (div) void div ();
        _m (result) std::string result ();
        _m (dot) std::string dot ();
        _m (plusMinus) std::string plusMinus ();
        _m (clr) std::string clr ();
        _m (ce) std::string ce ();
        _m (back) std::string back ();

private:

        void updateOperand ();
        void clearBuffer ();

private:

        std::string buffer;
        double operand;
        enum Operation { PLUS, MINUS, MULTI, DIV };
        Operation operation;

        _e (TestController)

};

#	endif /* TESTCONTROLLER_H_ */
