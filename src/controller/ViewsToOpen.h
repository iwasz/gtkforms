/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef VIEWSTOOPEN_H
#define VIEWSTOOPEN_H

#include <string>
#include <vector>

namespace GtkForms {

class ViewsToOpen {
public:
        struct ViewSlot {
                ViewSlot (std::string const &v, std::string const &s = "") : view (v), slot (s) {}
                std::string view;
                std::string slot;
        };

        typedef std::vector<ViewSlot> ViewSlotVector;

        ViewsToOpen () {}
        ViewsToOpen (const char *s) : ViewsToOpen (std::string (s)) {}
        ViewsToOpen (std::string const &s);

        void add (ViewSlot const &vs) { viewSlots.push_back (vs); }
        void add (std::string const &vn, std::string const &sn = "") { viewSlots.push_back (ViewSlot (vn, sn)); }
        ViewSlotVector const &getViewSlots () const { return viewSlots; }
        void clear () { viewSlots.clear (); }

private:
        ViewSlotVector viewSlots;
};

} // namespace

#endif // VIEWSTOOPEN_H