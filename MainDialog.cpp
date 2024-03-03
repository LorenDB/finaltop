// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "MainDialog.h"

MainDialog::MainDialog(finalcut::FApplication *app)
    : finalcut::FDialog(app),
      m_cpuDialog{new CpuDialog{this}},
      m_memoryDialog{new MemoryDialog{this}}
{
    setText("finaltop");
    setGeometry({4, 2}, {30, 5});

    m_about = new finalcut::FLabel("finaltop v0.0.1", this);
    m_about->setGeometry({3, 1}, {30, 1});

    m_uptime = new finalcut::FLabel("Uptime: " + getUptime(), this);
    m_uptime->setGeometry({3, 2}, {30, 1});
    m_uptimeTimer = addTimer(1000);
}

void MainDialog::onTimer(finalcut::FTimerEvent *e)
{
    if (e->getTimerId() == m_uptimeTimer)
    {
        m_uptime->setText("Uptime: " + getUptime());
        m_uptime->redraw();
    }
}

void MainDialog::onShow(finalcut::FShowEvent *e)
{
    setFocus();

    m_cpuDialog->show();
    m_memoryDialog->show();
}

std::string MainDialog::getUptime()
{
    std::ifstream proc_uptime("/proc/uptime");
    double uptime;
    proc_uptime >> uptime;
    int hours, minutes, seconds;
    hours = static_cast<int>(uptime / 3600);
    minutes = static_cast<int>((uptime - hours * 3600) / 60);
    seconds = static_cast<int>(uptime - hours * 3600 - minutes * 60);
    return std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}
