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


	for (unsigned int i = 0; i < 6e3; ++i)
	{
		unsigned int idx = i * (i+1);
		unsigned int j = idx % (size1);
		unsigned int k = idx % (size2);

		unsigned int tmp1 = arr1[j];
		unsigned int tmp2 = arr2[k];

		for (unsigned int l = 0; l < 1e3; ++l)
		{
			unsigned int m = (tmp1 + l) % size1;
			unsigned int n = (tmp2 + l) % size2;

			arr1[m] = tmp1 * 2;
			arr2[n] = tmp2 * 2;
		}

		arr1[j] = tmp1 + 1;
		arr2[k] = tmp2 + 1;
	}

	printf("done\n");
}

