#ifndef TIMEIT_H
#define TIMEIT_H

#include <time.h>
#include <stdio.h>

#define timeit(expr) 															\
	clock_t start = clock();													\
	expr																		\
	clock_t end = clock();														\
    long double time_spent = (long double)(end - start) / CLOCKS_PER_SEC;		\
    printf("Time, s: %Lf\n", time_spent);

#endif // TIMEIT_H
