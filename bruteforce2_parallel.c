#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
#include <math.h>

/*const char *alphabet = "abcdefghijklmnopqrstuvwxyz"
		       			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		       			"0123456789"; */

const char *alphabet = "abc";

static void generate(int maxlen);

int main(int argc, char *argv[])
{
    if (argc < 2) {
		fprintf(stderr, "Usage: %s Length\n", argv[0]);
		exit(1);
    }

    generate(atoi(argv[1]));
    return 0;
}

static void generate(int maxlen)
{
    int   alphaLen = strlen(alphabet);
    int   len      = 0;
    char *buffer   = malloc((maxlen + 1) * alphaLen * alphaLen);
    int  *letters  = malloc(maxlen * sizeof(int));

    if (buffer == NULL || letters == NULL) {
		fprintf(stderr, "Not enough memory.\n");
		exit(1);
    }

    omp_set_num_threads(4);
    for (len=1; len <= maxlen; len++) { 
   
		int i;
		int stride = len+1;
		int bufLen = stride * alphaLen * alphaLen;

		if (len == 1) {
		  
		    int j = 0;
		    bufLen = (len + 1) * alphaLen;
		    
		    for (i=0; i<alphaLen; i++) {
				buffer[j++] = alphabet[i];
				buffer[j++] = '\n';
		    }

		    //printf("%i ", omp_get_thread_num());
		    write(STDOUT_FILENO, buffer, bufLen);
		    continue;
		}

		// Initialize buffer to contain all first letters.
		memset(buffer, alphabet[0], bufLen);

		// Now write all the last 2 letters and newlines, which
		// will after this not change during the main algorithm.
		{
		    // Let0 is the 2nd to last letter.  Let1 is the last letter.
		    int let0 = 0;
		    int let1 = 0;
		    for (i=len-2;i<bufLen;i+=stride) {
				buffer[i]   = alphabet[let0];
				buffer[i+1] = alphabet[let1++];
				buffer[i+2] = '\n';
				if (let1 == alphaLen) {
				    let1 = 0;
				    let0++;
				    if (let0 == alphaLen)
						let0 = 0;
				}
		    }
		}

		// Write the first sequence out.
		write(STDOUT_FILENO, buffer, bufLen);

		// Special case for length 2, we're already done.
		if (len == 2)
		    continue;

		// Set all the letters to 0.
		for (i=0;i<len;i++)
		    letters[i] = 0;

		// Now on each iteration, increment the the third to last letter.
		i = len-3;
		int k = 0;
		//do{
		int start = i;
		float a = alphaLen-1;
		float b = len-2;

		printf("%i\n", (len-3) * alphaLen);

		int extra = 0;
		int y = len-3;
		while(y > 0){
			extra += pow(alphaLen, y)-1;
			y -= 1;
		}
		int c = (pow(alphaLen, len-2)-1) + extra;
		printf("zzzzzzzz: %i\n", c);
		#pragma omp for
		for (k = c; k > 0; k--){
		    char c;
		    int  j;

		    letters[i]++;

		    if (letters[i] >= alphaLen)
				letters[i] = 0;

		    c = alphabet[letters[i]];
		    for (j=i; j<bufLen; j+=stride)
				buffer[j] = c;

		    if (letters[i] != 0) {
				write(STDOUT_FILENO, buffer, bufLen);
				i = len - 3;
				continue;
		    }

		    i--;
		    if (i < 0)
				k = 0;
		}
		// while(1);
    }

    // Clean up.
    free(letters);
    free(buffer);
}