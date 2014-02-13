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

std::string LoginController::start ()
{
        // Ona tworzy obiekt mdoelu.
        std::shared_ptr <LoginForm> loginForm = std::make_shared<LoginForm>();

        /*
         * Ustawia go w kontekście. kiedy kontekst flash przestanie istnieć, automatyczny wskaźnik
         * skasuje ten form. Flash powinien istniec tylko podczas przekazywania danych z kontrolera na
         * widok i odwrotnie. Kontroler przekazuje flash do widoku, widok pobiera z niego dane i go kasuje.
         * Tworzy nowy flash, ustawia tam jakieś rzeczy i przekazuje do do kontrolera, Kontroler pobiera
         * dane i znów go kasuje.
         */
        getFlashScope ()["form"] = Core::Variant (loginForm);

        return "->loginPage";
}

/*
 * Po konwersji wykonuje się metoda IController::onSubmit:
 */
std::string LoginController::onSubmit ()
{
        std::shared_ptr <LoginForm> form = vcast <std::shared_ptr <LoginForm>> (getFlashScope()["form"]);
//        bool loginOk = loginService->checkLogin (form->login, form->password);
//
//        if (!loginOk) {
//                getFlashScope ()["errors"] = Core::Variant ("Błędny login lub hasło");
//                // Wymuś akcję view->model2View ("tylko dla errors"), co poinno zaskutkowac natychmiastowym wyświetleniem się komunikatu.
//                return "loginPage";
//        } else {
//                getFlashScope ()["feedback"] = Core::Variant ("Zalogowano prawidłowo");
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
