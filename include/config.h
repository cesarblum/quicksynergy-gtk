#ifndef CONFIG_H
#define CONFIG_H

#include "misc.h"

void load_config(struct entry_data *entries);
void save_config(struct entry_data *entries);
void save_synergy_config(struct entry_data *entries);

#endif
