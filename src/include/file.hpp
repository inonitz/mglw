#pragma once
#include "base.hpp"



/* 
Recommended to pass size = 0 and out = nullptr to find the size of the buffer.
When you know the size, allocate an appropriate buffer and pass the args to the function.
Then the function will load the File properly. 
*/
bool loadFile(
	const char* path, /* filepath 														   */ 
	size_t* 	size, /* the size of the out array that was pre-allocated.                 */
	char*		out   /* Where to place the file contents (Buffer of Min-Size 'size')      */
);