#include "ProcessList.h"

#include <filesystem>

#include <pwd.h>

ProcessList::ProcessList(finalcut::FWidget *parent)
    : finalcut::FDialog{parent},
      m_processList{new finalcut::FListView{this}}
{
    setText("Processes");
    setGeometry({35, 2}, {75, 15});

    m_processList->setGeometry({0, 0}, {75, 15});
    m_processList->addColumn("PID", 6);
    m_processList->addColumn("User", 15);
    m_processList->addColumn("Command");

    parseProcessList();
}

void ProcessList::parseProcessList()
{
    struct Process
    {
        std::string pid;
        std::string user;
        std::string command;
    };
    std::vector<Process> processes;

    const std::filesystem::path procDir{"/proc"};
    for (const auto &entry : std::filesystem::directory_iterator{procDir})
    {
        if (entry.is_directory())
        {
            const std::string procName = entry.path().filename().string();
            if (std::all_of(procName.begin(), procName.end(), ::isdigit))
            {
                std::ifstream status{entry.path().string() + "/status"};
                uid_t uid = 0;

                std::string line;
                while (std::getline(status, line))
                {
                    std::string row;
                    status >> row;
                    if (row == "Uid:")
                    {
                        status >> uid;
                        break;
                    }
                }
                auto user = getpwuid(uid);

                std::ifstream cmdline{entry.path().string() + "/cmdline"};
                std::string command;
                std::getline(cmdline, command);
                std::replace(command.begin(), command.end(), '\0', ' ');

                processes.push_back(Process{
                    .pid = procName, .user = user ? user->pw_name : std::to_string(uid), .command = command});
            }
        }
    }

    std::sort(processes.begin(), processes.end(), [](const auto &l, const auto &r) {
        if (r.command.size() == 0 && l.command.size() > 0)
            return true;
        else if (l.command.size() == 0 && r.command.size() > 0)
            return false;
        else
            return std::stoi(l.pid) < std::stoi(r.pid);
    });
    for (const auto &process : processes)
        m_processList->insert({process.pid, process.user, process.command});
}

void ProcessList::onClose(finalcut::FCloseEvent *e)
{
    hide();
    e->ignore();
}
