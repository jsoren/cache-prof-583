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
	int x = 0;
	int y = x;
	
	int arr[4];
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			arr[i+2*j] = i+j;
		}
	}

	printf("done\n");
}
