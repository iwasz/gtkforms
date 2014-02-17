/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TableMapping.h"
#include <gtk/gtk.h>
#include <beanWrapper/BeanWrapper.h>
#include "tools/GValueVariant.h"
#include "Mapper.h"

namespace GtkForms1 {
using namespace Core;

void TableMapping::m2v (MapperDTO *dto)
{
        Common::Context ctx;
        Variant store = dto->beanWrapper->get (storePath, &ctx);

        if (ctx.isError ()) {
                throw MappingException ("TableMapping::m2v : cant get GtkTreeModel from beanWrapper. Message : \n" + ctx.getMessage ());
        }

        GtkListStore *list = GTK_LIST_STORE (vcast <void const *> (store));

        dto->beanWrapper->setWrappedObject (dto->model);
        Ptr <IIterator> i = dto->beanWrapper->iterator ("");
        GtkTreeIter iter;

        while (i->hasNext ()) {

                // Wiersz kolecji wejściowej - najlepiej, żeby to była lista, wektor, lub set.
                Variant element = i->next ();
                dto->beanWrapper->setWrappedObject (element);

                // Dodaj wiersz i uzyskaj iterator.
                gtk_list_store_append (list, &iter);

                for (CellVector::const_iterator i = cells->begin (); i != cells->end (); ++i) {
                        GValue gVal = {0};
                        Core::Variant vVal = dto->beanWrapper->get (&element, (*i)->getModelProp ());
                        GtkForms::variantToGValue (&gVal, vVal);
                        gtk_list_store_set_value (list, &iter, (*i)->getViewRow (), &gVal);
                }
        }
}

/****************************************************************************/

void TableMapping::v2m (MapperDTO *dto)
{

}

} /* namespace GtkForms */
