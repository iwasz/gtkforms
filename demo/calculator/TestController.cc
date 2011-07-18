/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TestController.h"

void TestController::updateOperand ()
{
        operand = boost::lexical_cast <double> (buffer);
}

/****************************************************************************/

void TestController::clearBuffer ()
{
        buffer = "";
}

/****************************************************************************/

std::string TestController::digit (int digit)
{
        buffer += boost::lexical_cast <std::string> (digit);
        return buffer;
}

/****************************************************************************/

void TestController::plus ()
{
        updateOperand ();
        clearBuffer ();
        operation = PLUS;
}

/****************************************************************************/

void TestController::minus ()
{
        updateOperand ();
        clearBuffer ();
        operation = MINUS;
}

/****************************************************************************/

void TestController::multi ()
{
        updateOperand ();
        clearBuffer ();
        operation = MULTI;
}

/****************************************************************************/

void TestController::div ()
{
        updateOperand ();
        clearBuffer ();
        operation = DIV;
}

/****************************************************************************/

std::string TestController::result ()
{
        double oper1 = operand;
        updateOperand ();
        double oper2 = operand;

        switch (operation) {
        case PLUS:
                operand = oper1 + oper2;
                break;
        case MINUS:
                operand = oper1 - oper2;
                break;
        case MULTI:
                operand = oper1 * oper2;
                break;
        case DIV:
                operand = oper1 / oper2;
                break;
        default:
                break;
        }

        clearBuffer ();
        return boost::lexical_cast <std::string> (operand);;
}

/****************************************************************************/

std::string TestController::dot ()
{
        buffer += ".";
        return buffer;
}

/****************************************************************************/

std::string TestController::plusMinus ()
{
        return "-" + buffer;
}

/****************************************************************************/

std::string TestController::clr ()
{
        clearBuffer ();
        operand = 0;
        return buffer;
}

/****************************************************************************/

std::string TestController::ce ()
{
        clearBuffer ();
        operand = 0;
        return buffer;
}

/****************************************************************************/

std::string TestController::back ()
{
//        buffer.
        return buffer;
}
