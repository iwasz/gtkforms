.. GtkForms documentation master file, created by
   sphinx-quickstart on Wed Jan 15 16:42:27 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to GtkForms's documentation!
====================================

Contents:

.. toctree::
   :maxdepth: 2

Nowe rzeczy
===========
* Klasa Mapping ma pole *widget* a nie *input*.
* Domyślne property dla niektórych widgetów. Dzięki temu nie trzeba podawać mappingów. Jeżeli na
* Nazwy widgetów podajemy w Glade w polu "name" a nie w polu ID. W konfiguracji można włączyć, żeby czytał także z ID.
  Jeżeli włączona jest opcja z ID, to czyta także z ID, ale name ma zawsze pierwszeństwo.
* Jeśli nazwa widgetu (w ID lub w name) jest taka sama jak nazwa modelu, to to się automatycznie zmapuje
        * Dzięki tym zmianon minimalizuje się liczbę potrzebnych mappingów explicite w XML. Najczęściej wystarczą
          domyślne mapowania.
* Oczywiście można podawać mapowania explicite. Jest to przydatne gdy:
        * Domyślne mapowanie nie wystarcza, bo, przykładowo, zamiast napisu w labelu chcemy zmieniać jego kolor.
        * Chcemy mapować więcej niż jedno propery dla danego widgetu. Na przykład mamy toggle button i chcemy zmieniać
          zarówno jego napis, jak i jego aktywność (wciśnięty, niewciśnięty).
        * Nie ma domyślnego mapowania (na przykład dla tabel).
        * Chcemy użyć konwertera.

* Uproszczona struktura widoków - jeden kontroler = jeden widok i jeden scope.
* Struktura drzewiasta i to tylko w kontrolerze. Od tej struktury zależy struktura widoków i scopów (map na modele).
* Nie ma setToUnitScope, setToFlashScope, tylko po prostu set/get, które działają hiertarchicznie.
* Opisać w dokumentacji że w AbstractView można podać albo file (ścieżka), albo uiFIle (referencja do obiektu. W tym pierwsyzm
  wypadku GtkView będzie przy każdym load ładował plik UI. A w tym drugim uiFile może być singletonem i załaduje siętylko raz.

UWAGA!!!!!
If you create a gtk.Window and close it using the window manager (WM) --- in other words, by clicking on a close or X button on the window decorations, or using some other way the WM provides --- the window by default is destroyed.

W tym momencie wydaje mi się, że trzeba użyć gtk_builder_add_objects_from_file () i połączyć GtkView z UiFile (wywalić UiFile). Wygląda na to, że
GtkBuilder tworzy **singletony**. Znaczy to, że wołając gtk_builder_get_object zawsze dostanę wskaźnik do twgo samego obiektu. Co chyba oznacza że:
* Żeby stworzyć 2 okna, potrzebuję 2 instancje GtkBuilder.
* Po zamknięciu okna stworzonego z GtkBuilder, wywołanie gtk_builder_get_object zwróci mi wskaźńik do tego zniszczonego okna.
Niestety nie mogę znaleźć 100% potwierdzenia. Ale jeśli mam rację, to GtkView (==GtkWindow) pownno mieć relację 1 do 1 z GtkBulder, czyli pinno zawirać
swój. Jeśli okno zostanie zamknięte, to builder powinien zostać zniszczony. Ze starych notatek wynika to samo : "W ogóle, to GtkBuilder ZAWSZE zwroci tą
samą instancję obiektu (po ID). Nawet kiedy ten obiekt zniszczymy, to będzie zwracał wskaźnik to tego zniszczonego."

Do BuilderView - opcja podania dodatkowych obiektów do załadowania (treeModel, adjustment etc - rzeczy, które nie są niczyimi dziećmi). Albo dodatkowa
opcja do załadowania CAŁEGO pliku.
Uwaga! Sloty muszą mieć ID lub nazwę zaczynającą się od ^

Wyjaśnienie czemu w XML nie można dodawać child-kontrolerów jako referencje do innych kontrolerów. Otóż dodanie
child-kontrolera wymaga uruchomienia metody parent.open ('childName'). Tam się dzieje dużo rzeczy, które przy
zwykłym dodaniu referencji by się nie uruchomiły. No dobra, ale można by zrobić jakąś iterację po wektrze
dzieci i dla każdego uruchomić takie "open". Niby tak, ale pododuje to wiele niejasności. Przede wszystkim co
miałoby się stać z tymi child-kontrolerami kiedy zamykamy parent? Wektor ten nie powinien być wyczyszczony, bo
user będzie oczekiwał, że jak otworzy dany "parent", to zawsze otrzyma to co zdefiniował w XML (statycznie). Ale
w wypadku gdy nie będziemy czyścić listy child-kontrolerów przy zamykaniu parenta, to wówczas child-kontrolery
dodane do parenta dynamicznie też nie będą usuwane, co przecież jest bez sensu.


TODO
====
        * Zastanowić się nad globalnymi mapowaniami. Jak miałyby działać?
        * Uprościć ten submit - teraz trzeba w Glade podać aż 3 parametry do submit. Może da się zmniejszyć liczbę tych
          argumentów.
        * Wyjaśnić z tym _Bool
        * Żeby się pliki output gebnerowałuy za każdymn razem jak siępkompiluje.
        * Jak by się dało anotować prywtne pola, to by było super, ale chyba nie ma opcji. Wobec tego trzeba zorbić
          setery i getery tam gdzie trzeba (QTCreator ma opcję do generowania).
        * Dodać -Wall wszędzie
        * Przekompilować z anotacjami (tiliae) i sprawdzić czy działa.
        * Zarządzanie pamięcią w przypadku kontrolerów - żeby dało się ładnie tworzyć prototypy i żeby się same kasowały,
          a jednocześnie, żeby singletony sie nie kasowały - może jakiś sprytny rodzaj wskaźnika.
        * Żebyu metody open i close mogły być przeciążone (refleksja teraz nie chce działać jak są).
        * Nazwy wszedzie gdzie jest Input, to zamienić na "widget" lub "input widget".
        * Żeby można było robić submit z widoków bez podania kontrolera,
        * Żeby można było robić submit z widoków bez podania w ogóle niczego,
        * mapa sessionScope powinna być w App moim zdaniem. Analogicznie do controlerScope.
        * Zrobić komantarz nada klasą Context i wyjaśnić tam po co ona jest. Może zamienić na jakąś bardziej
          samo-opisującą się nazwę? ViewDTO? Nie wiem, bo nie za bardzo rozumiem po co on jest.
        * Kiedyś. Przejrzeć klasy pod kątem odpowiedzialności.
        * Kiedyś. Przejrzeć klasy pod kątem publicznego API.

TODO do przemyślenia
====================

        * Poczyścić, żeby nie generowało się tyle niepotrzebnych reczy dodać inne anotacje (?) chyba chodziło mi o metody,
          że każda publiczna metoda wskauje do refleksji - myślę, że to właśnie jest OK. Usunąć

TODO zrobione
=============

        * Dodać każdy plik w projekcie do źródeł explicite.
        * Page na View
        * Unit na controller poszukać, są te nazwy.

Plany
=====
controller
* open (lista po przewcinkach), replace (lista), close (this), close (lista)
* BuilderView file - plik, alsoLoad, loadWholeFile.

What it is
==========

It is a MVC library highly inspierd by some Java technologies that I used for years, most notably
Spring MVC and Struts. It is intended to help to make high level GUI applications (having GTK+ as
a frontend). There are two main areas that it covers:

1. Form submission and converting data between GTK widgets on the form and model objects. Conversion
works in both directions.
2. View loading, view display and composition views together to form bigger views. Composition of views
is performed automatically based on XML configuration. For example if you have main view with a empty
GtkFrame, you may configure it in sucha way, that if someone shows some another view (be it a window,
or smoe other container widget), it will be automatically embedded (reparented) into that frame. This
way users have only to wory about showing and hiding thier views when appropriate, and composition is
handled by the lib.

What it is not
==============

It is not another widget toolkit. It uses GTK for display views. Low level GUI stuff still has to
be done in GTK+ by hand.

Dependencies
============
1. GTK+ version 3.0+
#. Boost
#. libtiliae

How to compile
==============

Lets assume, that you unpacked the source to $GTK_FORMS_DIR. Lets cd to this directory: ::

cd $GTK_FORMS_DIR

To make sure that previous setup won't interfere with your changes perform (but if you build for the ferst time
you may skip this since there is no CMakeFiles folder and CMakeCache.txt yet): ::

cd build
rm -Rf CMakeFiles/ CMakeCache.txt

To build you can simply invoke: ::

cmake .

But you may customze a few things as well. First, you may specify where to install newly built files. Use ``-DCMAKE_INSTALL_PREFIX``
for this. Without this switch cmake would want to install gtkforms into default directory like ``/usr/lib`` which is OK for
most of the cases. Secondly you may to specify what kond of build that will be. There are "Debug" and "Release" options available.

cmake -DCMAKE_INSTALL_PREFIX:=/home/iwasz/local/ -DCMAKE_BUILD_TYPE=Debug .

You can use cmake-gui application if you wish, and set all available cmake options inside nice GUI. Among standard cmake
variables, one may set a few specific ones :

+------------------+-----------------------------+
| BUILD_LIB        | Build the GtkForms library. |
+------------------+-----------------------------+
| BUILD_TEST       | Build unit tests.           |
+------------------+-----------------------------+
| BUILD_CALCULATOR | Build first demo app.       |
+------------------+-----------------------------+
| BUILD_CATALOG    | Build second demo app.      |
+------------------+-----------------------------+

Simply invoke: ::

cmake-gui .

To run cmake-gui app.

Functionality list
==================
Those are main building blocks of a GtkForms app:

Application flow
----------------


Controller management
---------------------

Yes

View management
---------------

- Views are created and destroyed.
- Pages have to be defined as singletons in tiliae container.

Klasa IView ma nazwę. GtkView z daną nazwą będzie miała podpięty GtkBuilder i będzie z nigo wyciągać widget
o tej nazwie. To bedzie główny widget tego widoku. Poza tym niektóre widoki bedą miały konfigurację : tiles.
Ona będzie mówić który widget ma się wkleić w inny widget. Na przykład: ::

 <!-- W pliku *.ui będzie widget typu GtkWindow (lub inny GtkContainer) o nazwie loginView. Kiedy zrobimy
 view->show (), to on się załaduje i pokaże.
 <GtkView name="loginView" builder="@gtkBuilder" embed="">
         <Tile add-to="tiles" widget="toolbaView" targetWidget="sidePane"/>
         <Tile add-to="tiles" widget="headerView" targetWidget="upperPane"/>
 </GtkView>

 <GtkView name="toolbarView">
 <GtkView name="headerView">

Aktywna (aktualnie wyświetlana) jest tylko jedna strona na raz. Kiedy użytkownik zarząda nowej strony, ona
zastępuje tą starą, chyba, że ma ustawiony parametr ``joinable="true"`` i w takim przypadku nowa strona
**połączy się ze starą**. Aktualnie wyświetlaną stronę można też zamknąć zwracając jej nazwę z metody ``end``
kontrolera.

``loginPage.start ()``
-

Model, view and controller communication
========================================

There are only two options here:

1. *Data binding* is performed during form submission just like in any other MVC framework. Data put
   by the user into inputs (the term *input* is derived from web environment, where you place <input .../> elements
   inside <form>. In GTK+ application, and in GtkForms in particular inputs are simple GTK+ widgets with
   special name) is converted and passed further to your *model* (the term *model* refers to all so called business
   data you use. We split M, V and C here). Special beeings called *mappings* describe how to convert *input*
   values and where to put those values in the model.
#. *Expression language, K202*. You may use a simple `expression language <http://en.wikipedia.org/wiki/Unified_Expression_Language>`_
   called K202 to quickly connect signals on the view to methods of a controller. You simply write down simple expressions
   in Glade's (GTK+ gui editor) "signal" tab.

.. #. *Signals and slots* <- to jest komunikacja między kontrolerami.

Data binding and forms
----------------------

This term refers to converting model objects to values appropriate for display in GTK form, and back
from GTK form to model object.

.. Model object fields are converted one by one depending on what set of
data is required on the view. For instance if one has a view with single GtkEntry which is bind to some
field of complex model object, only this one value should be converted and the others left intact.


Form is a virtual beeing in GtkForms, because elements of a form (called inputs) may be laid out everywhere
on the window, making virtually entire vindow a form. GtkForms distingushig inputs and other GtkWindgets by
name. **Every widget whose name starts with an exclamation mark is an input**.

To submit a form you must invoke special function : ``$app->submit (...)```. You may do it specifying
a callback of some button, or you may do it programmaticaly in your custom ``IView`` implementation (I mean
concrete class derived from ``IView``. Example of a callback may look like this: ::

 $app->submit ('loginView', '', 'loginController')

Arguments of this call have the following meaning:

1. ``'loginView'`` is a name of a view from which data will be get. It may be view which contains the button which was used
   to submit a form, or some other view available in current ``Page``. If current page does not contain view with given
   name, an exception will be thrown.
#. ``''`` second argument in above example, which is an empty string is called a *dataRange*. It tells the library which
   part of the form to process. Data ranges can contain special wildcard character \*. Emty data range means that we
   want to get all input fields from the form (i.e. all ``GtkWidgets`` whise name starts with an ``!``).
#. ``'loginController'`` is a name of controller to submit converted data to.

Dla widoku o nazwie viewName (ten punkt nieaktualny):
Następnie pobrać kontener GtkContainer w którym jest ten guzik. Potem przeiterować
po widgetach w tym GtkContenerze i znaleźć wszystikie inne widgety. Z nich będzzie konwertował dane. Czyli
taki automatyczy formularz to jest GtkContainer, pola z danymi i guzik. Bierze każdy widget , p;obiera
jego nazwę viewProperty i próbuje dokonać konwersji:

1. próbuje zrobic value = view->getProperty (viewProperty)
2. mapper->view2Model (viewProperty, value);

To się może nie udać, bo na przykład nie da się pobrać wartości z jakiegoś widgetu dekoracyjnego typu
obrazek. Mogą być przecież widgty, które są na formularzu, ale

sposób 2

guzik ma podpieta akcję $app->submit ('loginView', 'form.*', 'loginController')

Czyli pierwszym argumentem jest nazwa widoku, potem jest dataRange, a potem nazwa kontrolera do którego ma
trafić ten submitEvent. Działa to tak:

1. App pobiera z aktualnego Page widok o podanej nazwie. Jeśli nie ma takiego widoku, to wyjątek.
#. Na podstawie dataRange jest podejmowana decyzja jakie wartości pobieramy z widoku. Są trzy możliwosći:
 a) dataRange jest szczegółowy, to znaczy jest tam konkretna nazwa jakiegoś widgetu na przykłąd form.login.
 #) dataRange zawiera gwiazdkę, na przykład form.*. Wówczas pobierane są wartości wszystkiech widgetów, których
 nazwa zaczyna się od form.
 #) data range jest puste - wówczas iterujemy po wszystkich widgetach z GtkContainera w którym znajduje się guzik,
 który wysłał SubmitEvent i używamy domyślnego accessora.

Model - view mapping
--------------------
Istnieje coś takiego jak IAccessor. To hermetyzuje sposób w jaki jest pobierana wartość z formularza
i w jaki sposób jest zapisywana. Domyślny Accessor działa tak, że pobiera i ustawia dane z dla danego
propery (bo widgety Gtk mają property. Na przykład GtkEntry ma property "text" i to jest zawartość
tego GtkEntry.

Mapowania. Widok ma podpięte mapowania. Może to wygladać jakoś tak: ::

 Patrz config.xml


Session (context)
-----------------

Like HttpSession. This is a storage place for various objects. Be it model objects, or view stuff. All
views and controllers have access to it. In fact data binding process looks for model objects just there.
Session is flat (i.e. some sort of std::map, or other simple container). Nested views, that is windows
which popped out from other windows, use the same level of session as root views.

Signals and callbacks
---------------------

User actions may emit signals which in turn invoke callbacks. Those are meant to be a controller's methods
which performs some custom operations (or more standard operations like data conversion). Callbacks may have
parameters which can be converted or raw.

Onscreen keyboard R&D
----------------------

https://wiki.gnome.org/action/show/Projects/Caribou?action=show&redirect=Caribou

FAQ
===
``Wrong vcast to shared_ptr<T>, typeid (T &) != v.ti, where typeid (T*) : N4Core9IIteratorE. Variant info : type=NONE, value=[], type_info=[NULL], object=false.``
From TableMapping : collection type may be not included in reflection system. User col_ macro.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

.. doxygenstruct:: IController



