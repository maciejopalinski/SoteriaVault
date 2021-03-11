#include "Profile.h"

ProfileView::ProfileView(Profile profile)
{
    this->profile = profile;
}

bool ProfileView::activate(CDKSCREEN *screen)
{
    CDKMENTRY *content_entry = newCDKMentry(
        screen,
        CENTER, CENTER,
        "<C>Content", 0,
        A_NORMAL, '_', vMIXED,
        0, 0, 100000, 0,
        true, false
    );

    setCDKMentryValue(content_entry, (CDK_CSTRING) profile.getData());
    
    activateCDKMentry(content_entry, 0);
    string data = strdup(content_entry->info);
    profile.setData(data);
    
    destroyCDKMentry(content_entry);
    return true;
}