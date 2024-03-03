// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <final/final.h>

class MemoryDialog : public finalcut::FDialog
{
public:
    MemoryDialog(finalcut::FWidget *parent);

    void parseMemUsage();

    void onTimer(finalcut::FTimerEvent *e) override;
    void onClose(finalcut::FCloseEvent *e) override;

private:
    finalcut::FProgressbar *m_ramMeter = nullptr;
    finalcut::FProgressbar *m_swapMeter = nullptr;
    finalcut::FLabel *m_ramLabel = nullptr;
    finalcut::FLabel *m_swapLabel = nullptr;
    int m_memUsageTimer;
};
