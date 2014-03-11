/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <memory>
#include "LoginController.h"
#include "LoginForm.h"
#include "Logging.h"
#include "Employee.h"

using namespace Core;
static src::logger_mt& lg = logger::get();

std::string LoginController::start ()
{
        // Ona tworzy obiekt mdoelu.
        std::shared_ptr <LoginForm> form = std::make_shared<LoginForm>();

        form->login = "login";
        form->password = "password";;
        form->encrypt = true;
        form->number = 12.3;

        /*
         * Ustawia go w kontekście. kiedy kontekst flash przestanie istnieć, automatyczny wskaźnik
         * skasuje ten form. Flash powinien istniec tylko podczas przekazywania danych z kontrolera na
         * widok i odwrotnie. Kontroler przekazuje flash do widoku, widok pobiera z niego dane i go kasuje.
         * Tworzy nowy flash, ustawia tam jakieś rzeczy i przekazuje do do kontrolera, Kontroler pobiera
         * dane i znów go kasuje.
         */
        getUnitScope ()["form"] = Variant (form);

        employees.push_back (new Employee {"Łukasz", "Iwaszkiewicz", "Mokotów" });
        employees.push_back (new Employee {"Joanna", "Szenajch", "Praga" });
        employees.push_back (new Employee {"Franek", "Iwaszkiewicz", "Śródmiescie" });
        getUnitScope ()["employees"] = Variant (&employees);

        app->refresh ("", "");

        return "->loginPage";
}

/*
 * Po konwersji wykonuje się metoda IController::onSubmit:
 */
std::string LoginController::onSubmit ()
{
        std::shared_ptr <LoginForm> form = vcast <std::shared_ptr <LoginForm>> (getUnitScope()["form"]);
//        LoginForm *form = vcast <LoginForm *> (getUnitScope()["form"]);
//        BOOST_LOG(lg) << form.get ();

        BOOST_LOG (lg) << "Submited data. login : [" << form->login <<
                          "], password : [" << form->password <<
                          "], encrypt : [" << form->encrypt <<
                          "], number : [" << form->number<< "]";

        bool loginOk = (form->login == "admin" && form->password == "zaq12wsx");

        if (!loginOk) {
                getUnitScope ()["errors"] = Variant ("Błędny login lub hasło");
                getUnitScope ()["feedback"] = Variant ("");
                // Wymuś akcję view->model2View ("tylko dla errors"), co poinno zaskutkowac natychmiastowym wyświetleniem się komunikatu.
                app->refresh ("", "errors");
                app->refresh ("", "feedback");
                return "";
        } else {
                getUnitScope ()["errors"] = Variant ("");
                getUnitScope ()["feedback"] = Variant ("Zalogowano prawidłowo");
                app->refresh ("", "errors");
                app->refresh ("", "feedback");

//                // Wymuś przejście na kontroler MainWindowController. Tej samej metody uzyją widoki żeby przejść
//                // na jakis inny unit : onClicked : $app->start ();
//                getApp ()->start ("main");
                return "";
        }
}

std::string LoginController::end ()
{
        // Don't need to return page to be closed, because next page will replace current page.
        return "";
}

void LoginController::buttonClicked (unsigned int column, Core::Variant selectedObject)
{
        void *p = vcast <void *> (selectedObject);
        Employee *e = static_cast <Employee *> (p);
        BOOST_LOG (lg) << "LoginController::buttonClicked. Column : [" << column << "], selectedObject : [" << *e << "]";
}
