const int pstPawnMg[64] =
{
//A1                                H1
  0,  0,  0,  0,  0,  0,  0,   0,
 10, 20,  0,  5,  5,  0, 20,  10,
-10,  0,  5, 15, 15,  5,  0, -10,
-15, -5, 10, 25, 25, 10, -5, -15,
-15, -5,  5, 15, 15,  5, -5, -15,
 45, 55, 65, 75, 75, 65, 55,  45,
105,115,125,135,135,125,115, 105,
  0,  0,  0,  0,  0,  0,  0,   0,
//A8                                H8
 };

const int pstPawnEg[64] =
{
 0,  0,  0,  0,  0,  0,  0,   0,
 3,  1, -1, -3, -3, -1,  1,   3,
 3,  1, -1, -3, -3, -1,  1,   3,
 9,  6,  3,  0,  0,  3,  6,   9,
 12, 9,  6,  3,  3,  6,  9,  12,
 87,85, 83, 81, 81, 83, 85,  87,
 159,157,155,153,153,155,157,159,
 0,  0,  0,  0,  0,  0,  0,   0,
 };


const int pstKnightMg[64] =
{
   -50, -40, -30, -25, -25, -30, -40, -50,
   -35, -25, -15, -10, -10, -15, -25, -35,
   -20, -10,   0,   5,   5,   0, -10, -20, 
   -10,   0,  10,  15,  15,  10,   0, -10, 
	-5,   5,  15,  20,  20,  15,   5,  -5,
    -5,   5,  15,  20,  20,  15,   5,  -5,
   -20, -10,   0,   5,   5,   0, -10, -20,
  -135, -25, -15, -10, -10, -15, -25, -135
};

const int pstKnightEg[64] =
{
   -40, -30, -20, -15, -15, -20, -30, -40,
   -30, -20, -10,  -5,  -5, -10, -20, -30,
   -20, -10,   0,   5,   5,   0, -10, -20,
   -15,  -5,   5,  10,  10,   5,  -5, -15,
   -15,  -5,   5,  10,  10,   5,  -5, -15,
   -20, -10,   0,   5,   5,   0, -10, -20,
   -30, -20, -10,  -5,  -5, -10, -20, -30,
   -40, -30, -20, -15, -15, -20, -30, -40
};

const int pstBishopMg[64] = 
{
    -2,  -5, -16,  -5,  -5, -16,  -5,  -2,
    -5,   6,   3,   6,   6,   3,   6,  -5,
    -5,   5,   8,  10,  10,   8,   5,  -5,
    -5,   0,  10,  13,  13,  10,   0,  -5,
    -5,   0,   5,  13,  13,   5,   0,  -5,
    -5,   0,   5,   5,   5,   5,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5
};

const int pstBishopEg[64] = 
{
   -15, -10,  -8,  -5,  -5,  -8, -10, -15,
   -10,  -8,   0,   5,   5,   0,  -8, -10,
    -5,   0,   5,  10,  10,   5,   0,  -8,
    -5,   5,  10,  15,  15,  10,   5,  -5,
    -5,   5,  10,  15,  15,  10,   5,  -5,
    -8,   0,   5,  10,  10,   5,   0,  -8,
   -10,  -8,   0,   5,   5,   0,  -8, -10,
   -15, -10,  -8,  -5,  -5,  -8, -10, -15
};

const int pstRookMg[64] = 
{
  	 0,   1,   2,   4,   4,   2,   1,   0,
    -4,  -2,   0,   2,   2,   0,  -2,  -4,
    -4,  -2,   0,   2,   2,   0,  -2,  -4,
    -4,  -2,   0,   2,   2,   0,  -2,  -4,
    -4,  -2,   0,   2,   2,   0,  -2,  -4,
    -4,  -2,   0,   2,   2,   0,  -2,  -4,
    15,  15,  15,  15,  15,  15,  15,  15,
     4,   4,   4,   4,   4,   4,   4,   4
};

const int pstRookEg[64] = 
{
  	 0,   0,   0,   0,   0,   0,   0,   0,
    -2,   0,   0,   0,   0,   0,   0,  -2,
    -2,   0,   0,   0,   0,   0,   0,  -2,
    -2,   0,   0,   0,   0,   0,   0,  -2,
    -2,   0,   0,   0,   0,   0,   0,  -2,
    -2,   0,   0,   0,   0,   0,   0,  -2,
    15,  15,  15,  15,  15,  15,  15,  15,
     4,   4,   4,   4,   4,   4,   4,   4
};

const int pstQueenMg[64] = 
{
	-5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

const int pstQueenEg[64] = 
{
   -24, -16, -12,  -8,  -8, -12, -16, -24,
   -16, -12,  -4,   0,   0,  -4, -12, -16,
   -12,  -4,   0,   4,   4,   0,  -4, -12,
    -8,   0,   4,   8,   8,   4,   0,  -8,
    -8,   0,   4,   8,   8,   4,   0,  -8,
   -12,  -4,   0,   4,   4,   0,  -4, -12,
   -16, -12,  -4,   0,   0,  -4, -12, -16,
   -24, -16, -12,  -8,  -8, -12, -16, -24
};

const int pstKingMg[64] = 
{
    40,  50,  30,  10,  10,  30,  50,  40,
    30,  40,  20,   0,   0,  20,  40,  30,
    10,  20,   0, -20, -20,   0,  20,  10,
     0,  10, -10, -30, -30, -10,  10,   0,
   -10,   0, -20, -40, -40, -20,   0, -10,
   -20, -10, -30, -50, -50, -30, -10, -20,
   -30, -20, -40, -60, -60, -40, -20, -30,
   -40, -30, -50, -70, -70, -50, -30, -40
};

const int pstKingEg[64] = 
{
   -72, -48, -36, -24, -24, -36, -48, -72,
   -48, -24, -12,   0,   0, -12, -24, -48,
   -36, -12,   0,  12,  12,   0, -12, -36,
   -24,   0,  12,  24,  24,  12,   0, -24,
   -24,   0,  12,  24,  24,  12,   0, -24,
   -36, -12,   0,  12,  12,   0, -12, -36,
   -48, -24, -12,   0,   0, -12, -24, -48,
   -72, -48, -36, -24, -24, -36, -48, -72
};