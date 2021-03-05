#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include "BaseView.h"
#include "../Profile.h"

class LoginView
{
    private:
        bool prompt_filename(CDKSCREEN *screen);
        bool prompt_password(CDKSCREEN *screen);
        bool activate_content(CDKSCREEN *screen);

    public:
        Profile profile;
        const char* filename;
        const char* password;

        bool activate(CDKSCREEN *screen);
        
        bool login();
};

#endif