const int g_i1 = 10, g_i2 = 5;
const char g_c1 = 'a', g_c2 = 'A';
int g_i3, g_i4;
char g_c3, g_c4;

void func1()
{
    int g_i2;
    char g_c1;
    g_i2 = 6;
    g_c1 = 'C';
    printf("\n>> func1 test");
    printf("\ng_i1 = ", g_i1);
    printf("\ng_c2 = ", g_c2);
    g_i4 = 100;
    g_c3 = 'H';
    printf("\ng_i4 = ", g_i4);
    printf("\ng_c3 = ", g_c3);
    printf("\ng_i2 = ", g_i2);
    printf("\ng_c1 = ", g_c1);
    return;
}

int func2()
{
    int a;
    char b;
    printf("\n>> func2 test");
    printf("\ninput integer a:");
    scanf(a);
    printf("confirm a: ", a);
    printf("\ninput character b:");
    scanf(b);
    printf("confirm b: ", b);
    return (0);
}

char func3()
{
    int a,b,c;
    printf("\n>> func3 test");
    a = 1+(2-3)*4;
    b = g_i1/g_i2;
    c = -(a+b);
    printf("\nc = ", c);
    return ('K');
}

void func4(int p1)
{
    printf("\n>> func4 test");
    while(p1 < 4)
    {
        if(p1 == 0)
        {
            printf("\np1 = 0");
        }
        else if(p1 == 1)
        {
            printf("\np1 = 1");
        }
        else if(p1 == 2)
        {
            printf("\np1 = 2");
        }
        else
        {
            printf("\np1 = ", p1);
        }
        p1 = p1+1;
    }
    return;
}

int func5(char p1)
{
    char c[3];
    printf("\n>> func5 test");
    c[0] = 'a';
    c[1] = 'g';
    c[2] = 'z';
    g_c4 = c[1];
    printf("\nc[1] = ", g_c4);
    return (0);
}

char func6(int p1, char p4)
{
    int a;
    char c[26], now;
    printf("\n>> func6 test");
    now = 'a';
    a = 0;
    while(a < 26)
    {
        c[a] = now;
        now = now+1;
        a = a+1;
    }
    a = 0;
    printf("\n");
    while(a < 26)
    {
        now = c[a];
        printf(now);
        a = a+1;
    }
    return ('Z');
}

void main()
{
    int res_i;
    char res_c;
    printf("Begin");
    func1();
    printf("\n<< func1 return");
    res_i = func2();
    printf("\n<< func2 return: ", res_i);
    res_c = func3();
    printf("\n<< func3 return: ", res_c);
    func4(res_i);
    printf("\n<< func4");
    res_i = func5(res_c);
    func5(99);
    printf("\n<< func5 return: ", res_i);
    res_c = func6(52,97);
    printf("\n<< func6 return: ", res_c);
    printf("\nEnd\n");
}