void move(int n,char x,char y,char z) {
    if (n==1) {
        printf(x);
        printf("--->",z);
        printf("\n");
    }else {
        move(n-1,x,z,y);
        printf(x);
        printf("--->",z);
        printf("\n");
        move(n-1,y,x,z);
    }
}

void main()
{
    int n;
    printf("Input number : ");
    scanf(n);
    move(n,'X','Y','Z');
    return (0);
}