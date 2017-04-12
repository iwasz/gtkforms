/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef FILE_CHOOSER_ADAPTER_H
#define FILE_CHOOSER_ADAPTER_H

#include "AbstractSignalAdapter.h"

namespace GtkForms {

/**
 * 0 : responseId
 * 1 : filePath
 * @brief The FileChooserAdapter class
 */
class __tiliae_reflect__ FileChooserAdapter : public AbstractSignalAdapter {
public:
        virtual ~FileChooserAdapter () {}
        Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const;
        virtual std::string getSignal () const { return "response"; }

};

} // namespace

#endif // FileChooserAdapter_H
