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

class MemoryDialog : public finalcut::FDialog
{
public:
    MemoryDialog(finalcut::FWidget *parent)
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
            setGeometry({4, 18}, {60, 9});
            auto swapLabel = new finalcut::FLabel{"Swap:", this};
            swapLabel->setGeometry({1, 5}, {5, 1});
            m_swapMeter = new finalcut::FProgressbar{this};
            m_swapMeter->setGeometry({7, 5}, {30, 1});
            m_swapLabel = new finalcut::FLabel{this};
        }
        else
            setGeometry({4, 18}, {60, 6});
        parseMemUsage();

        m_memUsageTimer = addTimer(500);
    }

    void parseMemUsage()
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

    void onTimer(finalcut::FTimerEvent *e) override
    {
        if (e->getTimerId() == m_memUsageTimer)
        {
            parseMemUsage();
            m_ramMeter->redraw();
        }
    }

private:
    finalcut::FProgressbar *m_ramMeter = nullptr;
    finalcut::FProgressbar *m_swapMeter = nullptr;
    finalcut::FLabel *m_ramLabel = nullptr;
    finalcut::FLabel *m_swapLabel = nullptr;
    int m_memUsageTimer;
};

int main(int argc, char *argv[])
{
    finalcut::FApplication app(argc, argv);

    MainDialog mainDialog(&app);
    mainDialog.show();
    finalcut::FWidget::setMainWidget(&mainDialog);

    CpuDialog cpuDialog(&mainDialog);
    cpuDialog.show();

    MemoryDialog memoryDialog(&mainDialog);
    memoryDialog.show();

    return app.exec();
}
