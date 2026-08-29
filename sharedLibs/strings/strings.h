#ifndef STRINGS_H
#define STRINGS_H

typedef struct {
    char **dados;
    char *buffer;
} fraseSep;

fraseSep separarStrings(char *caractere, char *frase, int range);

#endif