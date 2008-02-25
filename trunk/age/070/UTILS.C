// utility functions & structures
// November, 1997 jerry@mail.csh.rit.edu

#include "utils.h"


int lookup_translation(char ch_input, struct lookup_list *lu, char end_ch)
{
    //look in tools.c for an example of this in use with the lookup table
    int index=0;
    
    while (index < 512){
	// the counter is just an extra safeguard just in case there's
	// no {end_ch,end_ch} termination (512 is an arbitrary number)
	if(lu[index].ch == end_ch)
	    return (end_ch);
	if(lu[index].ch == ch_input)
	    return (lu[index].translation);
	index++;
    }
    return (end_ch);
}

