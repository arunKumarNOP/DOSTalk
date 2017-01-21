DOSTalk
====

This is a small program that i did as a Network Assignment to achieve both way commuinication between two computer in a LAN. This program run above OSI layer 2 ie above Packet Driver. This is as basic an Ethernet Programming could get.

DOS inface FreeDOS[1] was choosen as Guest OS as it doesn't have network capability so we could install Packet Driver[2] and then code programs above it and learn how things work.

Building
====

Make sure Turbo C is added to the PATH environment variable.

Before building make sure you change the destination MAC address to whom you want to communicate to by editing the file.

To build talk.c run `tcc -mh talk.c` , -mh flag to set the model size to huge otherwise it gives Null Pointer Exceptions while executing the program.

In ova file talk.c file is present in 'C:\PROG' folder

Usage
====

Run talk.exe after building then you can send and receive messages from other computer.
To exit from the program press enter twice without pressing anyother key in between.


Some other infos
====
The ova file has mTCP[4] package installed and added to PATH environment so you can issue DHCP to get an IP and then issue FTPSRV to create a ftp server to transfer files easily.

Few commands like ls=dir, rm=del, vim=edit are aliased so you can have linux like environment.

Author<br>
Arun Kumar Shreevastava

[1] http://www.freedos.org/
[2] http://www.georgpotthast.de/sioux/packet.htm
[3] https://winworldpc.com/product/turbo-c/3x
[4] http://www.brutman.com/mTCP/
