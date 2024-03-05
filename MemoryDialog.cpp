// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "MemoryDialog.h"

MemoryDialog::MemoryDialog(finalcut::FWidget *parent)
    : finalcut::FDialog(parent)
{
    setText("RAM");

    auto memLabel = new finalcut::FLabel{"RAM:", this};
    memLabel->setGeometry({1, 2}, {5, 1});

    m_ramMeter = new finalcut::FProgressbar{this};
    m_ramMeter->setGeometry({7, 2}, {30, 1});
    m_ramLabel = new finalcut::FLabel{this};
    m_ramLabel->setGeometry({40, 2}, {15, 1});

    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    while (std::getline(meminfo, line))
        if (line.starts_with("SwapTotal:"))
            break;
    std::string dummy;
    int swapTotal;
    std::stringstream ss(line);
    ss >> dummy >> swapTotal;
    if (swapTotal > 0)
    {
        setGeometry({2, 18}, {60, 9});
        auto swapLabel = new finalcut::FLabel{"Swap:", this};
        swapLabel->setGeometry({1, 5}, {5, 1});
        m_swapMeter = new finalcut::FProgressbar{this};
        m_swapMeter->setGeometry({7, 5}, {30, 1});
        m_swapLabel = new finalcut::FLabel{this};
    }
    else
        setGeometry({2, 18}, {60, 6});
    parseMemUsage();

    m_memUsageTimer = addTimer(500);
}

void MemoryDialog::parseMemUsage()
{
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    std::string dummy;
    double total, available, swapTotal, swapFree;
    while (std::getline(meminfo, line))
    {
        if (line.starts_with("MemTotal:"))
        {
            std::stringstream ss(line);
            ss >> dummy >> total;
        }
        else if (line.starts_with("MemAvailable:"))
        {
            std::stringstream ss(line);
            ss >> dummy >> available;
        }
        else if (line.starts_with("SwapTotal:"))
        {
            std::stringstream ss(line);
            ss >> dummy >> swapTotal;
        }
        else if (line.starts_with("SwapFree:"))
        {
            std::stringstream ss(line);
            ss >> dummy >> swapFree;
        }
    }

    auto setUsageLabel = [](finalcut::FLabel *label, double usage, double total) {
        auto usageStr = std::to_string(usage / 1048576);
        auto totalStr = std::to_string(total / 1048576);
        usageStr = usageStr.substr(0, usageStr.find('.') + 2);
        totalStr = totalStr.substr(0, totalStr.find('.') + 2);
        while (usageStr.size() < 4)
            usageStr = " " + usageStr;
        while (totalStr.size() < 4)
            totalStr = " " + totalStr;

        label->setText(usageStr + " GB/" + totalStr + " GB");
        label->redraw();
    };

    m_ramMeter->reset();
    m_ramMeter->setPercentage((total - available) / total * 100);
    m_ramMeter->redraw();
    setUsageLabel(m_ramLabel, total - available, total);
    if (m_swapMeter)
    {
        m_swapMeter->reset();
        m_swapMeter->setPercentage((swapTotal - swapFree) / swapTotal * 100);
        m_swapMeter->redraw();
        setUsageLabel(m_swapLabel, swapTotal - swapFree, swapTotal);
    }
}

void MemoryDialog::onTimer(finalcut::FTimerEvent *e)
{
    if (e->getTimerId() == m_memUsageTimer)
    {
        parseMemUsage();
        m_ramMeter->redraw();
    }
}

void MemoryDialog::onClose(finalcut::FCloseEvent *e)
{
    hide();
    e->ignore();
}
