/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef SPECIFIC_ANNOTATIONMACROS_H_
#define SPECIFIC_ANNOTATIONMACROS_H_

#include <reflection/Reflection.h>

#undef b_
#undef b__
#undef d__
#undef g_
#undef c_
#undef c__
#undef fn_
#undef f_
#undef m_
#undef m__
#undef M_
#undef M__
#undef Mc_
#undef M__
#undef s_
#undef s__
#undef e_
#undef e__
#undef tb_
#undef t_
#undef te_
#undef p_
#undef pe_
#undef P_

#define b_      REFLECTION_BASE_CLASS
#define b__     REFLECTION_BASE_CLASS_
#define d__     REFLECTION_CLASS
#define g_      REFLECTION_COLLECTION
#define C_      REFLECTION_CONSTRUCTOR
#define C__     REFLECTION_CONSTRUCTOR_
#define fn_     REFLECTION_FACTORY_NAME
#define F_      REFLECTION_FACTORY
#define m_      REFLECTION_METHOD
#define m__     REFLECTION_METHOD_
#define M_      REFLECTION_OVERLOADED_METHOD
#define M__     REFLECTION_OVERLOADED_METHOD_
#define Mc_     REFLECTION_OVERLOADED_CONST_METHOD
#define Mc__    REFLECTION_OVERLOADED_CONST_METHOD_
#define S_      REFLECTION_SETTER
#define S__     REFLECTION_SETTER_
#define E_      REFLECTION_END
#define e__     REFLECTION_END_
#define tb_     REFLECTION_TEMPLATE_BEGIN
#define t__     REFLECTION_TEMPLATE
#define te_     REFLECTION_TEMPLATE_END
#define p_      REFLECTION_FIELD_VALUE_INPLACE
#define pe_     REFLECTION_FIELD_ENUM_INPLACE
#define P_      REFLECTION_FIELD_REFERENCE_INPLACE


#	endif /* DEFAULTANNOTATIONMACROS_H_ */
