//
// Created by Birkee
//


// Xiangyu : This class is for compression, however, the performance is not good
// Sometimes make files bigger.

// Omitting this one


#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP


#include <cstdio>




class Huffman {
public:

    static constexpr char leaf[16]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    typedef unsigned char byte;


    //Huffman() = default;

    class node
    {
    public:
        node *Basic;
        node *Lside;
        node *Rside;
        long  Symbol;
        byte  Volume;
    public:
        node();
        node(long n,const byte &t);
    };

    node *original(node *t);
    node *maintain(node *r);
    void encode(const char *to,const char *from);
    void decode(const char *to,const char *from);


};


#endif //ECE180_HUFFMAN_HPP
