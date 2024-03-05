// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "CpuDialog.h"

CpuDialog::CpuDialog(finalcut::FWidget *parent)
    : finalcut::FDialog(parent)
{
    setText("CPU");
    setGeometry({2, 9}, {60, 7});

    m_cpuMeter = new finalcut::FProgressbar{this};
    m_cpuMeter->setGeometry({3, 2}, {50, 1});
    parseCpuUsage();

    m_cpuInfo = new finalcut::FLabel{this};
    m_cpuInfo->setGeometry({3, 4}, {60, 1});

    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line))
        if (line.starts_with("model name"))
            break;
    m_cpuInfo->setText(line.substr(line.find(':') + 2));

    m_cpuUsageTimer = addTimer(500);
}

void CpuDialog::parseCpuUsage()
{
    std::ifstream proc_stat("/proc/stat");
    std::string cpuLine;
    std::getline(proc_stat, cpuLine);
    std::string cpu;
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
    std::stringstream cpuStream(cpuLine);
    cpuStream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
    double totalTime = user + nice + system + idle + iowait + irq + softirq + steal + guest + guestnice;
    double activeTime = user + nice + system + irq + softirq + steal + guest + guestnice;
    m_cpuMeter->reset();
    m_cpuMeter->setPercentage(((activeTime - m_activeCpuLastTick) / (totalTime - m_totalCpuLastTick)) * 100);
    m_totalCpuLastTick = totalTime;
    m_activeCpuLastTick = activeTime;
}

void CpuDialog::onTimer(finalcut::FTimerEvent *e)
{
    if (e->getTimerId() == m_cpuUsageTimer)
    {
        parseCpuUsage();
        m_cpuMeter->redraw();
    }
}

void CpuDialog::onClose(finalcut::FCloseEvent *e)
{
    hide();
    e->ignore();
}
