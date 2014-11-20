#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk/gdk.h>
#include "division.h"
#include "rs_ocr.h"

int RS_PixelIsBlack(OCR *o, int x, int y)
{
  guchar *p = get_pixel(o->pixbuf, x, y);
  return (p[0]==0 && p[1]==0 && p[2]==0) ;
}

int RS_CharacterRowHasBlackPixels(OCR *o, int Y1, int Y2, int X)
{
    // Return number of black pixels found between Y1 and Y2 column x
    int numPixels = 0;

    for (int y=Y1;y<Y2;y++)
    {
        numPixels+= RS_PixelIsBlack(o, X, y);
    }
    return numPixels;
}

int RS_HasBlackPixels(OCR *o, int y)
{
    // Return number of black pixels found
    int numPixels = 0;
    int width  = gdk_pixbuf_get_width(o->pixbuf);

    for (int x=0;x<width;x++)
    {
        numPixels+= RS_PixelIsBlack(o, x, y);
    }
    return numPixels;
}

void RS_ProcessCharacter(OCR *o,int x1, int y1, int x2, int y2)
{
    // Process the Character
    // Scale to standard Size ?
    // Have to create a standard size array

    // Draw Character with Xes and spaces
    for(int y=y1;y<=y2;y++)
    {
        for(int x=x1;x<=x2;x++)
        {
            if(RS_PixelIsBlack(o,x,y)) printf("X"); else printf(".");
        }
        printf("\n");
    }

    // Scale to 8*8 array
    // Need to find a way not to lose to many pixels
    // This way is risky as a single vertical column may be lost by the reduction
    char arr[8][8];

    for (int y = 0;y<8;y++)
    {
        for(int x = 0;x<8;x++)
        {
            // Reduce/Scale scanned character to 8*8
            int x3 = (x2-x1)*x/8;
            int y3 = (y2-y1)*y/8;

            // DO not forget to offset (x1+x3) and (y1+y3)
            if(RS_PixelIsBlack(o,x1+x3,y1+y3)) arr[x][y]='X'; else arr[x][y]=' ';
        }
    }
    // Show the reduced letter
    for (int y = 0;y<8;y++)
    {
        for(int x = 0;x<8;x++) printf("%c",arr[x][y]);
        printf("\n");
    }

    // Now the other way around: Scan full character, and fill 8*8
    // This gives better results: The reduced letter resembles more the original
    for (int y = 0;y<8;y++) for(int x = 0;x<8;x++) arr[x][y]=' ';
    for (int y = y1;y<y2;y++)
    {
        for(int x = x1;x<x2;x++)
        {
            // Transform 8*8 coordinate
            int x3 = 8*(x-x1)/(x2-x1);
            int y3 = 8*(y-y1)/(y2-y1);
            if(RS_PixelIsBlack(o,x,y)) arr[x3][y3]='X';
        }
    }
    // Show the reduced letter
    for (int y = 0;y<8;y++)
    {
        for(int x = 0;x<8;x++) printf("%c",arr[x][y]);
        printf("\n");
    }

    // Now really process the letter: Return a string
    /*int LetterArray[8][8];        // Store 8*8
    for (int y = 0;y<8;y++)
    {
        for(int x = 0;x<8;x++) LetterArray[x][y] = arr[x][y];
    }*/
}

void RS_ProcessLine(OCR *o,int Start, int End)
{
    // Process the line here
    // Scan for Characters
    int CharacterStart = 0;
    int CharacterEnd = 0;
    //int LastCharacterEnd=0;     // Use to count blank columns since last letter, if many there is a space
    int prevCol = 0;
    int curCol = 0;
    int numCharacters = 0;

    for (int x=0; x<gdk_pixbuf_get_width(o->pixbuf); x++)
    {
        prevCol = curCol;
        curCol =  RS_CharacterRowHasBlackPixels(o,Start,End,x);

        if(curCol>0 && prevCol==0)
        {
            CharacterStart = x;
            //printf("Character Start at %d\n",CharacterStart);
        }

        if(curCol==0 && prevCol>0)
        {
            CharacterEnd = x;
            numCharacters++;
            //printf("Character End at %d\n",CharacterEnd);
            //printf("Character from %d to %d\n",CharacterStart,CharacterEnd);

            // If there is much blank space, process it as a space character

            RS_ProcessCharacter(o,CharacterStart,Start,CharacterEnd,End);
        }
    }
    printf("Total Characters on this line is %d\n",numCharacters);


}


void RS_Test(OCR *o)
{
    int prevLine = 0;
    int curLine = 0;
    int TextLineStart = 0;
    int TextLineEnd = 0;

    for (int y=0; y<gdk_pixbuf_get_height(o->pixbuf); y++)
    {
        prevLine = curLine;
        curLine = RS_HasBlackPixels(o,y);

        // Text Starts if currentline has pixels and previous has not
        if(curLine>0 && prevLine==0)
        {
            //printf("Start of Textline\n");
            TextLineStart = y;
        }

        // Text Ends if currentline has no pixels and previous has
        if(curLine==0 && prevLine>0)
        {
            //printf("End of TextLine\n");
            TextLineEnd = y-1;

            // Should process the line here
            printf("Process Line from %d to %d\n",TextLineStart,TextLineEnd);
            RS_ProcessLine(o,TextLineStart,TextLineEnd);
        }

        //printf("Black Pixels on line %d = %d\n",y,curLine);
    }
}
