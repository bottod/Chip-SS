#include "chipscheck.h"


ChipsCheck::ChipsCheck()
{
}

PayDlg *ShowPayDlg(double Sum,int* KindChip,QString *ChipNames)
{
    PayDlg *m_PayDlg = new PayDlg(Sum,KindChip,ChipNames);
    return m_PayDlg;
}
