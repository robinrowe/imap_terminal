#include <iostream>
#include <tree_util.hh>
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
        /*tree<string> tr;
        tree<string>::iterator top, one, two, loc, banana;

        top = tr.begin();
        one = tr.insert(top, "one");
        two = tr.append_child(one, "two");

        kptree::print_tree_bracketed(tr);*/
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}