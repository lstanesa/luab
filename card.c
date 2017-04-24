#include "card.h"

static card_t *card_new(char *name, char *desc, int power, int fort) {
    card_t *card;
    card = (card_t*)malloc(sizeof(*card));
    card->name = name;
    card->desc = desc;
    card->power = power;
    card->fort = fort;
    return card;
}

static int card_inc_hp(card_t *card, int n) {
    card->fort += n;
    return card->fort;
}

static void card_attack(card_t *attacker, card_t *target) {
    card_inc_hp(target, attacker->power);
    card_inc_hp(attacker, target->power);
}

static char *card_getname(card_t *card) {
    return card->name;
}

static char *card_getdesc(card_t *card) {
    return card->desc;
}

static int card_getpower(card_t *card) {
    return card->power;
}

static int card_getfort(card_t *card) {
    return card->fort;
}

static void card_free(card_t *card) {
    if(card == NULL)
        return;

    free(card);
}

static lcard_userdata_t *lcard_check(lua_State *L, int n, char *meta) {
    return (lcard_userdata_t*)luaL_checkudata(L, n, meta);
}

static int lcard_new(lua_State *L) {
    lcard_userdata_t *lcard;
    const char *name;
    const char *desc;
    int power;
    int fort;
    
    name = luaL_checkstring(L, 1);
    desc = luaL_checkstring(L, 2);
    power = luaL_checkint(L, 3);
    fort = luaL_checkint(L, 4);

    if(name == NULL || desc == NULL || power == NULL || fort == NULL)
        luaL_error(L, "one or more required parameters is empty");

    lcard = (lcard_userdata_t*)lua_newuserdata(L, sizeof(*lcard));
    lcard->card = NULL;
    
    luaL_getmetatable(L, CARD_NAME);
    lua_setmetatable(L, -2);
    
    lcard->card = card_new(name, desc, power, fort);

    return 1;
}

static int lcard_inc_hp(lua_State *L) {
    lcard_userdata_t *lcard;
    int n;

    lcard = lcard_check(L, 1, CARD_NAME);
    n = luaL_checkint(L, 2);
    card_inc_hp(lcard->card, n);
    return 0;
}

static int lcard_attack(lua_State *L) {
    lcard_userdata_t *lcard;
    lcard_userdata_t *target;

    lcard = lcard_check(L, 1, CARD_NAME);
    target = lcard_check(L, 2, CARD_NAME);

    card_attack(lcard->card, target->card);
    
    return 0;
}

static int lcard_getname(lua_State *L) {
    lcard_userdata_t *lcard;
    lcard = lcard_check(L, 1, CARD_NAME);
    lua_pushstring(L, card_getname(lcard->card));
    return 1;
}

static int lcard_getdesc(lua_State *L) {
    lcard_userdata_t *lcard;
    lcard = lcard_check(L, 1, CARD_NAME);
    lua_pushstring(L, card_getdesc(lcard->card));
    return 1;
}

static int lcard_getpower(lua_State *L) {
    lcard_userdata_t *lcard = lcard_check(L, 1, CARD_NAME);
    lua_pushinteger(L, card_getpower(lcard->card));
    return 1;
}

static int lcard_getfort(lua_State *L) {
    lcard_userdata_t *lcard = lcard_check(L, 1, CARD_NAME);
    lua_pushinteger(L, card_getfort(lcard->card));
    return 1;
}

static int lcard_free(lua_State *L) {
    lcard_userdata_t *lcard;
    lcard = lcard_check(L, 1, CARD_NAME);
    
    if(lcard->card != NULL)
        card_free(lcard->card);

    lcard->card = NULL;

    return 0;
}

static int lcard_tostring(lua_State *L) {
    lcard_userdata_t *lcard;

    lcard = lcard_check(L, 1, CARD_NAME);
    lua_pushfstring(L, "%s(%d/%d)", lcard->card->name, lcard->card->power, lcard->card->fort);
}

static const struct luaL_Reg lcard_methods[] = {
    { "__gc",       lcard_free },
    { "__tostring", lcard_tostring },
    { NULL, NULL }
};


static const struct luaL_Reg lcard_functions[] = {
    { "inc_hp",     lcard_inc_hp },
    { "attack",     lcard_attack },
    { "getname",    lcard_getname },
    { "getdesc",    lcard_getdesc },
    { "getpower",   lcard_getpower },
    { "getfort",      lcard_getfort },
    { "new", lcard_new },
    { NULL, NULL }
};

/*static int luaopen_card(lua_State *L) {
    luaL_newmetatable(L, CARD_NAME);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, lcard_methods, 0);
    luaL_newlib(L, lcard_functions);

    return 1;
}*/

static int luaopen_card(lua_State *L) {
    int lib_id, meta_id;

    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, CARD_NAME);
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, lcard_methods, 0);
    
    luaL_newlib(L, lcard_functions);
    lua_setfield(L, meta_id, "__index");
    
    luaL_newlib(L, lcard_methods);
    lua_setfield(L, meta_id, "__metatable");

    lua_setmetatable(L, lib_id);
    lua_setglobal(L, CARD_NAME);
}

