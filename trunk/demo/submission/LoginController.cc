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

//        bool loginOk = loginService->checkLogin (form->login, form->password);
//
//        if (!loginOk) {
//                getFlashScope ()["errors"] = Variant ("Błędny login lub hasło");
//                // Wymuś akcję view->model2View ("tylko dla errors"), co poinno zaskutkowac natychmiastowym wyświetleniem się komunikatu.
//                return "loginPage";
//        } else {
//                getFlashScope ()["feedback"] = Variant ("Zalogowano prawidłowo");
//
//                // Wymuś przejście na kontroler MainWindowController. Tej samej metody uzyją widoki żeby przejść
//                // na jakis inny unit : onClicked : $app->start ();
//                getApp ()->start ("main");
//                return "";
//        }
        return "";
}

std::string LoginController::end ()
{
        // Don't need to return page to be closed, because next page will replace current page.
        return "";
}
