#ifndef NON_SLIDING_MOVES
#define NON_SLIDING_MOVES

#include "globals.h"

u64 soutOne(u64 bb);
u64 nortOne(u64 bb);
u64 eastOne(u64 bb);
u64 westOne(u64 bb);

u64 noEaOne(u64 bb);
u64 noWeOne(u64 bb);
u64 soEaOne(u64 bb);
u64 soWeOne(u64 bb);

u64 get_king_attacks(int sq);
u64 king_attacks(u64 kingSet);
u64 get_knight_attacks(int sq);
u64 knight_attacks(u64 knightSet);

void init_king_attacks();
void init_knight_attacks();

/*https://chessprogramming.wikispaces.com/Pawn+Pushes+%28Bitboards%29
*/

u64 wSinglePushTargets(u64 wpawns, u64 empty);
u64 wDblPushTargets(u64 wpawns, u64 empty);

u64 bSinglePushTargets(u64 bpawns, u64 empty);
u64 bDoublePushTargets(u64 bpawns, u64 empty);

u64 wPawnsAble2Push(u64 wpawns, u64 empty);
u64 wPawnsAble2DblPush(u64 wpawns, u64 empty);

u64 bPawnsAble2Push(u64 bpawns, u64 empty);
u64 bPawnsAble2DblPush(u64 bpawns, u64 empty);

u64 wPawnEastAttacks(u64 wpawns);
u64 wPawnWestAttacks(u64 wpawns);

u64 bPawnEastAttacks(u64 bpawns);
u64 bPawnWestAttacks(u64 bpawns);

u64 wPawnAnyAttacks(u64 wpawns);
u64 wPawnDblAttacks(u64 wpawns);
u64 wPawnSingleAttacks(u64 wpawns);

u64 bPawnAnyAttacks(u64 bpawns);
u64 bPawnDblAttacks(u64 bpawns);
u64 bPawnSingleAttacks(u64 bpawns);

#endif