int a[5];

void quicksort(int l,int r) 
{ 
    int i,j;
    int tmp,tt;
    i = l;
    j = r;
    tmp = a[(l+r)/2];
    while(i <= j)
    {  
        while(a[j] > tmp)
        {
            j = j-1;
        }
        while(a[i] < tmp)
        {
            i = i+1;
        }
        if(i<=j)
        {
            tt=a[i];
            a[i]=a[j];
            a[j]=tt;
            i = i+1;
            j = j-1;
        }
    }
    if(i<r)
    {
        quicksort(i,r);
    }
    if(j>l)
    {
        quicksort(l,j);
    }
}

void main()
{
	int i,j;
    a[0] = 8;
    a[1] = 9;
    a[2] = 5;
    a[3] = 7;
    a[4] = 6;
    i = 0;
    printf("Before sort: ");
	while(i<5)
    {
        j = a[i];
		printf(" ",j);
        i = i+1;
    }
    printf("\nAfter  sort: ");
	quicksort(0,4);
    i = 0;
	while(i<5)
    {
        j = a[i];
		printf(" ",j);
        i = i+1;
    }
	printf("\n");
	return (0);
}