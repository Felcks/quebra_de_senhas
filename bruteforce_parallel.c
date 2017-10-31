#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*static const char alphabet[] = 
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"0123456789";*/

static const char alphabet[] = "abc";

unsigned long generated_passwords = 0;
static const int alphabet_size = sizeof(alphabet) - 1;

void bruteImpl(char* str, int index, int maxDepth)
{
    #pragma omp parallel for
    for (int i = 0; i < alphabet_size; ++i)
    {
        str[index] = alphabet[i];

        if (index == maxDepth - 1) {
           // printf("%s %i\n", str, omp_get_thread_num());
        }
        else bruteImpl(str, index + 1, maxDepth);
    }
}

void bruteSequential(int maxLen)
{
    char* buf = malloc(maxLen + 1);

    for (int i = 1; i <= maxLen; ++i)
    {
        memset(buf, 0, maxLen + 1);
        bruteImpl(buf, 0, i);
    }

    free(buf);
}

int main(void)
{
    printf("%i\n", omp_get_num_threads());
    bruteSequential(3);
    return 0;
}