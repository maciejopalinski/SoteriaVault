#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include "BaseView.h"
#include "Create.h"
#include "../Profile.h"

class LoginView : BaseView
{
    private:
        bool prompt_filename();
        bool prompt_password();

    public:
        Profile profile;
        const char* filename;
        const char* password;

        LoginView(CDKSCREEN *screen);

        bool activate();
        
        bool login();
};

#endif