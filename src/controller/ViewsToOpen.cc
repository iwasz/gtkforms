/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "ViewsToOpen.h"
#include <boost/algorithm/string.hpp>
#include <core/Typedefs.h>

namespace GtkForms {

ViewsToOpen::ViewsToOpen (std::string const &s)
{

        Core::StringVector viewNamesList;
        boost::split (viewNamesList, s, boost::is_any_of (", "), boost::token_compress_on);

        for (std::string const &viewAndSlot : viewNamesList) {
                std::string viewName;
                std::string slotName;

                size_t offset;
                if ((offset = viewAndSlot.find ("->")) != std::string::npos) {
                        slotName = viewAndSlot.substr (offset + 2);
                }

                viewName = viewAndSlot.substr (0, offset);

                if (viewName.empty ()) {
                        continue;
                }

                viewSlots.push_back (ViewSlot (viewName, slotName));
        }
}

} // namespace
