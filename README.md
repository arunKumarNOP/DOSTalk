DOSTalk Readme

1. Installtion and setup of Guest OS

	1.a - Installation
	VirtualBox[1] is used to run the virtual OS, FreeDOS[2] is used as a Guest Operating, after installtion download

	1.b - Setup
		1.b.i - Packet Driver
		After installation of FreeDOS download and install NICSCAN[3] to determine the NIC type then corresponding to that type download the Packet Driver, in my case it was AMD type so both PCNTPK and PCNETODI worked fine, i ended up using PCNTPK.

		Commands used:
			#pcntpk.com int=0x60

		Here we are setting Software interrupt as 0x60 and after executing the command it shows the Hardware interrupt number which is 0x0A or 10.

		1.b.ii - Testing of packet Driver
		To test if the packet driver is working correctly i used mTCP[4] tools and then ran DHCP.exe to get a IP which made sure that packet driver is working correctly.

		1.b.iii - Turbo C compiler[5] 
		Download and then setup PATH environment variable in autoexec.bat file accordingly.

2. Building DOSTalk program

Make sure Turbo C is added to the PATH environment variable.

Before building make sure you change the destination MAC address to whom you want to communicate to by editing the file.

To build talk.c run `tcc -ml talk.c` , -ml flag to set the model size to large otherwise it gives Null Pointer Exceptions while executing the program.

3. Usage

Run talk.exe after building then you can send and receive messages from other computer.
To exit from the program press enter twice without pressing anyother key in between.


Authors
Arun Kumar Shreevastava

[1] = https://www.virtualbox.org/
[2] = http://www.freedos.org/
[3] = http://www.georgpotthast.de/sioux/packet.htm
[4] = http://www.brutman.com/mTCP/
[5] = https://winworldpc.com/product/turbo-c/3x
