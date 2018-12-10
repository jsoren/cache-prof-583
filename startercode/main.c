#include <stdio.h>

void loadprint(int idx, int* addr)
{
	printf("load %d from %p\n", idx, (void*) addr);
}

void storeprint(int idx, int* addr)
{
	printf("store %d to %p\n", idx, (void*) addr);
}

// void slowfunc(unsigned int* arr1, unsigned int* arr2, unsigned int size1, unsigned int size2);

int main()
{
	const unsigned int size1 = 1e3;
	unsigned int arr1[size1];

	const unsigned int size2 = 3e4;
	unsigned int arr2[size2];

	for (unsigned int ii = 0; ii < 10; ++ii)
	{
		for (unsigned int i = 0; i < 6e4; ++i)
		{
			unsigned int idx = i * (i+1);
			unsigned int j = (idx % (size1-1)) + 1;
			unsigned int k = (idx % (size2-1)) + 1;

			arr1[j]++;
			arr2[k]++;
		}

		arr1[0]++;
		arr2[0]++;
	}

	printf("done\n");
}

// void slowfunc(unsigned int* arr1, unsigned int* arr2, unsigned int size1, unsigned int size2)
// {
// 	for (unsigned int i = 0; i < 6e4; ++i)
// 	{
// 		unsigned int idx = i * (i+1);
// 		unsigned int j = idx % size1;
// 		unsigned int k = idx % size2;

// 		arr1[j]++;
// 		arr2[k]++;
// 	}
// }
