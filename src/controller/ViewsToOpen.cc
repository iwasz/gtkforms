/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "ViewsToOpen.h"
#include <boost/algorithm/string.hpp>
#include <core/Exception.h>
#include <core/Typedefs.h>

namespace GtkForms {

ViewsToOpen::ViewsToOpen (std::string const &s)
{
        if (s.empty ()) {
                clear ();
        }

        Core::StringVector viewNamesList;
        boost::split (viewNamesList, s, boost::is_any_of (", "), boost::token_compress_on);

        for (std::string const &viewAndSlot : viewNamesList) {
                std::string viewName;
                std::string slotName;
                std::string slotFeature;

                Core::StringVector names;
                boost::split (names, viewAndSlot, boost::is_any_of ("->"), boost::token_compress_on);

                if (names.empty ()) {
                        throw Core::Exception ("ViewsToOpen::ViewsToOpen : names.empty ()");
                }

                viewName = names[0];

                if (names.size () > 1) {
                        slotName = names[1];
                }

                if (names.size () > 2) {
                        slotFeature = names[2];
                }

                viewSlots.push_back (ViewSlot (viewName, slotName, slotFeature));
        }
}

} // namespace
