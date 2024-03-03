// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <final/final.h>

class CpuDialog : public finalcut::FDialog
{
public:
    CpuDialog(finalcut::FWidget *parent);

    void parseCpuUsage();

    void onTimer(finalcut::FTimerEvent *e) override;
    void onClose(finalcut::FCloseEvent *e) override;

private:
    finalcut::FProgressbar *m_cpuMeter;
    finalcut::FLabel *m_cpuInfo;
    int m_cpuUsageTimer;

    double m_totalCpuLastTick = -1;
    double m_activeCpuLastTick = -1;
};
