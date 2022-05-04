Assignment: Project 3 - ***LOCAL OPTION***
Author: Brett Bax
Class: CS 4280
Teacher: Mark Hauschild


Project will be invoked as 'statSem'. ***LOCAL OPTION***

To run:

- Run 'make' 
- One invocation method:
	
	through pre-existing file (program adds extension .sp2022): 

		>./statSem [file]



Things of note:

- 	This project exposed flaws with my parser, particularly relating to loop1 and loop2. Many fixes were made. 

-	In testFile4, I can't figure out why, but my program is skipping straight over a singular semicolon
	on line 12 after 'assign x = y'. I combed through every single line, and I found that it jumps straight 
	from 'assign' to 'yell' when calling the scanner for a reason I'm unable to figure out. It treats the 
	semicolon as white space, so I just added a second semicolon after it and it works just fine. Weird. 

-	Functionality for both the testScanner and makeTree files were left in the project, just adjust the 
	Makefile and call them as you please (code left in Makefile but commented out).


