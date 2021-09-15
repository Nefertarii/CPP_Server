#include "Metaconfirm.h"

int main() {
    Meta_Parse T;
    (T.MetaLogin("5tempaccount", "passwor"));
    (T.MetaLogin("5tempaccount5", "pasword"));
    (T.MetaLogin("5tempaccount5", "pasword"));
    (T.MetaLogin("5tempaccount5", "pasword"));
    (T.MetaLogin("5tempaccount5", "pasword"));
    (T.MetaLogin("5tempaccount5", "pasword"));
    (T.MetaLogin("5tempaccount5", "paswor5"));
    (T.MetaLogin("5tempaccount5", "paswor5"));
    (T.MetaLogin("5tempaccount5", "password5"));
    (T.MetaRegsiter("0tempaccount0", "password0"));
    (T.MetaChangePassword("2tempaccount2", "password2", "password22"));
}