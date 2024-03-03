// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <final/final.h>

#include "CpuDialog.h"
#include "MemoryDialog.h"

class MainDialog : public finalcut::FDialog
{
public:
    MainDialog(finalcut::FApplication *app);

    std::string getUptime();

    void onTimer(finalcut::FTimerEvent *e) override;
    void onShow(finalcut::FShowEvent *e) override;

private:
    CpuDialog *m_cpuDialog = nullptr;
    MemoryDialog *m_memoryDialog = nullptr;

    finalcut::FLabel *m_about;
    finalcut::FLabel *m_uptime;

    int m_uptimeTimer;
};
