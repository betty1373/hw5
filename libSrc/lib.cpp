#include "../version.h"
#include "lib.h"
namespace libSrc 
{
    int version()
    {
        return PROJECT_VERSION_PATCH;
    }

    int fact(int V) {
        return (V==0 || V==1) ? 1 :fact(V-1) * V;
    }
}
