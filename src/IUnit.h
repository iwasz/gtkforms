/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IUNIT_H_
#define IUNIT_H_

namespace GtkForms {

/**
 * Stores information which controllers were added and which were removed durnig App operatons
 * (statr, join and split).
 */
struct UnitOperationResult {
        ControllerMap added;
        ControllerMap removed;

        UnitOperationResult &operator += (UnitOperationResult const &);
};

/**
 * Why interface? Beczause one could want to reimplement some functionality of
 * units. Imagine for example units that are conditionally enabled and could
 * provide debig stuff or sth.
 */
class IUnit : public Core::Object {
public:
        virtual ~IUnit () {}

        virtual UnitOperationResult start (IUnit *unit) = 0;
        virtual UnitOperationResult join (IUnit *unit) = 0;
        virtual UnitOperationResult split (IUnit *unit) = 0;
        virtual IController *getController (std::string const &controllerName) = 0;
        virtual ControllerMap &getControllers () = 0;

};

} // namespace GtkForms

#endif /* IUNIT_H_ */
