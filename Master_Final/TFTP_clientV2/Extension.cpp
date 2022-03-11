#include "Extension.h"

Extension::Extension()
{

}


int Extension::verifierExtension(const char* Destination)
{
    extension = strrchr(Destination, '.');
    nbPoint = 0;

    if (Destination == NULL)
    {
      return ERROR_EXTENSION;
    }
    
    for (int cpt = 0 ;Destination[cpt] != '\0' ; cpt++ )
    {
        if (Destination[cpt] == '.')
        {
            nbPoint += 1;
        }
    }

    if (nbPoint == 1)
    {
        if ((!strcmp(extension, ".avi") || !strcmp(extension, ".txt")) && strlen(Destination) > 4)
        {
            return EXTENSION_VALIDE;
        }
        else
        {
            return ERROR_EXTENSION;
        }
    }
    else
    {
        return ERROR_EXTENSION;
    } 
}