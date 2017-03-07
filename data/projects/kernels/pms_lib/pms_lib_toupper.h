#ifndef PMS_LIB_TOUPPER
#define PMS_LIB_TOUPPER

char
pms_toupper(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        character &= ~(1 << 5);
    }
    return character;
}

#endif
