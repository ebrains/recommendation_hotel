#include<iostream>
#include<stdlib.h>

using namespace std;

int *bubbleSort(int* arr, int len)
{
	for(int i=0; i<len; i++)
	{
		for(int j=0; j<len-i; j++)
		{	
			if(arr[j]>arr[j+1])
			{	
				int tmp = arr[j];
				arr[j] =arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
	return arr;
}
int main(int argc, char **argv)
{
	if(argc<2)
	{
		cout<<"Please input something" <<endl;
	}
	
	int *a = (int *)malloc((argc-1) * sizeof(int));
	for(int i=1; i<argc; i ++)
	{
		int tmp = atoi(argv[i]);
		*a = tmp;
		a++;
	}
	
	int *b = bubbleSort((a-argc),argc-1);
	for(int i=0; i<argc-1;i++)
	{
		cout<<*(b+i) <<' ';
	}
	free(a);
	cout<<endl;
	return 1;
	
}
