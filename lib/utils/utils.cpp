#include <Arduino.h>
#include "utils.h"

void splitAtFirst(const String &string, const char delimeter, String *arr)
{
    String firstString = "";

    for (int i = 0; i < string.length(); i++)
    {
        char c = string[i];

        if (c == delimeter)
        {
            arr[0] = firstString;
            arr[1] = string.substring(i + 1);
        }
        else
        {
            firstString += c;
        }
    }

    arr[0] = firstString;
    arr[1] = "";
}
