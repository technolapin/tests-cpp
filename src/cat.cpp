// du c mais osef


#include <stdio.h>

int
main(int argc, char ** argv)
{
    char c;
    while ((c = getchar())!=EOF)
    {
        fputc(c, stdout);
        fflush(stdout);
    }
    return 0;
}
