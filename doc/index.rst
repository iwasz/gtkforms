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

Functionality list
==================
Those are main building blocks of a GtkForms app:

Data binding
------------
This term refers to converting model objects to values appropriate for display in GTK form, and back
from GTK form to model object. Model object fields are converted one by one depending on what set of
data is required on the view. For instance if one has a view with single GtkEntry which is bind to some
field of complex model object, only this one value should be converted and the others left intact.

Controller management
---------------------

Yes

View management
----------------

Views are created and destroyed.

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

