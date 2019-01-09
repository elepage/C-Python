# C-Python
Call of Python code from C/C++ Binary

The simple idea is to call python code from C/C++ code with the possibility to have call backs from python back to C/C++
The second target was to see how in python one could directly read / write data from the memeory blocks of the heap allocated in C
you will find here a simple example of implementation with python 2.7x32
directions
1) build the executable file "Test_python" from the main.cpp
2) then call Test_python {pythoncodefilename} {functionname} {argument1} {argument2}
____________________________________________________________________
example1 "Test_python multiply multiply 3 2" 
// this will call the multiply function from multiply.py
// this is very simple with no call back and no modification of data allocated by the C layer

Will compute 3 times 2
Pointers:50075968,18348536,18348516
sizeof:4,20
************************************************************
Result of call: 6
************************************************************
_____________________________________________________________________
example2 "Test_python multiply2 multiply 3 2"
// this will call the multiply function from multiply2.py
// this will be a bit more subtle use call backs and modify data allocated in by the C layer

Number of arguments 5
Test2 28
Test3 3
Original values: (1, 'ab', 2.7)
Format string  : I 2s f
Uses           : 12 bytes
Packed Value   : 0100000061620000cdcc2c40
Will compute 3 times 2
piInput 8190224 piOutput 8190204
multiplyWithUserData 3
0 Input:( 8190224 , 1 ),Output:( 8190204 , 3 )
1 Input:( 8190228 , 2 ),Output:( 8190208 , 6 )
2 Input:( 8190232 , 3 ),Output:( 8190212 , 9 )
3 Input:( 8190236 , 4 ),Output:( 8190216 , 12 )
4 Input:( 8190240 , 5 ),Output:( 8190220 , 15 )
Will compute 3 times 2
Pointers:45422912,8190224,8190204
sizeof:4,20
This is C Hello World;2,6
************************************************************
Result of call: 6
************************************************************
(This is C 45422912|1)(This is C 45422912|0)(This is C 45422912|1)(This is C 45422912|0)This is C 45422912;2
Result of call: 6
[0]:1|3
[1]:2|6
[2]:3|9
[3]:4|12
[4]:5|15
************************************************************
The last printout of the content of the arrays is done from the C Layer and has been amended by Python with no use of a callback 
