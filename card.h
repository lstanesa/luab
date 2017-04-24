#ifndef __CARD_H__
#define __CARD_H__

#define CARD_NAME "Card"

typedef struct {
	char *name;
	char *desc;
	int power;
	int fort;
} card_t;

typedef struct {
    card_t *card;
} lcard_userdata_t;

static card_t *card_new(char *name, char *desc, int power, int fort);
static int card_inc_hp(card_t *card, int n);
static void card_attack(card_t *card, card_t *target);
static char *card_getname(card_t *card);
static char *card_getdesc(card_t *card);
static int card_getpower(card_t *card);
static int card_getfort(card_t *card);
static void card_free(card_t *card);
static lcard_userdata_t *lcard_check(lua_State *L, int n, char *meta);

#endif
