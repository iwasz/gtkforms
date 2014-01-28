/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Page.h"

namespace GtkForms {
using namespace std;

/*--------------------------------------------------------------------------*/

PageOperationResult Page::join (IPage *page)
{
        ViewMap &ctmap = page->getViews ();
        PageOperationResult result;

        for (ViewMap::value_type &entry : ctmap) {
                if (views.find (entry.first) == views.end ()) {
                        views[entry.first] = entry.second;
                        result.added[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

PageOperationResult Page::start (IPage *page)
{
        ViewMap &ctmap = page->getViews ();
        PageOperationResult result;

        for (auto i = views.begin (); i != views.end (); ) {
                if (ctmap.find (i->first) == ctmap.end ()) {
                        i = views.erase (i);
                        result.removed[i->first] = i->second;
                }
                else {
                        ++i;
                }
        }

        for (ViewMap::value_type &entry : ctmap) {
                if (views.find (entry.first) == views.end ()) {
                        views[entry.first] = entry.second;
                        result.added[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

PageOperationResult Page::split (IPage *page)
{
        ViewMap &ctmap = page->getViews ();
        PageOperationResult result;

        for (ViewMap::value_type &entry : ctmap) {
                if (views.find (entry.first) != views.end ()) {
                        views.erase (entry.first);
                        result.removed[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

ostream &operator<< (ostream &o, Page const &p)
{
        o << "Page [";

        for (auto i = p.views.begin (); i != p.views.end (); ) {
                o << i->second->getName ();

                if (++i != p.views.end ()) {
                        o << ", ";
                }
        }

        o << "]";
        return o;
}

/*--------------------------------------------------------------------------*/

PageOperationResult &PageOperationResult::operator += (PageOperationResult const &uor)
{
        copy (uor.added.begin (), uor.added.end (), inserter (added, added.end ()));
        copy (uor.removed.begin (), uor.removed.end (), inserter (removed, removed.end ()));
        return *this;
}

} // namespace GtkForms
