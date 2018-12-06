#include <stdio.h>

void loadprint(int idx, int* addr)
{
	printf("load %d from %p\n", idx, (void*) addr);
}

void storeprint(int idx, int* addr)
{
	printf("store %d to %p\n", idx, (void*) addr);
}

int main()
{
	const unsigned int size1 = 100;
	int arr1[size1];

	const unsigned int size2 = 3000;
	int arr2[size2];

	for (unsigned int i = 0; i < 60000; ++i)
	{
		unsigned int j = i % size1;
		arr1[j] += i;

		unsigned int k = i % size2;
		arr2[k] += i;
	}

	printf("done\n");
}
