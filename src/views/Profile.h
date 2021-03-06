#ifndef PROFILE_VIEW_H
#define PROFILE_VIEW_H

#include "BaseView.h"
#include "../Profile.h"

class ProfileView
{
    private:
        Profile profile;

    public:
        ProfileView(Profile profile);
        
        bool activate(CDKSCREEN *screen);
};

#endif