/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TABLEMAPPING_H_
#define TABLEMAPPING_H_

#include <core/ApiMacro.h>
#include <reflection/Reflection.h>
#include "mapping/IMapping.h"
#include "mapping/MapperDTO.h"

namespace GtkForms {

/**
 * Cell mapping.
 */
class Cell {
public:
        __c (void)

        int getViewRow () const { return viewRow; }
        _m (setViewRow) void setViewRow (int viewRow) { this->viewRow = viewRow; }

        std::string const &getModelProp () const { return modelProp; }
        _m (setModelProp) void setModelProp (std::string const &modelProp) { this->modelProp = modelProp; }

private:

        int viewRow;
        std::string modelProp;
        _e (Cell)
};

typedef std::vector <Ptr <Cell> > CellVector;
_g (CellVector)

/**
 *
 */
class TILIAE_API TableMapping : public IMapping {
public:

        __c (void)
        virtual ~TableMapping () {}

        virtual void m2v (MapperDTO *dto);
        virtual void v2m (MapperDTO *dto);

/*--------------------------------------------------------------------------*/

        /**
         * Where to get the ListStore, or TreeStore objects from.
         */
        std::string const &getStorePath() const { return storePath; }
        _m (setStorePath) void setStorePath(std::string storePath) { this->storePath = storePath; }

        /**
         * What model element (sigle row object) properties are mapped to what
         * row numbers in ListStore.
         */
        Ptr <CellVector> getCells () const { return cells; }
        _m (setCells) void setCells (Ptr <CellVector> cells) { this->cells = cells; }

private:

        std::string storePath;
        Ptr <CellVector> cells;

        _e (TableMapping)
}

}

#	endif /* TABLEMAPPING_H_ */
