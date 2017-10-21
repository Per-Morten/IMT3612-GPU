#ifndef PMS_LIB_TOLOWER
#define PMS_LIB_TOLOWER

char
pms_tolower(char character)
{
    if (character >= 'A' && character <= 'Z')
    {
        character |= (1 << 5);
    }
    return character;
}

#endif
