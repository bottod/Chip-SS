#ifndef QTSQLDLL_GLOBAL_H
#define QTSQLDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTSQLDLL_LIBRARY)
#  define QTSQLDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTSQLDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTSQLDLL_GLOBAL_H
