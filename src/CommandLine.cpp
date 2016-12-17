// CommandLine.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "CommandLine.h"

using namespace std;

namespace portable
{

void CommandLine::Set(int argc,char** argv)
{	this->argc = argc;
	this->argv = argv;
	// skip arg[0] = prog_name
    for(int i=1;i<argc;i++)
	{	Append(argv[i]);
    }
}

void CommandLine::Append(const char* keyval)
{   const char* eq = strchr(keyval,'=');
	if(eq)
	{	std::string key(keyval,eq-keyval);
		data[std::move(key)]=eq+1;
        return;
	}
    data[keyval] = "true";
}

std::string CommandLine::toString()
{	std::string s;
	for(int i=0;i<argc;i++)
	{	if(argv[i])
		{	s+=argv[i];
			s+=" ";
	}	}
	s.pop_back();
	return s;
}

CommandLine::CommandLine(const std::vector<std::string>& argvVector)
{
    vector<char*> argv;
    argv.resize(argvVector.size(), NULL);

    vector<string>::const_iterator i;
    int n = 0;
    for (i = argvVector.begin(); i != argvVector.end(); ++i, ++n)
    {
        argv[n] = const_cast<char*>((*i).c_str());
    }

    Set(argvVector.size(), (&(argv[0])));
}

const char* CommandLine::operand(int n) const
{
    map<string, string>::const_iterator i;
    int counter = 0;
    for (i = data.begin(); i != data.end(); ++i)
    {
        if (i->second == "true")
        {
            if (counter == n)
            {
                return i->first.c_str();
            }
            ++counter;
        }
    }

    return NULL;
}

bool CommandLine::empty() const
{
    return data.size() == 0;
}

std::map<std::string, std::string>::size_type CommandLine::size()
{
    return data.size();
}

}//portable
