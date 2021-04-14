#ifndef CREATE_VIEW_H
#define CREATE_VIEW_H

#include "BaseView.h"
#include "../Profile.h"

class CreateView : BaseView
{
    private:
        bool prompt_filename();
        bool prompt_password();
        bool data_check();

    public:
        Profile profile;
        const char* filename;
        const char* password;

        CreateView(CDKSCREEN *screen);

        bool activate();
};

#endif