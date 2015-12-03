#Stack Visualizer
A program that allows you to see how the variables in your program are stored in the stack.

###Usage
* Compile with a C compiler and make an executable.  
* Run it like any other C program.  
 
It will ask you to specify the number of variables you are going to input. When you enter your variables, the name must only be one character long.  

Input should look exactly like this(_don't end lines in a semicolon as you would when writing the program_):
 ```
int a
char b
long c
 ```

###Example output
```
Number of variables to initialize : 12
int a
char b
short c
int d
float e
double f
float g
char h
char i
char j
short k
char l
	 _______________________________
	|        |             |        |
	| OFFSET |    STACK    | MEMORY |
	|________|_____________|________|
	|                               |
	|	 	 ---------------   LOW  |
	|  -36	| X | l |   k   |   ^   |
	|  -32	| X | j | i | h |   ^   |
	|  -28	|       g       |   ^   |
	|  -24	|       f       |   ^   |
	|  -20	|       f       |   ^   |
	|  -16	|       e       |   ^   |
	|  -12	|       d       |   ^   |
	|  -8	|   c   | X | b |   ^   |
	|  -4	|       a       |   ^   |
	|  %fp -> --------------   HIGH |
	|_______________________________|
```
###Additional info
* It can accept a __char__, __short__, __int__, __long__, __float__, and __double__.  Making any 
of these unsigned or constant won't change the output. 

* An '__X__' represents padding, so don't name any variables '__X__' because then you won't know if you are looking at your variable or padding.  

* At this point, it can't deal with arrays.
 
* It is set to hold 50 stack frames, but if you want more you can change __STACKSIZE__
