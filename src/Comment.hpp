#ifndef _COMMENT_HPP
#define _COMMENT_HPP

#include <stdio.h>

int skipcomment1(FILE *fp); // skips comments of the form # to eol
int skipcomment2(FILE *fp); // skips comments of the form -- to eol
int skipcomment3(FILE *fp); // skips comments of the form << to eol
int skipcomment4(FILE *fp); // skips comments of the form >> to eol
int skipcomment(FILE *fp); // 1, 2, 3, and 4 combined
int eat_comment(FILE *fp, const char *s); // skips commnets of the form s to eol
int skipline(FILE *fp);
int get_comment_char(void);
void long_to_string (unsigned long input, char *str, char numdigits);
void long_to_string_lz (unsigned long input, char *str, char numdigits);

#endif
