#pragma once

/*
 * A simple random number generator based on Marsaglia's MWC
 */
#include <iostream>

class Prand;
class Prand
{
    private:  unsigned int GetUint()
    {
        m_z = 36969 * (m_z & 65535) + (m_z >> 16);
        m_w = 18000 * (m_w & 65535) + (m_w >> 16);
        return (m_z << 16) + m_w;
    }
    public:  int det_toss(){ 
                 unsigned int u = GetUint();
                 return u%2;
                 //return GetUint()%2;
             }
    public:  int det_number(int max)
            { 
              unsigned int x = GetUint();
              return x%max;
            }
    public:  double GetUniform()
    {
        unsigned int u = GetUint();
        return (u + 1.0) * 2.328306435454494e-10;
    }
    public:  void set_seed(int seed)
    {
        m_z = seed;
        m_w = 110003441;
    }
    public: 
             ptrdiff_t operator() (ptrdiff_t i) {
                 return static_cast<ptrdiff_t>(det_number(i));
             }
    public: 
             template<typename T>
                 void myshuffle(T& cont)
                 { 
                     // Pointer to random number function
                     // Randomize the container using my own random number function
                     //random_shuffle(cont.begin(), cont.end(), p_myrandom);
                     Prand p;
                     random_shuffle(cont.begin(), cont.end(), p);
                 }
    private:
             ptrdiff_t myrandom(ptrdiff_t i)
             {
                 return det_number(i);
             }
    public:
     int m_z, m_w;
};
