#include <iostream>
#include "./imap_terminal.h"

using namespace std;
using namespace imap_terminal;

int main()
{
    try
    {
        CImapTerminal terminal(
            "imap.terminal.test@gmail.com",
            "1234567890q",
            "imap.gmail.com");
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}