#include "CMask.h"

bool CMask::WildMatch(const QString &sWild, const QString &sString, const QString &sLimitChar)
{
    bool bAny = false;
    bool bNextIsOptional = false;
    bool bAutorizedChar = true;

    int i = 0;
    int j = 0;

    // Check all the string char by char
    while (i < sString.length())
    {
        // Check index for array overflow
        if (j < sWild.length())
        {
            // Manage '*' in the wildcard
            if (sWild[j] == '*')
            {
                // Go to next character in the wildcard
                j++;

                // End of the string and wildcard end 
                // with *, only test string validity
                if (j >= sWild.length())
                {
                    // Check end of the string
                    while (!sLimitChar.isEmpty() && i < sString.length())
                    {
                        // If this char is not ok, return false
                        if (sLimitChar.indexOf(sString[i]) < 0)
                            return false;

                        i++;
                    }

                    return true;
                }

                bAny = true;
                bNextIsOptional = false;
            }
            else
            {
                // Optional char in the wildcard
                if (sWild[j] == '^')
                {
                    // Go to next char in the wildcard and indicate 
                    // that the next is optional
                    j++;

                    bNextIsOptional = true;
                }
                else
                {
                    bAutorizedChar =
                        (sLimitChar.isEmpty()) || (sLimitChar.indexOf(sString[i]) >= 0);

                    // IF :
                    if (// Current char match the wildcard
                        sWild[j] == sString[i]
                        // '?' is used and current char is in autorized char list
                        || (sWild[j] == '?' && bAutorizedChar)
                        // Char is optional and it's not in the string
                        // and it's necessary to test if '*' make any 
                        // char browsing
                        || (bNextIsOptional && !(bAny && bAutorizedChar)))
                    {
                        // If current char match wildcard, 
                        // we stop for any char browsing
                        if (sWild[j] == sString[i])
                            bAny = false;

                        // If it's not an optional char who is not present,
                        // go to next
                        if (sWild[j] == sString[i] || sWild[j] == '?')
                            i++;

                        j++;

                        bNextIsOptional = false;
                    }
                    else
                        // If we are in any char browsing ('*') 
                        // and curent char is autorized
                        if (bAny && bAutorizedChar)
                            // Go to next
                            i++;
                        else
                            return false;
                }
            }
        }
        else
            // End of the wildcard but not the 
            // end of the string => 
            // not matching
            return false;
    }

    if (j < sWild.length() && sWild[j] == '^')
    {
        bNextIsOptional = true;
        j++;
    }

    // If the string is shorter than wildcard 
    // we test end of the 
    // wildcard to check matching
    while ((j < sWild.length() && sWild[j] == '*') || bNextIsOptional)
    {
        j++;
        bNextIsOptional = false;

        if (j < sWild.length() && sWild[j] == '^')
        {
            bNextIsOptional = true;
            j++;
        }
    }

    return j >= sWild.length();
}