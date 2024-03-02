#include <iostream>

#include <final/final.h>

class MainDialog : public finalcut::FDialog
{
public:
    MainDialog(finalcut::FApplication *app)
        : finalcut::FDialog(app)
    {
        setText("finaltop");
        setGeometry({4, 2}, {30, 5});

        m_about = new finalcut::FLabel("finaltop v0.0.1", this);
        m_about->setGeometry({3, 1}, {30, 1});

        m_uptime = new finalcut::FLabel("Uptime: " + getUptime(), this);
        m_uptime->setGeometry({3, 2}, {30, 1});
        m_uptimeTimer = addTimer(1000);
    }

    void onTimer(finalcut::FTimerEvent *e) override
    {
        if (e->getTimerId() == m_uptimeTimer)
        {
            m_uptime->setText("Uptime: " + getUptime());
            m_uptime->redraw();
        }
    }

    std::string getUptime()
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

private:
    finalcut::FLabel *m_about;
    finalcut::FLabel *m_uptime;

    int m_uptimeTimer;
};

class CpuDialog : public finalcut::FDialog
{
public:
    CpuDialog(finalcut::FWidget *parent)
        : finalcut::FDialog(parent)
    {
        setText("CPU");
        setGeometry({4, 9}, {60, 7});

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

    void parseCpuUsage()
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

    void onTimer(finalcut::FTimerEvent *e) override
    {
        if (e->getTimerId() == m_cpuUsageTimer)
        {
            parseCpuUsage();
            m_cpuMeter->redraw();
        }
    }

private:
    finalcut::FProgressbar *m_cpuMeter;
    finalcut::FLabel *m_cpuInfo;
    int m_cpuUsageTimer;

    double m_totalCpuLastTick = -1;
    double m_activeCpuLastTick = -1;
};

int main(int argc, char *argv[])
{
    finalcut::FApplication app(argc, argv);

    MainDialog mainDialog(&app);
    mainDialog.show();
    finalcut::FWidget::setMainWidget(&mainDialog);

    CpuDialog cpuDialog(&mainDialog);
    cpuDialog.show();

    return app.exec();
}
