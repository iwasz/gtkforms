/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <Container.h>
#include "../src/GtkForms.h"

using namespace Container;
using namespace GtkForms;

/**
 *
 */
int main (int argc, char **argv)
{
        try {
                GtkBootstrap::init (&argc, &argv);
                Ptr <BeanFactoryContainer> container = XmlContainerFactory::createContainer ("../demo/main.xml");
                Ptr <App> app = vcast <Ptr <App> > (container->getBean ("app"));
                app->run ();
        }
        catch (std::exception const &e) {
                std::cerr << e.what () << std::endl;
                return 1;
        }

        return 0;
}


