#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <cdk.h>
using namespace std;

class BaseView
{
    protected:
        CDKSCREEN *screen;

    public:
        BaseView(CDKSCREEN *screen);
};

#endif