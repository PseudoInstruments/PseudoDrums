#pragma once

#include "ofMain.h"

typedef unsigned char uint8;
typedef char int8;

//Exceptions
#define de_exception(message) { cout << message << endl; __debugbreak(); ofSystemAlertDialog(message); _Exit(1); }
#define de_assert(cond, message) {if (!(cond)) de_exception(message);}

//OutputDebugStringA( message "\n" ); 
//		*(volatile int*)nullptr = 1;        