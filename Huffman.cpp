//
// Created by Birkee
//


// Xiangyu : This class is for compression, however, the performance is not good
// Sometimes make files bigger.


#include "Huffman.hpp"
#include <iostream>


constexpr char Huffman::leaf[16];

Huffman::node::node()
{
    Basic = Lside = Rside = (Huffman::node*)leaf;
    Symbol=1;
}

Huffman::node::node(long n, const Huffman::byte &t)
        :Symbol(n)
        ,Volume(t)
{
    Basic=Lside=Rside=(Huffman::node*)leaf;
}




Huffman::node *Huffman::original(Huffman::node *t)
{
    Huffman::node *p,*r;

    for(r=t;r-t<256;r++)
    {
        r->Symbol=0;
        r->Volume=r-t;
    }
    for(p=t;p+1<r;p++,r++)
    {
        r->Symbol=0;
        r->Volume=0;
        r->Lside=p;
        p->Basic=r;
        p++;
        r->Rside=p;
        p->Basic=r;
    }
    return p;
}

Huffman::node *Huffman::maintain(Huffman::node *r)
{
    Huffman::node *p,*q,*a,*b,*t;
    for(p=r->Basic;leaf!=(char*)p;p=r->Basic)
    {
        r=p;
        p=r->Lside;
        q=r->Rside;
        if(p->Symbol>q->Symbol)
        {
            r->Lside=q;
            r->Rside=p;
            t=p,p=q,q=t;
        }
        if(p->Symbol<q->Symbol)
        {
            a=q->Lside;
            b=q->Rside;
            if(b->Symbol>p->Symbol)
            {
                t=r->Basic;
                if(leaf!=(char*)t)
                {
                    if(t->Lside==r)t->Lside=q;else t->Rside=q;
                }

                r->Rside=a;
                if(leaf!=(char*)a)a->Basic=r;
                r->Symbol=p->Symbol+a->Symbol;

                q->Basic=t;
                r->Basic=q;
                q->Symbol=r->Symbol+b->Symbol;

                p=r,r=q,q=b;
                if(p->Symbol>q->Symbol)
                {
                    r->Lside=q;
                    r->Rside=p;
                }
                else
                {
                    r->Lside=p;
                    r->Rside=q;
                }
                continue;
            }
        }
        r->Symbol=p->Symbol+q->Symbol;
    }
    return r;
}



void Huffman::decode(const char *to, const char *from)
{
    FILE *sfa=fopen(from,"rb");
    FILE *dst=fopen(to,"wb");
    Huffman::node arr[512];
    Huffman::node *p,*q;
    char c;
    int i;

    if(sfa== nullptr || dst== nullptr)
    {
        std::cerr << "Empty file" << std::endl;
        return;
    }
    p=original(arr);
    for(;;)
    {
        c=fgetc(sfa);
        if(feof(sfa))break;

        for(i=7;i>=0;i--)
        {
            q = (c>>i)&1 ? p->Rside : p->Lside;
            if(leaf==(char*)q)
            {
                putc((char)p->Volume,dst);
                p->Symbol++;
                p=maintain(p);
                i++;
            }
            else p=q;
        }
    }
    if(leaf==(char*)p->Lside && leaf==(char*)p->Rside)
    {
        putc((char)p->Volume,dst);
    }
    fclose(sfa);
    fclose(dst);
}
        
        
        

void Huffman::encode(const char *to, const char *from)
{
    FILE *src=fopen(from,"rb");
    FILE *sfa=fopen(to,"wb");
    Huffman::node *p,*q,*r;
    Huffman::node arr[512];
    bool t[256];
    int i,j;
    byte c,o;

    if(src == nullptr || sfa== nullptr)
    {
        std::cerr << "No in file" << std::endl;
        return;
    }
    original(arr);
    c=0,i=j=0;
    for(;;)
    {
        o=fgetc(src);
        if(feof(src))break;

        r=arr+byte(o);
        for(p=r;;p=q)
        {
            q=p->Basic;
            if(leaf==(char*)q)
            {
                break;
            }
            if(q->Lside==p)
            {
                t[i++]=0;
            }
            else
            {
                t[i++]=1;
            }
        }
        r->Symbol++;
        maintain(r);
        while(i)
        {
            i--;
            c= c<<1 | int(t[i]);
            j++;
            if(j==8)
            {
                putc(c,sfa);
                c=0,j=0;
            }
        }
    }
    if(j)
    {
        putc(c<<(8-j), sfa);
    }
    fclose(src);
    fclose(sfa);
}







