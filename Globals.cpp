#include "Globals.hpp"

void print_vi(const vi& x)
{
    tr(x,it)
    {
        cout<<","<<*it<<",";
    }
    cout<<endl<<"------------------"<<endl;
}
void print_vd(const vd& x)
{
    tr(x,it)
    {
        cout<<","<<*it<<",";
    }
    cout<<endl<<"------------------"<<endl;
}
void print_vvi(const vvi& x)
{
    tr(x,it)
    {
        print_vi(*it);
    }
}


void print_ii(const ii& x)
{
    cout << "(" << x.first<< "," << x.second << ")" << endl;
}
void print_vii(const vii& x)
{
    tr(x,it)
    {
        print_ii(*it);
    }
}
