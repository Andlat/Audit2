#include "Util.h"

#include <iostream>
using namespace std;
char util::toUpperCase(char c) {
	if (c >= 97)//a = 97, A=65 en ASCII
		c -= 32;

	return c;
}