#include "Login.h"

static int entryCB(EObjectType cdktype, void *object, void *clientData, chtype key)
{
    CDKBUTTONBOX *buttonbox = (CDKBUTTONBOX *) clientData;
    (void) injectCDKButtonbox(buttonbox, key);
    return true;
}

LoginView::LoginView(CDKSCREEN *screen) : BaseView(screen) {}

bool LoginView::prompt_filename()
{
    // const char *items[] = {
    //     "Item 1",
    //     "Item 2",
    //     "Item 3",
    //     "(+) Create New Profile"
    // };

    // CDKSCROLL *scroll_list = newCDKScroll(
    //     screen,
    //     LEFT, CENTER,
    //     RIGHT,
    //     0, COLS / 2,
    //     "Choose profile",
    //     (CDK_CSTRING2) items,
    //     4,
    //     true,
    //     A_REVERSE,
    //     true, false
    // );

    // int selection = activateCDKScroll(scroll_list, 0);
    
    // destroyCDKScroll(scroll_list);

    // if(selection == 3)
    // {
    //     CreateView create = CreateView(screen);
    //     if(!create.activate()) return false;
    // }

    // return false;

    CDKENTRY *profile_entry = newCDKEntry(
        screen,
        CENTER,
        CENTER,
        "<C>Filename",
        "Filename: ",
        A_NORMAL,
        '_',
        vMIXED,
        40,
        0,
        256,
        true,
        false
    );

    const char *buttons[] = {"  OK  ", " EXIT "};
    CDKBUTTONBOX *button_box = newCDKButtonbox(
        screen,
        getbegx(profile_entry->win),
        getbegy(profile_entry->win) + profile_entry->boxHeight - 1,
        1,
        profile_entry->boxWidth - 1,
        0,
        1,
        2,
        (CDK_CSTRING2) buttons,
        2,
        A_REVERSE,
        true,
        false
    );

    /* Set the lower left and right characters of the box. */
    setCDKEntryLLChar(profile_entry, ACS_LTEE);
    setCDKEntryLRChar(profile_entry, ACS_RTEE);
    setCDKButtonboxULChar(button_box, ACS_LTEE);
    setCDKButtonboxURChar(button_box, ACS_RTEE);

    bindCDKObject(vENTRY, profile_entry, KEY_TAB, entryCB, button_box);

    drawCDKButtonbox(button_box, true);

    filename = copyChar(activateCDKEntry(profile_entry, 0));

    if(profile_entry->exitType == vESCAPE_HIT) return false;

    destroyCDKButtonbox(button_box);
    destroyCDKEntry(profile_entry);

    return button_box->currentButton == 0;
}

bool LoginView::prompt_password()
{
    CDKENTRY *password_entry = newCDKEntry(
        screen,
        CENTER,
        CENTER,
        "<C>Password",
        "Password: ",
        A_NORMAL,
        '_',
        vHMIXED,
        40,
        0,
        256,
        true,
        false
    );

    const char *buttons[] = {"  OK  ", " BACK "};
    CDKBUTTONBOX *button_box = newCDKButtonbox(
        screen,
        getbegx(password_entry->win),
        getbegy(password_entry->win) + password_entry->boxHeight - 1,
        1,
        password_entry->boxWidth - 1,
        0,
        1,
        2,
        (CDK_CSTRING2) buttons,
        2,
        A_REVERSE,
        true,
        false
    );

    /* Set the lower left and right characters of the box. */
    setCDKEntryLLChar(password_entry, ACS_LTEE);
    setCDKEntryLRChar(password_entry, ACS_RTEE);
    setCDKButtonboxULChar(button_box, ACS_LTEE);
    setCDKButtonboxURChar(button_box, ACS_RTEE);

    bindCDKObject(vENTRY, password_entry, KEY_TAB, entryCB, button_box);

    drawCDKButtonbox(button_box, true);

    password = copyChar(activateCDKEntry(password_entry, 0));

    if(password_entry->exitType == vESCAPE_HIT) return false;

    destroyCDKButtonbox(button_box);
    destroyCDKEntry(password_entry);

    return button_box->currentButton == 0;
}

bool LoginView::activate()
{
    filename:
    if(!prompt_filename()) return false;

    password:
    if(!prompt_password()) goto filename;

    if(!login()) goto password;

    return true;
}

bool LoginView::login()
{
    profile = Profile();

    string filename(this->filename);
    string password(this->password);

    profile.setFilename(filename);
    if(!profile.loadFromFile()) return false;

    if (profile.authenticate(password))
    {
        profile.decryptData();
        return true;
    }

    return false;
}