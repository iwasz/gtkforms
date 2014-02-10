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

        /*
         * Albo 1. converter/mapper zmapuje to na widoku.
         * albo 2. jakiś toolbar controller weźmie tą mapę, (albo serwis) i ręcznie GTK+ ukryje guziczki.
         */
        Core::VariantMap buttons;
        buttons["power"] = Core::Variant (true);
        buttons["logout"] = Core::Variant (false);
        buttons["lock"] = Core::Variant (false);
        buttons["home"] = Core::Variant (false);
        getUnitScope ()["buttons"] = Core::Variant (buttons);

        return "+loginPage";
}

/*
 * Po konwersji wykonuje się metoda IController::onSubmit:
 */
std::string LoginController::onSubmit ()
{
        std::shared_ptr <LoginForm> form = vcast <std::shared_ptr <LoginForm>> (getFlashScope()["form"]);
        bool loginOk = loginService->checkLogin (form->login, form->password);

        if (!loginOk) {
                getFlashScope ()["errors"] = Core::Variant ("Błędny login lub hasło");
                // Wymuś akcję view->model2View ("tylko dla errors"), co poinno zaskutkowac natychmiastowym wyświetleniem się komunikatu.
                return "loginPage";
        } else {
                getFlashScope ()["feedback"] = Core::Variant ("Zalogowano prawidłowo");

                // Wymuś przejście na kontroler MainWindowController. Tej samej metody uzyją widoki żeby przejść
                // na jakis inny unit : onClicked : $app->start ();
                getApp ()->start ("main");
                return "";
        }
}

std::string LoginController::end ()
{
        // Don't need to return page to be closed, because next page will replace current page.
        return "";
}

/*
 * Jest też na widoku taka możliwość odpalenia klawiatury ekranowej. Ta funkcja kontrolera jest handlerem sygnału
 * z widoku (powiedzmy kliknięcie guzika z klawiaturką, czy on focus z pola).
 */
void LoginController::onScreenKeyboard ()
{
        /*
         * Zamiast nazwy start mogła by by c nazwa sugerująca zmianę obiektu, który aktualnie
         * wykonuje jakies zadanie. Bo raz jest to widok, a raz kontroler. Propozycje:
         * - goTo? Ale w sumie to jest uruchomienie, więc start jest też OK.
         *
         * To zadziała tak jak zadziałao uruchomienie gółownego okna:
         * - Sprawdzenie, czy coś takiego jest załadowae
         * - Jesli nie, to załadowanie kontrolera KeyboardController, a kontroler załaduje widok i potem widok z kontrolerem będą się przerzucac danymi.
         * - te jednostki będą się nazywac ... unit? action? page?
         */
        getApp ()->join ("numericKeyboard");

        /*
         * Podłączamy sygnał, który klawiatura ekranowa wysyła kiedy user wpisze tekst i kliknie OK.
         */
//        /*Listener */keyboardFinishListener = std::make_shared < Listener > (this, "$onKeyboardFinish (%0, %1)", "keyboardFinish");
}

void LoginController::onKeyboardFinish (std::string const &field, std::string const &text)
{
        getFlashScope ()[field] = Core::Variant (text);

        /*
         * To jest customowa metoda (nie z interfejsu IController), więc zwrócenie nazwy widoku nic nie da.
         * Musimy więc uruchomić model2View.
         */

//        To musi tu być
//        getApp ()->model2View();
}

/*
 * UWAGA! Druga opcja, która blokuje okno pod spodem:
 * Do przemyślenia, czy to się w ogóle opłaca, ale raczej będzie za trudne, a
 * narazie nie jest potrzebne.
 */
//void LoginController::onscreenKeyboard1 ()
//{
//        /*
//         * Parametr block może też być podawany w konfiguracji.
//         */
//        app->merge ("numericKeyboard", BLOCK);
//        KeyboardResult *kb = vcast<KeyboardResult *>(getUnitScope()["keyboardResult"]);
//        getFlash()[kb->getField()] = kb->getText();
//        app->model2View();
//}
