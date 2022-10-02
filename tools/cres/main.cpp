#include "res.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    BmpReader r;
    r.LoadFile(argv[1]);
    if(!r.GetHeader())
    {
        r.PrintImageInfo();
        return -1;
    }
    else
    {
        r.PrintImageInfo();
        r.MakeRes();
        //r.SaveOutput();
    }


    return 0;
}