/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GtkView.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include "Slot.h"

namespace GtkForms {
using namespace std;
static src::logger_mt& lg = logger::get ();

struct GtkView::Impl {

        static void onIterateWidget (GtkWidget *widget, gpointer data);
        static void onPrintWidget (GtkWidget *widget, gpointer data);

};

/****************************************************************************/

GtkView::GtkView ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

GtkView::~GtkView ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

//void GtkView::loadUi (Context *context)
//{
//        if (!uiFile) {
//                throw Core::Exception ("No UiFile object set inside GtkView.");
//        }
//
//        impl->builder = uiFile->load ();
//        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));
//
//        if (!impl->widget) {
//                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
//        }
//
//        connectSignals (context);
//}

/*--------------------------------------------------------------------------*/

void GtkView::show ()
{
//        gtk_builder_connect_signals (builder, NULL);
//        gtk_builder_connect_signals_full (builder, myConnectFunc, NULL);


//                TODO Podłączyć do czegoś, co będzie wysylać QuitEvent do wszystkich kontrolerów.
//                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
//                g_signal_connect (window, "destroy", G_CALLBACK (gtk_widget_destroyed), &window);

        gtk_widget_show_all (GTK_WIDGET (getUi ()));
}

/*--------------------------------------------------------------------------*/

//void GtkView::hide ()
//{
//
//}
//
///*--------------------------------------------------------------------------*/
//
//void GtkView::destroy ()
//{
//        // TODO : if (toplevel)
//        gtk_widget_destroy (impl->widget);
//        impl->widget = 0;
//        impl->builder = 0;
//}

/*--------------------------------------------------------------------------*/

void GtkView::model2View (std::string const &dataRange)
{

}

/*--------------------------------------------------------------------------*/

void GtkView::view2Model (std::string const &dataRange)
{

}

/*--------------------------------------------------------------------------*/

//GObject *GtkView::getUi (/*std::string const &name*/)
//{
//        GObject *obj =  gtk_builder_get_object (impl->builder, name.c_str ());
//
//        if (!obj) {
//                throw Core::Exception ("GtkView::getGObject could not find object in UI. Ui file : [" + uiFile->getFile () + "], object name : [" + std::string (name) + "].");
//        }
//
//        return obj;
//}

/*
 * Excerpt from GTK+ documentation : "A GtkBuilder holds a reference to all objects that it has constructed and drops
 * these references when it is finalized. This finalization can cause the destruction of non-widget objects or widgets
 * which are not contained in a toplevel window. For toplevel windows constructed by a builder, it is the responsibility
 * of the user to call gtk_widget_destroy() to get rid of them and all the widgets they contain."
 *
 * So GtkWidnows should be destroyed explicitely, the others are reference-counted.
 */
void GtkView::reparent (GtkTileMap const &tiles, SlotVector const &slots, Context *context)
{
//        map <string, GtkBin *> slots;
//        map <string, GtkWidget *> tiles;
//        SlotVector allTiles;

        /*
         * Możliwości:
         * - Otworzyć nowy widok (A) składający się z kafelków.
         *  - Ładujemy główny widok.
         *  - Ładujemy kafelki.
         *  - Składamy do kupy.
         *
         * - Otworzyć jeszcze jeden widok (B) (możliwe że też kafelkowy) nad tym już istniejącym (drugie top-level window).
         *  - Ładujemy nowy widok, nie ruszamy tego starego.
         *  - Ładujemy kafelki.
         *  - Składamy do kupy.
         *
         * - Otworzyć nowy kafelek i dokleić go do już istniejącego widoku.
         *  - Załadować kafelek.
         *  - Umiescić go na miejscu.
         *
         * - Otworzyć widok (C), który ma zamienić widok (A) i (B)
         *  - Ładujemy główny widok.
         *  - Ładujemy kafelki (te kóre nie są załadowane).
         *  - Zamykamy poprzedni główny widok.
         *  - Składamy do kupy.
         *
         * - Page , czyli strona skłąda się tylko:
         *  - GtkWindow - tylko jedno (standalone).
         *  - GtkTile - wiele.
         *  - Z poniższego wynika, że strona musi mieć nazwę, a więc musi być jakaś mapa stron zdefiniowana niestety.
         * Operacje
         *  - Page.getName ().  Pobierz nazwę.
         *  - Page.getView (). Pobierz głowne okno.
         *  - Page.getTiles (). Pobierz kafelki.
         *  loadUi (). Wywołuje loadUi na wsyztskich tilesach i na GtkWindow
         *
         *  GtkTile operacje. Kafelek.
         *   - loadUi (). załaduj UI. jezeli już załadowane, to nic się nie dzieje. To jest pomyślane jako singleton, który zajmuje pamięć lub zwalnia, ale jest tylko jeden.
         *   - getUi ().  pobierz ui.
         *   - show ().
         *   - destroy ()
         *
         * GtkView/albo GtkWindow. Główne okno
         *   - loadUi (). załaduj UI.
         *   - getUi ().  pobierz ui.
         *   - show ().
         *   - destroy ()
         *   - reparent (map <string, GtkTile *>).
         *
         *
         *
         *  Wartości zwracane z kontrolerów (nazwy widoków ze specalnymi znacznikami):
         *  +page (otwórz stronę).
         *  -page (zamknij stronę, ale po załadowaniu srona nie ma nazwy, więc jak ją zamknąć?).
         *  pageA->pageB, lub po prostu ->page (move, czyli zmień aktualną stronę na nową stronę przenosząc kafelki jeśli się da)
         *  +pageA,+pageB otwórz dwie strony.
         *
         * Tylko operacja -> wymaga wyjaśnienia. Operacja pageA->pageB.
         * - pageA ma widok głowny (umownie GtkWindow), i pageB też. Ten z A jest NA PEWNO DO ZAMKNIĘCIA, a ten z B do otworzenia (załadować).
         * - kafelki z A są już załadowane. Trzeba załadować kafelki od widoku B.
         * - Pula kafelków to teraz jest pula z A i z B - trzeba je dodac do jednej mapy - jeżeli nazwy się powtarzają, to wygrywają te nowsze,
         *   alternatywnie kafelki muszą mieć unikalne nazwy. Widok docelowy to jest
         * - Umieszczamy kafelki w slotach.
         *
         * Operacja +page.
         * - Container.getBean (Page.getname ()) Znajdujemy page w tej super-mapie (singleton, załadowany już).
         * - Page.loadUi (). ładujemy gowny widok i kafelki.
         * - tiles = Page.getTiles (). Pula kafelków : mapa, albo jakiś obiekt typu mapa.
         * - mainWindow = Page.getMainWindow . pobieramy główny widok.
         * - mainWindow.reparent (tiles);
         * - Page.show (). Pokazujemy wszystko. (lub mainWindow.show ()).
         *
         * ----------
         *
         * - View ma getWidget (bae arg) - zwraca widget o nazwie takiej jak name (w postaci GObject)
         * - View ma get slots (map <string, GtkBin *>)
         * - mamy wszystikie widoki i do zamknięcia i do otworzenia i te już widoczne.
         * - Te do otworzenia ładujemy do pamięci. Stan #1 : Wszystko co jest potrzebne jest w pamięci, każdy widget jest dostępny.

         * - Pobieramy sloty z tych już widocznych i tych załadowanych (do otworzenia).
         * - jesteśmy w stanie #2, w kŧórym wszystko jest w pamięci, mamy wskaźniki do wszystkich slotów wszystkich widoków i wsakźniki do wszystkich widoków (plugów i nie plugów poprzez GObject *view.getWidget ()).
         *
         * - Plugi zawsze mają jakiegos parenta, więc trzeba je reparentować (zawsze).
         *
         */

        SlotWidgetMap slotWidgets = getSlotWidgets (slots);

        // 2. Reparent.
        for (Slot *slot : slots) {
                GtkBin *slotWidget = 0;
                GtkWidget *tileWidget = 0;

                auto i = slotWidgets.find (slot->getName ());

                if (i != slotWidgets.end ()) {
                        slotWidget = i->second;
                }
                else {
                        throw Core::Exception ("No such slot [" + slot->getName () + "]");
                }

                auto j = tiles.find (slot->getTileName ());

                if (j != tiles.end ()) {
                        GtkTile *gtkTile = j->second;
                        tileWidget = GTK_WIDGET (gtkTile->getUi ());
                }
                else {
                        continue;
                }

                // Throw away old child ... ???
                GtkWidget *oldChild = gtk_bin_get_child (slotWidget);

                if (oldChild) {
                        gtk_container_remove (GTK_CONTAINER (slotWidget), oldChild);
                }

                // Add new.
                GtkWidget *oldParent = 0;
                if ((oldParent = gtk_widget_get_parent (tileWidget))) {
                        gtk_widget_reparent (tileWidget, GTK_WIDGET (slotWidget));
                }
                else {
                        gtk_container_add (GTK_CONTAINER (slotWidget), tileWidget);
                }

                BOOST_LOG (lg) << "Reparented : tile [" << (void *)tileWidget << "], to slot [" << (void*)slotWidget << "]";
        }
}

/*--------------------------------------------------------------------------*/

GtkView::SlotWidgetMap GtkView::getSlotWidgets (SlotVector const &slots)
{
        SlotWidgetMap slotWidgets;

        for (Slot *slot : slots) {
                if (slotWidgets.find (slot->getName ()) != slotWidgets.end ()) {
                        throw Core::Exception ("There are either two Tiles with the same slot-name : [" + slot->getName () + "], or two widgets with this same name.");
                }

                // Throws if not found.
                GObject *obj = getUi (slot->getName ());

                if (obj) {
                        if (!GTK_IS_BIN (obj)) {
                                throw Core::Exception ("Slot widgets have to be of type GtkBin. Your slot [" + slot->getName () + "] is not a GtkBin.");
                        }

                        slotWidgets[slot->getName ()] = GTK_BIN (obj);
                }
        }

        return slotWidgets;
}

/*--------------------------------------------------------------------------*/

void GtkView::Impl::onIterateWidget (GtkWidget *widget, gpointer data)
{
        std::cerr << gtk_buildable_get_name (GTK_BUILDABLE (widget)) << std::endl;

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkView::Impl::onIterateWidget, 0);
        }
}

/*--------------------------------------------------------------------------*/

void GtkView::getInputs (std::string const &dataRange)
{
        GtkBuildable *mainWidget = GTK_BUILDABLE (getUi ());

        std::cerr << gtk_buildable_get_name (mainWidget) << std::endl;

        if (!GTK_IS_CONTAINER (getUi ())) {
                return;
        }

        gtk_container_foreach (GTK_CONTAINER (mainWidget), &GtkView::Impl::onIterateWidget, (gpointer)this);

#if 0
        GList *children, *iter;

        children = gtk_container_get_children(GTK_CONTAINER(container));
        for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
        g_list_free(children);

        // -----

        if(GTK_IS_CONTAINER(widget)) {
                GList *children = gtk_container_get_children(GTK_CONTAINER(widget));
                ...
        }
        If the widget is a GtkBin it has only one child. In that case, the following is simpler than dealing with a GList:

        if(GTK_IS_BIN(widget)) {
                GtkWidget *child = gtk_bin_get_child(GTK_BIN(widget));
                ...
        }

#endif
}

/*--------------------------------------------------------------------------*/

void GtkView::Impl::onPrintWidget (GtkWidget *widget, gpointer data)
{
        int *indent = static_cast <int *> (data);
        std::string id;

        for (int i = 0; i < *indent; ++i) {
                id += " ";
        }

        BOOST_LOG (lg) << id << gtk_buildable_get_name (GTK_BUILDABLE (widget));

        if (GTK_IS_CONTAINER (widget)) {
                int newIndent = *indent + 1;
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkView::Impl::onPrintWidget, &newIndent);
        }
}

/*--------------------------------------------------------------------------*/

void GtkView::printStructure ()
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "UI is not of type GtkBuildable. Can not get ID then.";
        }

        GtkBuildable *mainWidget = GTK_BUILDABLE (getUi ());

        BOOST_LOG (lg) << gtk_buildable_get_name (mainWidget);

        if (!GTK_IS_CONTAINER (getUi ())) {
                return;
        }

        int indent = 1;
        gtk_container_foreach (GTK_CONTAINER (mainWidget), &GtkView::Impl::onPrintWidget, &indent);
}

} // namespace GtkForms