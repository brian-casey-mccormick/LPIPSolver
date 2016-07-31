#include <ctype.h>
#include <string.h>
#include "Comment.hpp"

static char cc='#';

int get_comment_char(void)
{
	return cc;
}

int skipline (FILE *fp)
{
	int ch;

	while ((ch = getc(fp) ) != EOF && ch != '\n');

	return 0;
}

int eat_comment(FILE *fp, const char *s)
{
	fpos_t fpos;

	int ch, ns, i;
	int done = 0;
	int rollback = 0;

	ns = strlen(s);

	if (ns < 1) return 0;

	while (!done) 
	{
		rollback=0;

		// save the rollback position
		fgetpos (fp, &fpos);

		// get a character
		ch = getc(fp);

		// if it's the end of the file, we're done
		if (ch == EOF) 
		{
			break;

		// if it's whitespace, eat it and continue
		} 
		else if (isspace(ch)) 
		{
			continue;

		// if it's a possible comment
		} 
		else if (ch == s[0]) 
		{
			// check the beginning of the possible comment
			i=1;

			while (i < ns  && (ch = getc(fp)) != EOF  &&  ch == s[i])  i++;

			// eat the remainder of the comment
			if (i == ns)
			{
				while (( ch = getc(fp) ) != EOF   &&  ch != '\n');
			}
			// it wasn't a comment, rollback
			else 
			{
				rollback = 1;
			}
		}
		else
		{
			rollback = 1;
		}

		if (rollback) 
		{
			fsetpos(fp, &fpos);
			done = 1;
		}
	}

	return 0;
}

int skipcomment1(FILE *fp)
{
	return eat_comment(fp,"#");
}

int skipcomment2(FILE *fp)
{
	return eat_comment(fp,"--");
}

int skipcomment3(FILE *fp)
{
	return eat_comment(fp,"<<");
}

int skipcomment4(FILE *fp)
{
	return eat_comment(fp,">>");
}

int skipcomment(FILE *fp)
{
	skipcomment1(fp);
	skipcomment2(fp);
	skipcomment3(fp);
	skipcomment4(fp);

	return 0;
}

// Description : Convert a "long" to a null-terminated string (base = decimal)
#define TRUE 1
#define FALSE 0

void long_to_string(unsigned long input, char *str, char numdigits)
{
	char digit, blank = TRUE;
	
	long_to_string_lz(input, str, numdigits);
	
	for (digit=0; digit < numdigits-1; digit++)
	{
		if (str[digit] == '0')
		{
			if (blank == TRUE) str[digit] = ' ';
		}
		else
		{
			blank = FALSE;
		}
	}

	return;
}

// Convert a "long" to a null-terminated string, with leading zeros (base = decimal)
void long_to_string_lz (unsigned long input, char *str, char numdigits)
{
	char digit;

	for (digit=numdigits; digit > 0; digit--) 
	{
		str[digit-1] = (char)((input % 10) + '0');
		input = input / 10;
	}

	// null-terminate the string
    str[numdigits] = 0;

	return;
}










