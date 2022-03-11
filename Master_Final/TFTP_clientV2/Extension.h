#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ERROR_EXTENSION 1
#define EXTENSION_VALIDE 0

class Extension
{
private:
	const char* extension;
	int TAILLE = 0;
	int nbPoint = 0;
public:

	Extension();
	int verifierExtension(const char* Destination);

};

