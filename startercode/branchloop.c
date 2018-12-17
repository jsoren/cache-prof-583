#include <stdio.h>

void loadprint(int idx, int* addr)
{
	printf("load\t%d\tat\t%p\n", idx, (void*) addr);
}

void storeprint(int idx, int* addr)
{
	printf("store\t%d\tat\t%p\n", idx, (void*) addr);
}

int main()
{
	const unsigned int size1 = 1e3;
	unsigned int arr1[size1];

	const unsigned int size2 = 3e5;
	unsigned int arr2[size2];


	for (unsigned int i = 0; i < 6e6; ++i)
	{
		unsigned int idx = i * (i+1);
		unsigned int j = idx % (size1-1);
		unsigned int k = idx % (size2-1);

		unsigned int tmp = arr1[j] + arr2[k] + i;
		
		arr1[j+1] = tmp;

		if (j > size1 / 2)
		{
			arr2[k] = tmp * tmp;
		}
	}

	printf("done\n");
}

