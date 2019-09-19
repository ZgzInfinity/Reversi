/*********************************************************************************************
* Fichero:	reversi_def.c
* Autores:  737215 Rubén Rodríguez Esteban, 738455 Adrián Samatán Alastuey
* Descrip:	Fichero de definiciones comunes de constantes y funciones utiles para reversi
*********************************************************************************************/

#include "reversi_def.h"
#include <string.h>

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    do
    {                          /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);   /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void ordenar(ULONG valores[], int n_elementos)
{
    int i, j;
    ULONG tmp;

    for (i = 0; i < n_elementos - 1; ++i)
    {
        for (j = 0; j < n_elementos - i - 1; ++j)
        {
            if (valores[j] > valores[j + 1])
            {
                tmp = valores[j];
                valores[j] = valores[j + 1];
                valores[j + 1] = tmp;
            }
        }
    }
}
