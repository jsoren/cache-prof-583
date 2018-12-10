#include <stdio.h>

void loadprint(int idx, int* addr)
{
	printf("load %d from %p\n", idx, (void*) addr);
}

void storeprint(int idx, int* addr)
{
	printf("store %d to %p\n", idx, (void*) addr);
}

void slowfunc(void);

int main()
{
	const unsigned int size1 = 1e3;
	unsigned int arr1[size1];

	const unsigned int size2 = 3e4;
	unsigned int arr2[size2];

	for (unsigned int i = 0; i < 6e6; ++i)
	{
		unsigned int idx = i * (i+1);
		unsigned int j = idx % size1;
		unsigned int k = idx % size2;

		slowfunc();

		arr1[j] += i;
		arr2[k] += i;
	}

	printf("done\n");
}

void slowfunc(void)
{
	int x = 0;
}
