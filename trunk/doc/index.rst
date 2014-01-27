.. GtkForms documentation master file, created by
   sphinx-quickstart on Wed Jan 15 16:42:27 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to GtkForms's documentation!
====================================

Contents:

.. toctree::
   :maxdepth: 2


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

Model, view and controller communication
========================================

There are only two options here: 

1. *Data binding* is performed during form submission just like any other MVC framework does. Data put
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
on the window, making virtually entire vindow a form. GtkForms distingusig inputs and other GtkWindgets by
name. **Every widget whise name starts with an exclamation mark is an input**.  

To submit a form you must invoke special function : ``$app->submit (...)```. You may do it specifying 
a callback of some button, or you may do it programmaticaly in your custom ``IView`` implementation (I mean
concrete class derived from ``IView``. Example of a callback may look like this: ::
 
 $app->submit ('loginView', '', 'loginController')

Arguments of this call have the following meaning:

1. ``'loginView'`` is a name ov a view from data will be get. It may be view which contains the button shich was used
   to submit a form, or some other view available in current ``Page``. If current page does not contain view with given 
   name, an exception will be thrown.
#. ``''`` second argument in above example, which is an empty string is called a *dataRange*. It tells the library which
   part of the form to process. Data ranges can contain special wildcard character \*. Emty data range means that we
   want to get all input fields from the form (i.e. all ``GtkWidgets`` whise name starts with an ``!``). 
#. ``'loginController'`` is a name of controller to submit converted data to.

Dla widoku o nazwie viewName:
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


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

.. doxygenstruct:: IController


 