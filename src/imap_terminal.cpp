#include <iostream>

#include "vmime/vmime.hpp"
#include "vmime/platforms/windows/windowsHandler.hpp"


int main()
{
    std::cout << std::endl;

    // VMime initialization
    vmime::platform::setHandler<vmime::platforms::windows::windowsHandler>();

    try
    {
        vmime::messageBuilder mb;

        // Fill in the basic fields
        mb.setExpeditor(vmime::mailbox("me@somewhere.com"));

        vmime::addressList to;
        to.appendAddress(vmime::create <vmime::mailbox>("you@elsewhere.com"));

        mb.setRecipients(to);

        vmime::addressList bcc;
        bcc.appendAddress(vmime::create <vmime::mailbox>("you-bcc@nowhere.com"));

        mb.setBlindCopyRecipients(bcc);

        mb.setSubject(vmime::text("My first message generated with vmime::messageBuilder"));

        // Message body
        mb.getTextPart()->setText(vmime::create <vmime::stringContentHandler>(
            "I'm writing this short text to test message construction " \
            "using the vmime::messageBuilder component."));

        // Construction
        vmime::ref <vmime::message> msg = mb.construct();

        // Raw text generation
        std::cout << "Generated message:" << std::endl;
        std::cout << "==================" << std::endl;

        vmime::utility::outputStreamAdapter out(std::cout);
        msg->generate(out);
    }
    // VMime exception
    catch (vmime::exception& e)
    {
        std::cout << "vmime::exception: " << e.what() << std::endl;
        throw;
    }
    // Standard exception
    catch (std::exception& e)
    {
        std::cout << "std::exception: " << e.what() << std::endl;
        //throw;
    }

    std::cout << std::endl;
}