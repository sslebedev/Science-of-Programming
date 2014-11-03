#ifndef SL_TEXT_H
#define SL_TEXT_H

#include <stdio.h>

typedef struct tagLine
{
    int start;
} Line;

void InitText   ( FILE *file );
void DeinitText ();
void GetContent ( char **start, int *length );
void GetLines   ( Line **lines, int *length, int *maxLength );
int  GetLineLegth(int i);

#endif // SL_TEXT_H
