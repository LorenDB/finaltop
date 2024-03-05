// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "MainDialog.h"

MainDialog::MainDialog(finalcut::FApplication *app)
    : finalcut::FDialog(app),
      m_processList{new ProcessList{this}},
      m_cpuDialog{new CpuDialog{this}},
      m_memoryDialog{new MemoryDialog{this}}
{
    setText("finaltop");
    setGeometry({2, 2}, {30, 5});

    m_about = new finalcut::FLabel("finaltop v0.0.1", this);
    m_about->setGeometry({3, 1}, {30, 1});

    m_uptime = new finalcut::FLabel("System uptime: " + getUptime(), this);
    m_uptime->setGeometry({3, 2}, {30, 1});
    m_uptimeTimer = addTimer(1000);

    auto menubar = new finalcut::FMenuBar{this};
    auto windowMenu = new finalcut::FMenu{"&Window", menubar};

    auto cpuAction = new finalcut::FMenuItem{"&CPU", windowMenu};
    auto memoryAction = new finalcut::FMenuItem{"&Memory", windowMenu};
    auto uptimeAction = new finalcut::FMenuItem{"&Uptime", windowMenu};
    auto processListAction = new finalcut::FMenuItem{"&Process List", windowMenu};

    cpuAction->addCallback("clicked", [this, cpuAction] {
        m_cpuDialog->show();
        m_cpuDialog->setFocus();
    });
    memoryAction->addCallback("clicked", [this, memoryAction] {
        m_memoryDialog->show();
        m_memoryDialog->setFocus();
    });
    uptimeAction->addCallback("clicked", [this, uptimeAction] {
        show();
        setFocus();
    });
    processListAction->addCallback("clicked", [this, processListAction] {
        m_processList->show();
        m_processList->setFocus();
    });

    auto quitMenu = new finalcut::FMenuItem{"&Quit", menubar};
    quitMenu->addCallback("clicked", [this] {
        quit();
    });

    setMenuBar(menubar);
}

void MainDialog::onTimer(finalcut::FTimerEvent *e)
{
    if (e->getTimerId() == m_uptimeTimer)
    {
        m_uptime->setText("System uptime: " + getUptime());
        m_uptime->redraw();
    }
}

void MainDialog::onShow(finalcut::FShowEvent *e)
{
    setFocus();

    m_cpuDialog->show();
    m_memoryDialog->show();
    m_processList->show();
}

void MainDialog::onClose(finalcut::FCloseEvent *e)
{
    hide();
    e->ignore();
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
