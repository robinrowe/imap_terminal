# imap_terminal Design Spec

By Robin Rowe 2016/10/2

## Summary

A simple command-line program to test remote IMAP server functionality. Use libetpan or another suitable open source IMAP library. Use strtok() or strstream or some other simple command parser in a for(;;) loop until the user enters "exit".

Process commands against any remote IMAP server and display to users to make it seem as though they're at the command line of a local Linux file system. 

## Example Session 

For an account robin@example.com with a password of pass...

	login> example.com robin pass
	example.com/> 
	example.com/> limit 20
	example.com/> ls -s "test message"
	example.com/> ls 3
	example.com/> head 3
	example.com/> cd sent
	example.com/sent/> cd ..
	example.com/> mv -s "test message" "test msg 1"
	example.com/> mv 3 junk
	example.com/> rm 2
	example.com/> rm -s "test msg 1"
	example.com/> head 1
	example.com/> send robin@example.com "Test msg 2" "Hello, this is msg body"
	example.com/> exit

The limit command limits the # of lines returned from any command. Default is 20. 

A message # may be the actual server Message-Id or the message # as returned by the last ls command. That is, 1 through 20, if there are 20 messages on our IMAP server. Keep a vector of strings to track the Message-Ids of the last ls command to use with later commands.

The ls command accepts flags for -from, -to, -subject or their abbreviations, -f, -t, -s. With no flag it's expecting a msg#. Output of ls has format of #-subject-from:

	1. "Test msg 1" robin@example.com 
	2. "Test msg 2" robin@example.com
	3. "Hello World" root@example.com

The mv command can move a message to a folder or rename its fields.

