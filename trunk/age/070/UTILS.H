// utility functions & structures
// November, 1997 jerry@mail.csh.rit.edu


struct lookup_list{
    char ch;
    int  translation;
};


int lookup_translation(char ch_input, struct lookup_list *lu, char end_ch);
// returns the translation of ch, or end_ch if not found
// last entry in the lookup_list MUST be {end_ch, ?}
// (see manips.c for an example)
