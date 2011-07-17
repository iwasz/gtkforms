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

        _m (onClick) void onClick ();
        _m (onClick2) void onClick2 ();
        _m (valueChanged) void valueChanged (double);
        _m (bufferChanged) void bufferChanged (std::string const &);

        _m (digit) std::string digit (int digit);
        _m (plus) void plus ();
        _m (minus) void minus ();
        _m (multi) void multi ();
        _m (div) void div ();
        _m (result) void result ();
        _m (dot) void dot ();
        _m (plusMinus) void plusMinus ();
        _m (clr) void clr ();
        _m (ce) void ce ();
        _m (back) void back ();

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
