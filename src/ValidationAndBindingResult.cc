/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "ValidationAndBindingResult.h"

namespace GtkForms {

bool ValidationAndBindingResultContainer::add (ValidationAndBindingResult const &v)
{
        ValidationResultList::iterator p
                = std::find_if (previous.begin (), previous.end (), [&](ValidationAndBindingResult const &el) { return el.model == v.model; });
        ValidationResultList::iterator r
                = std::find_if (results.begin (), results.end (), [&](ValidationAndBindingResult const &el) { return el.model == v.model; });

        // No r in results yet
        if (r == results.end ()) {
                if (!v.valid) {
                        results.push_back (v);
                        return true;
                }
                else {
                        if (p != previous.end () && !p->valid) {
                                results.push_back (v);
                                return true;
                        }
                }
        }

        if (!r->valid) {
                return false;
        }

        if (r != results.end ()) {
                results.erase (r);
        }

        results.push_back (v);
        return true;
}
}
