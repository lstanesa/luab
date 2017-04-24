#include "conditions.h"

static int cond_card_isalive(card_t *card) {
	return card->fort > 0;
}

static int cond_card_wouldkill(card_t *target, card_t *attacker) {
	return (target->fort - attacker->power) <= 0;
}

static int lcond_card_isalive(lua_State *L) {
	lcard_userdata_t *lcard = lcard_check(L, 1, CARD_NAME);
	lua_pushinteger(L, cond_card_isalive(lcard->card));
	return 1;
}

static int lcond_card_wouldkill(lua_State *L) {
	lcard_userdata_t *target = lcard_check(L, 1, CARD_NAME);
	lcard_userdata_t *attacker = lcard_check(L, 2, CARD_NAME);
	lua_pushinteger(L, cond_card_wouldkill(target->card, attacker->card));
	return 1;
}

static const struct luaL_Reg lcond_methods[] = {
	{ "isalive", lcond_card_isalive },
	{ "wouldkill", lcond_card_wouldkill },
	{ NULL, NULL }
};

static int luaopen_conditions(lua_State *L) {
	luaL_newmetatable(L, COND_NAME);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	luaL_setfuncs(L, lcond_methods, 0);
    luaL_newlib(L, lcond_methods);
	lua_setglobal(L, COND_NAME);
	return 1;
}
