/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <Pointer.h>
#include <Container.h>
#include <boost/make_shared.hpp>
#include "../src/GtkForms.h"
#include <gtk/gtk.h>

using namespace Container;

/**
 *
 */
int main (int argc, char **argv)
{
        try {
                GtkBootstrap::init (&argc, &argv);
                Ptr <BeanFactoryContainer> container = XmlContainerFactory::createContainer ("../demo/main.xml");

//                container->getBean ("machine");

                Ptr <App> app = vcast <Ptr <App> > (container->getBean ("app"));
                app->run ();
        }
        catch (std::exception const &e) {
                std::cerr << e.what () << std::endl;
                return 1;
        }

        return 0;
}


