#ifndef SORT_H_
#define SORT_H_

int binary(int array[],int n,int K)
{
    int l = -1;
    int r = n;
    while(l+1!=r)
    {
        int i = (l + r) / 2;
        if(K<array[i])
            r = i;
        if(K==array[i])
            return i;
        if(K>array[i])
            l = i;
    }
    return n;
}

#endif