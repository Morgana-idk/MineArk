#include "strings.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

fraseSep separarStrings(char *caractere, char *frase, int range) {
    fraseSep fraseSeparada = {0};

    if (frase == NULL || caractere == NULL) return fraseSeparada;

    fraseSeparada.dados = calloc(range + 1, sizeof(char*));
    
    if (fraseSeparada.dados == NULL) return fraseSeparada;

    fraseSeparada.buffer = strdup(frase);
    if (fraseSeparada.buffer == NULL) {
        free(fraseSeparada.dados);
        fraseSeparada.dados = NULL;
        return fraseSeparada;
    }

    fraseSeparada.dados[0] = strtok(fraseSeparada.buffer, caractere);

    for (int i = 1; i <= range; i++) {
        fraseSeparada.dados[i] = strtok(NULL, caractere);
        if (fraseSeparada.dados[i] == NULL) {
            break;
        }
    }

    return fraseSeparada;
}