#include <iostream>
#include "./imap_terminal.h"
#include "./utils.h"
#include "./CommandLine.h"

using namespace std;
using namespace imap_terminal;

namespace
{
    class CommandLineException {};
}

int main(int argc, char** argv)
{
    try
    {
        portable::CommandLine cmdLine(argc, argv);

        string sHost, sPort, sUser, sPass;
        bool fSsl = false;

        sHost = (cmdLine.Get("-host") == NULL) ? "" : cmdLine.Get("-host");
        if (sHost.length() <= 0)
        {
            sHost = (cmdLine.Get("-h") != NULL) ? cmdLine.Get("-h") : "";
        }

        sPort = (cmdLine.Get("-port") == NULL) ? "" : cmdLine.Get("-port");
        if (sPort.length() <= 0)
        {
            sPort = (cmdLine.Get("-p") != NULL) ? cmdLine.Get("-p") : "";
        }

        sUser = (cmdLine.Get("-user") == NULL) ? "" : cmdLine.Get("-user");
        if (sUser.length() <= 0)
        {
            sUser = (cmdLine.Get("-u") != NULL) ? cmdLine.Get("-u") : "";
        }

        sPass = (cmdLine.Get("-pass") == NULL) ? "" : cmdLine.Get("-pass");
        if (sPass.length() <= 0)
        {
            sPass = (cmdLine.Get("-P") != NULL) ? cmdLine.Get("-P") : "";
        }

        fSsl = cmdLine.IsKey("-s") || cmdLine.IsKey("-ssl");
        
        if (sHost.length() == 0 || sUser.length() == 0)
        {
            throw CommandLineException();
        }
        
        if (sPort.length() == 0)
        {
            sPort = "993";
        }

        CImapTerminal terminal(
            sUser,
            sPass,
            sHost,
            sPort,
            fSsl);        
    }
    catch (const CommandLineException&)
    {
        cout << "Usage:" << endl;
        cout <<
            "imap_terminal -h|-host=<host addr> [-p|-port=<tcp port>] -u|-user=<user name> [-P|-pass=<password>] [-s|-ssl]" <<
            endl;
        return 1;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}