#ifndef CHIPSCHECK_H
#define CHIPSCHECK_H

#include "chipscheck_global.h"
#include "paydlg.h"


class CHIPSCHECKSHARED_EXPORT ChipsCheck
{

public:
    ChipsCheck();
};
extern "C" CHIPSCHECKSHARED_EXPORT PayDlg *ShowPayDlg(double Sum,int* KindChip,QString *ChipNames);

#endif // CHIPSCHECK_H
