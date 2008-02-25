#include <stdio.h>
#include <conio.h>


void main(void)
{
    int ch;

    printf("Hit escape to quit\n");

    while ((ch = getch()) != 27)
    	printf("key %d 0x%02x %c\n", ch, ch, ch);
}
