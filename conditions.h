#ifndef __CONDITIONS_H__
#define __CONDITIONS_H__

#define COND_NAME "conditions"

#include "card.h"

static int cond_card_isalive(card_t *card);
static int cond_card_wouldkill(card_t *target, card_t *attacker);

#endif
