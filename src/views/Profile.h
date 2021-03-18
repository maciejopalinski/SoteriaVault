#ifndef PROFILE_VIEW_H
#define PROFILE_VIEW_H

#include "BaseView.h"
#include "../Profile.h"

class ProfileView : BaseView
{
    public:
        Profile profile;

        ProfileView(CDKSCREEN *screen, Profile profile);
        
        bool activate();
};

#endif