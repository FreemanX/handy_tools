#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define LINE_LEN    (80)


void    stdin_dump(int);
void    file_dump(char *);
void    hex_dump(unsigned char [], int, long);
char    *get_cmdname(char *);
char    *cmdname;


int     main(argc, argv)
int     argc;
char    *argv[];
{
	int i;

	/* get cmdname */
	cmdname = get_cmdname(argv[0]);

	/* main loop */
	if (argc == 1)
	    stdin_dump(fileno(stdin));
	else
	{
	    for (i = 1; i < argc; i++)
	    	file_dump(argv[i]);
	} /* else */

	/* exit now */
	return (0);
} /* main */


void    stdin_dump(fd)
int     fd;
{
	unsigned char   buf[16];
	int             nbytes;
	long            pos;

	for (pos = 0; (nbytes = read(fd, buf, sizeof (buf))) > 0; pos += nbytes)
	    hex_dump(buf, nbytes, pos);
} /* stdin_dump */


void    file_dump(fname)
char    *fname;
{
	unsigned char   buf[16];
	int             nbytes;
	long            pos;
	int             fd;

	/* open file */
	if ((fd = open(fname, O_RDONLY)) != -1)
	{
	    for (pos = 0; (nbytes = read(fd, buf, sizeof (buf))) > 0; pos += nbytes)
	    	hex_dump(buf, nbytes, pos);
	}
	else
	{
	    fprintf(stdout, "%s: unable to open %s [%s]!\n", cmdname,
	    	fname, strerror(errno));
	} /* else */
} /* file_dump */


void            hex_dump(buf, bcnt, pos)
unsigned char   buf[];
long            pos;
{
	char    line[LINE_LEN+1], ascii_buf[17], *p;
	int     j;

	/* main loop of hex_dump */
	ascii_buf[sizeof (ascii_buf) - 1] = '\0';

	/* init p */
	sprintf(line, "0x%08lx: ", pos);
	p = line + strlen(line);

	/* loop through the next batch */
	for (j = 0; j < 16; j++)
	{
	    if (j < bcnt)
	    {
	    	sprintf(p,"%02x%c", (unsigned char) buf[j], j==7 ? '-' : ' ');
	    	ascii_buf[j] = (buf[j]<0x80 && isprint(buf[j])) ? buf[j] : '.';
	    } /* if */
	    else
	    {
	    	sprintf(p, "   ");
	    	ascii_buf[j] = ' ';
	    } /* else */
	    p += 3;
	} /* for */
	printf("%s\t%s\n", line, ascii_buf);

	return;
} /* hex_dump */


char    *get_cmdname(argv0)
char    *argv0;
{
	/* get cmdname */
	cmdname = argv0 + strlen(argv0) - 1;
	for (; cmdname > argv0 && *(cmdname-1) != '/'; cmdname--)
	    ;
	return (cmdname);
} /* get_cmdname */

