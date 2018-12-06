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
	const unsigned int size = 60;
	int arr[size];
	for (unsigned int i = 0; i < 60; ++i)
	{
		arr[i] = i;
	}

	printf("done\n");
}
