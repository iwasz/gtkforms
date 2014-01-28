/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKVIEW_H_
#define GTKVIEW_H_

#include "IView.h"
#include "ReflectionMacros.h"

class GtkView : public IView {
public:
        ctr__ (void)

        virtual ~GtkView () {}

        std::string getName () const;
        void show ();
        void hide ();

        void model2View (std::string const &dataRange);
        void view2Model (std::string const &dataRange);

private:

        std::string prr_ (name);

        end_ (GtkView)
};

#endif /* GTKVIEW_H_ */
