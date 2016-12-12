#include <iostream>
#include "./imap_terminal.h"
#include "./utils.h"

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

        /*vector<string> v = CUtils::cmdline("command -src=\"sdf fg\" -dst \"123 456\"", " ");
        for (vector<string>::iterator i = v.begin(); i != v.end(); i++)
        {
            cout << "<" << *i << ">" << endl;
        }*/
        
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}