
                  aStub MacOS X Read Me File

--------------------------------------------------------------	
This is a simple stub program that demonstrates using the aIO
and aStem libraries to communicate with the BrainStem modules.

This code was developed using Project Builder and is a command
line program.

This simple program just creates a simple greeting UI and 
wiggles servo 0 on the BrainStem module back and forth each
second.

Here are the steps required to get this up and running on 
your  Mac.

1.  Install the libraries.  In a terminal shell (under the 
Utilities folder of your Applications folder), change 
directories to the garcia distribution and move down to 
the aBinary folder.  In this folder is an install script
that will copy the shared libraries (frameworks) into the 
usable location (your Library folder).  To do this, type:

./install

2.  Now, build the application.  Open the project file in
Project Builder and build the application.  The result will
be a new application file in the aBinary folder named aStub.

3.  Next, plug a standard RC servo into the servo 0 port on 
the BrainStem.

4.  Run the file from the aBinary directory by using the 
terminal shell in that folder.  Type:

./aStub

If all is well, you should get a simple display with a 
greeting, the servo should move, and you should also see
a display of IO readings.

--------------------------------------------------------------	

Where to go from here

This is obviously a very simple example but it shows the 
basics of setting up the BrainStem interface libraries and 
calling them.  The BrainStem reference contains more 
information and API documentation that provide the 
information needed to do much more sophisticated coding on
your Mac to manipulate the physical world via a
BrainStem Module.

This example demonstrates simple slave-mode operation where
the Mac is pretty much delagating all operations 
to the BrainStem module.  The BrainStem offers much more
capability including the ability to run programs on the 
Stem itself using the TEA (Tiny Embedded Applications) 
language.  The BrainStem topic area on the Acroname website
goes into more detail on this and other modes of operation at:

    http://www.acroname.com/brainstem/brainstem.html

------------- Thanks for using the BrainStem! -------------
