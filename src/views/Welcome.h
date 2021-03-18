#ifndef WELCOME_VIEW_H
#define WELCOME_VIEW_H

#include "BaseView.h"

class WelcomeView : BaseView
{
    public:
        WelcomeView(CDKSCREEN *screen);
        
        bool activate();
};

#endif