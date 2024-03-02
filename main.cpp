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

int main(int argc, char *argv[])
{
    finalcut::FApplication app(argc, argv);

    MainDialog mainDialog(&app);
    mainDialog.show();
    finalcut::FWidget::setMainWidget(&mainDialog);

    return app.exec();
}
