#pragma once

#include <final/final.h>

class ProcessList : public finalcut::FDialog
{
public:
    ProcessList(finalcut::FWidget *parent);

    void parseProcessList();

    void onClose(finalcut::FCloseEvent *e) override;

private:
    finalcut::FListView *m_processList;
};

