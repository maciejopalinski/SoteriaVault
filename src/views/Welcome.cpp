#include "Welcome.h"

WelcomeView::WelcomeView(CDKSCREEN *screen) : BaseView(screen) {}

bool WelcomeView::activate()
{
    curs_set(0);

    const char* ascii_data[] = {
        R"( __       _            _                          _ _   )",
        R"(/ _\ ___ | |_ ___ _ __(_) __ _  /\   /\__ _ _   _| | |_ )",
        R"(\ \ / _ \| __/ _ | '__| |/ _` | \ \ / / _` | | | | | __|)",
        R"(_\ | (_) | ||  __| |  | | (_| |  \ V | (_| | |_| | | |_ )",
        R"(\__/\___/ \__\___|_|  |_|\__,_|   \_/ \__,_|\__,_|_|\__|)",
        R"()", R"()",
        R"(                 Press ANY key to START                 )"
    };

    CDKLABEL* ascii_art = newCDKLabel(
        screen,
        CENTER, CENTER,
        (char**) ascii_data, 8,
        false, false
    );

    drawCDKLabel(ascii_art, 0);

    int input = getch();
    curs_set(1);
    destroyCDKLabel(ascii_art);
    
    if(input == KEY_F(1) || input == KEY_ESC) return false;
    else return true;
}