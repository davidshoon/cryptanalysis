/*

	An example of differential cryptanalysis.

	by David Shoon 2022-12-01

Explanation:

Let's say we take a look at the 'differences' between terms of the fibonacci sequence:

1 1 2 3 5 8 13 21

fibonacci

difference:
 0 1 1 3 5 8 13 21

i.e. the differences between the terms of the fibonacci sequence is itself.

...


2, 4, 8, 16, 32

2, 4, 8, 16,...

i.e. the differences between the terms of the 2^N sequence is the 2^N sequence itself.

....

2, 4, 6, 8, 10

2, 2, 2, 2, 2, 

i.e. the differences between the even numbers is just 2.

...

According to the theory on differential cryptanalysis: https://en.wikipedia.org/wiki/Differential_cryptanalysis

delta_x = 1 (we can choose whatever)

f(x), f(x + delta_x)...

delta_y = f(x + delta_x) + f(x) ; Note the plus sign is the XOR operator -- this isn't exactly a "difference" (in terms of arithmetic on the natural numbers), at least not the way we know it...

However...

0 xor 0 = 0
1 xor 0 = 1
0 xor 1 = 1
1 xor 1 = 0

0 + 0 = 0
1 + 0 = 1
0 + 1 = 1
1 + 1 = 2 (but then, after modulo 2, that's just 0).

therefore, xor = addition modulo 2.


however:

0 - 0 = 0
1 - 0 = 1
0 - 1 = -1 (however, after modulo 2, that's just 1).
1 - 1 = 0 

therefore, xor = substraction modulo 2.

Therefore: xor is both an addition and a substraction operator. (It makes no difference -- it's like an analog clock, with '0' at the twelve o'clock position, and '1' at the six o'clock position).

so the differential cryptanalysis relying on "xor" allows us to "take the difference modulo 2".

...

Q. What is this differential cryptanalysis doing?

A. It's just trying to find the difference of results from calling the function f() with differing inputs, where the inputs differ by delta_x. That is, as we try and modify our inputs (in some predictable manner), we analyse
if the outputs are being made in a predictable manner. That's all differential cryptanalysis is doing, even though they may use many chosen plaintexts to analyse this.

Usually the function f() being analysed is the S-Box, which is non-linear, and trying to extract any sort of bias from it. Weak S-boxes have bias, and is usually a form of a flaw in the cryptographic function.

...

Further reading:

https://en.wikipedia.org/wiki/S-box
https://en.wikipedia.org/wiki/Linear_cryptanalysis
https://en.wikipedia.org/wiki/Differential_cryptanalysis

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// our function to analyse
int func(int x)
{
	return x + 1; // this appears to be uniformly distributed, but obviously has a distinct pattern to it... so our diff analysis should show up quite easily...
}

int main()
{
	const int total = 1000;
	int results[total];
	int results_x_xor_delta_x[total];
	const int delta_x = 1;

	srand(0);

	// generate results from function to analyse

	int x = 0;

	for (int i = 0; i < total; i++) {
		results[i] = func(x);
		results_x_xor_delta_x[i] = func(x ^ delta_x); // the diff is just 1, since i++

		x += delta_x;
	}

	// analyse function by taking diff of xor'd results

	int diff_results[total];

	for (int i = 0; i < total; i++) {
		diff_results[i] = results[i] ^ results_x_xor_delta_x[i];

		// you'll find in the output (stdout), that the values are all repeating/predictable, i.e. the f()/S-box/etc function being analysed is weak.
		printf("%d\n", diff_results[i]);
	}

	// output original results to stderr

	for (int i = 0; i < total; i++) {
		fprintf(stderr, "%d\n", results[i]);
	}
}
