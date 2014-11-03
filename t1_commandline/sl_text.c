#include <stdlib.h>
#include <assert.h>

#include "sl_text.h"

// Text object data
static struct Text {
    char* buf;       // character buffer
    int bufLength;   // character count

    Line* lines;     // array of text lines (from buffer) - from file
    int linesLength; // array length
    int maxLength;   // max character line lingth (chars)
} text;

//************************************
// Returns:   void
// Parameter: FILE * file
// Throws:    none
//************************************
void InitText( FILE *file )
{
    int fileSize;
    int countLines;
    int lineLength;
    int lineStart;
    int i;


    assert(file);

    // Read file
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    text.buf = (char*)calloc(sizeof(char), fileSize);
    text.bufLength = fileSize;

    fread(text.buf, sizeof(char), fileSize, file);

    // Recount lines
    i          = 0;
    countLines = 0;

    while (i < text.bufLength) {
        if (text.buf[i] == '\n') {
            ++countLines;
        }
        ++i;
    }
    ++countLines;

    // fill text lines
    text.lines       = (Line*)calloc(sizeof(Line), fileSize);
    text.linesLength = countLines;
    text.maxLength = 0;

    i          = 0;
    lineLength = 0;
    lineStart  = -1;
    countLines = 0;

    while (i < text.bufLength) {
        if (lineStart < 0) {
            lineStart = i;
            lineLength = 0;
        }

        if (text.buf[i] == '\n') {
            text.lines[countLines].start  = lineStart;
            lineStart = -1;
            ++countLines;
            if (text.maxLength < lineLength) {
                text.maxLength = lineLength;
            }
        } else {
            ++lineLength;
        }
        ++i;
    }
    if (lineStart < 0) {
        lineStart = i;
        lineLength = 0;
    }
    text.lines[countLines].start  = lineStart;
    if (text.maxLength < lineLength) {
        text.maxLength = lineLength;
    }
    assert(countLines + 1 == text.linesLength);
}


//************************************
// Returns:   void
// Throws:    none
//************************************
void DeinitText()
{
    if (text.buf) {
        free(text.buf);
        free(text.lines);
    }
    text.buf = NULL;
    text.lines = NULL;
    text.bufLength = 0;
    text.linesLength = 0;
}

//************************************
// Returns:   void
// Parameter: char ** start
// Parameter: int * length
// Throws:    none
//************************************
void GetContent( char **start, int *length )
{
    *start  = text.buf;
    *length = text.bufLength;
}

//************************************
// Returns:   void
// Parameter: Line ** lines
// Parameter: int * length
// Throws:    none
//************************************
void GetLines( Line **lines, int *length, int *maxLength )
{
    *lines    = text.lines;
    *length   = text.linesLength;
    *maxLength = text.maxLength;
}

//************************************
// Returns:   int
// Parameter: int i
// Throws:    none
//************************************
int GetLineLegth( int i )
{
    if (text.lines == NULL) {
        return 0;
    }

    if (i < text.linesLength - 1) {
        return text.lines[i + 1].start - text.lines[i].start;
    }
    return text.bufLength - text.lines[i].start;
}
