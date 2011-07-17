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

void TestController::onClick ()
{
        std::cerr << "TestController::onClick" << std::endl;
}

/****************************************************************************/

void TestController::onClick2 ()
{
        std::cerr << "TestController::onClick2" << std::endl;
}

/****************************************************************************/

void TestController::valueChanged (double d)
{
        std::cerr << "TestController::valueChanged (" << d << ")" << std::endl;
}

/****************************************************************************/

void TestController::bufferChanged (std::string const &s)
{
        std::cerr << "TestController::bufferChanged (" << s << ")" << std::endl;
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
        std::cerr << "+" << std::endl;
}

/****************************************************************************/

void TestController::minus ()
{
        updateOperand ();
        clearBuffer ();
        operation = MINUS;
        std::cerr << "-" << std::endl;
}

/****************************************************************************/

void TestController::multi ()
{
        updateOperand ();
        clearBuffer ();
        operation = MULTI;
        std::cerr << "*" << std::endl;
}

/****************************************************************************/

void TestController::div ()
{
        updateOperand ();
        clearBuffer ();
        operation = DIV;
        std::cerr << "/" << std::endl;
}

/****************************************************************************/

void TestController::result ()
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
        std::cerr << "=" << operand << std::endl;
}

/****************************************************************************/

void TestController::dot ()
{
        buffer += ".";
        std::cerr << buffer << std::endl;
}

/****************************************************************************/

void TestController::plusMinus ()
{

}

/****************************************************************************/

void TestController::clr ()
{
        clearBuffer ();
        operand = 0;
}

/****************************************************************************/

void TestController::ce ()
{
        clearBuffer ();
        operand = 0;
}

/****************************************************************************/

void TestController::back ()
{

}
