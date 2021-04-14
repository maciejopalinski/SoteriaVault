#include "Create.h"

static int entryCB(EObjectType cdktype, void *object, void *clientData, chtype key)
{
    CDKBUTTONBOX *buttonbox = (CDKBUTTONBOX *) clientData;
    (void) injectCDKButtonbox(buttonbox, key);
    return true;
}

CreateView::CreateView(CDKSCREEN *screen) : BaseView(screen) {}

bool CreateView::prompt_filename()
{
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

bool CreateView::prompt_password()
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

bool CreateView::data_check()
{
    const char* lines[4] = {
        "You are about to create new profile",
        strcat(copyChar("Filename: ./profiles/"), filename),
        "If file already exists, it will be overwritten!",
        "To continue, press ANY key. To abort, press ESC."
    };

    CDKLABEL* info = newCDKLabel(
        screen,
        CENTER, CENTER,
        (char**) lines, 4,
        false, false
    );

    drawCDKLabel(info, 0);

    int input = getch();
    destroyCDKLabel(info);

    if(input == KEY_F(1) || input == KEY_ESC) return false;
    else return true;
}

bool CreateView::activate()
{
    if(!prompt_filename()) return false;

    password:
    if(!prompt_password()) goto password;

    if(!data_check()) return false;

    profile.setData(">===> default <===<");
    profile.encryptData();

    profile.setFilename(strcat(copyChar("profiles/"), filename));
    profile.saveToFile();

    return true;
}