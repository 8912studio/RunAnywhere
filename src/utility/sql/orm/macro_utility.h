#pragma once

#define SQL_UTILITY_EXPAND(X) X

#define SQL_UTILITY_JOIN_(A, B, C, D, E, F, G, H, ...) A##B##C##D##E##F##G##H

#define SQL_UTILITY_JOIN(...) SQL_UTILITY_EXPAND( SQL_UTILITY_JOIN_(__VA_ARGS__,,,,,,,) )
