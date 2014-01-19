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
a frontend).

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

Data binding
------------
This term refers to converting model objects to values appropriate for display in GTK form, and back
from GTK form to model object. Model object fields are converted one by one depending on what set of
data is required on the view. For instance if one has a view with single GtkEntry which is bind to some
field of complex model object, only this one value should be converted and the others left intact.

Application flow
----------------


Controller management
---------------------

Yes

View management
----------------

Views are created and destroyed. 

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

Forms
-----
Formularze mozna submitować. Mamy na przykład GtkContainer, a w nim mamy 2 pola GtkEntry o nazwach 
form.login i form.pasword. Pod nimi, w tym samym kontenerze mamy guzik, o nazwie submit.loginController.
Podczas ładowania się widoku następuje łączenie sygnałow w ten sposób, że ów guzik podłączony zostanie 
do funkcji, która wykona submit: ::

GtkView::show ()
{
        getApp ()->submit (this->GtkView::getName () /*widok ma nazwę */, "", controllerName /* nazwa kontrolera to jest to co jest po 'submit'*/);
}

Pusty argument to jest dataRange. Pusty oznacza, że chcemy konwertować wszystko co jest w formularzu.
Funkcja musi przeszukać zadziałać tak.


Dla widoku o nazwie viewName:
Znaleźć wszystkie guziki, których nazwa zaczyna się od "submit.". Odciąć to co jest po kropce i zapisać
to w zmiennej controllerName. Następnie pobrać kontener GtkContainer w którym jest ten guzik. Potem przeiterować
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
trafić ten submitEvent.

Model - view mapping
--------------------
Istnieje coś takiego jak IAccessor. To hermetyzuje sposób w jaki jest pobierana wartość z formularza
i w jaki sposób jest zapisywana. Domyślny Accessor działa tak, że pobiera i ustawia dane z dla danego
propery (bo widgety Gtk mają property. Na przykład GtkEntry ma property "text" i to jest zawartość
tego GtkEntry.


Session
-------

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

