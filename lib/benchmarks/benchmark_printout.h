#pragma once

#include "statistics.h"
#include <ConsoleTable.h>

void print(struct statistics &stats);
void addTable(ConsoleTable &table, struct statistics &stats);