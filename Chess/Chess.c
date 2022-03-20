#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U64 unsigned long long

// Operations on U64 bitboard by square
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) (((bitboard) >> (square)) & 1)
#define toggle_bit(bitboard, square) ((bitboard) ^= (1ULL << (square)))
#define unset_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

//#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)


// Constants:

#define start_fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define empty_fen "8/8/8/8/8/8/8/8 w KQkq - 0 1"
#define fen1 "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define fen2 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define castle_fen1 "8/4k3/8/8/8/8/P6P/R3K2R w KQ - 0 1" // can castle both sides
#define castle_fen2 "8/8/8/8/3k4/8/rp6/R3K3 w Q - 0 1" // cant castle queenside due to pawn
#define castle_fen3 "8/8/4k3/8/8/pr6/8/R3K2R w KQ - 0 1" // Can casle despite pawn attacking b1
#define promo_fen "1k6/5P2/8/8/8/8/8/4K3 w - - 20 1"

U64 bishop_magic_numbers[64] = {
    0x40040822862081ULL,
    0x14310404009000ULL,
    0x1004015401000208ULL,
    0x4043080023200ULL,
    0x8102021081055120ULL,
    0xa06089208000201ULL,
    0x202010120100002ULL,
    0xa04a403180800ULL,
    0x100081004880040ULL,
    0x481144049200ULL,
    0x408008408902eULL,
    0x281a09200001ULL,
    0xc005084840002000ULL,
    0x20880440210bULL,
    0x8000220104024110ULL,
    0x881688041082100ULL,
    0x484800100210ec05ULL,
    0xa20a01012488500ULL,
    0x44000820440008ULL,
    0x7000820420000ULL,
    0x801002820082035ULL,
    0xd2003700420208ULL,
    0x444c400206422020ULL,
    0x184800a00840140ULL,
    0x49008c0502001c1ULL,
    0x44100002226800ULL,
    0x804280024880100ULL,
    0x120802028020220ULL,
    0x8010088004002080ULL,
    0x88002002020100ULL,
    0x4040052841a0200ULL,
    0x40008200008200daULL,
    0x890100802048800ULL,
    0x80148d02c0840ULL,
    0xc2010100100044ULL,
    0x20020080a80080ULL,
    0x40080200004104ULL,
    0x2120a00024818ULL,
    0x8924094a00214814ULL,
    0x1052100320048ULL,
    0x4410828840006002ULL,
    0xa002180104080840ULL,
    0x12042201048800ULL,
    0x4400410280824801ULL,
    0x4008205200841410ULL,
    0x10200b1009080080ULL,
    0xe020022401101444ULL,
    0x10008100506d04ULL,
    0x24880807304702ULL,
    0x8a084128080380ULL,
    0x8008424980000ULL,
    0x4048008042022000ULL,
    0x1000012002048001ULL,
    0x401080810042006ULL,
    0x108086800840508ULL,
    0x1802840400861282ULL,
    0x6000110801500802ULL,
    0x103084200900822ULL,
    0xf01008042009000ULL,
    0x600800a300411088ULL,
    0x1021010a40104114ULL,
    0x3800204004884081ULL,
    0x5020e00401220414ULL,
    0x20248400440020ULL
};

U64 rook_magic_numbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x1401800c00060080ULL,
    0x203000200040100ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x804040008024110ULL,
    0x24020000440081ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x2004200108024ULL,
    0x61022100100208ULL,
    0x28000880800400ULL,
    0x20080040080ULL,
    0x20410400301218ULL,
    0x2110a00006084ULL,
    0x804010800020ULL,
    0x8200201002400040ULL,
    0x20842008801001ULL,
    0x280400812002200ULL,
    0x400110005000800ULL,
    0x40a008010410ULL,
    0x1201240018b0ULL,
    0x2202102a6000844ULL,
    0x4000400026948000ULL,
    0x240100800202002ULL,
    0x5010004020010100ULL,
    0x10000800108080ULL,
    0x1000801110004ULL,
    0xd223040002008080ULL,
    0x4150020810840001ULL,
    0x1000024109820034ULL,
    0x40002040801080ULL,
    0x81201000400140ULL,
    0x10002000881080ULL,
    0x401090010002500ULL,
    0x1408000901841100ULL,
    0x800800400020080ULL,
    0x3000011042084400ULL,
    0x2001000291420900ULL,
    0x4141005025408001ULL,
    0x45912940010081ULL,
    0x508426000085103ULL,
    0x2011000822049001ULL,
    0x1002408001023ULL,
    0x101000c00460875ULL,
    0xa0001021020800a4ULL,
    0x80000910c4008026ULL
};


// Relevant piece occupancy bits
const int bishop_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

enum {
    wK, wQ, wR, wB, wN, wP,
    bK, bQ, bR, bB, bN, bP
};

char piece_chars[12] = "KQRBNPkqrbnp";


const char* piece_strings[] = {
    "White King", "White Queen", "White Rook", "White Bishop", "White Knight", "White Pawn",
    "Black King", "Black Queen", "Black Rook", "Black Bishop", "Black Knight", "Black Pawn"
};


int piece_from_char[] = {
    ['K'] = wK,
    ['Q'] = wQ,
    ['R'] = wR,
    ['B'] = wB,
    ['N'] = wN,
    ['P'] = wP,
    ['k'] = bK,
    ['q'] = bQ,
    ['r'] = bR,
    ['b'] = bB,
    ['n'] = bN,
    ['p'] = bP,
};



enum { // Castling rights
    K = 1, Q = 2, k = 4, q = 8
};

// Enumerate bits from square names -- a8: 0, b8: 1, a7: 8, ...
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

const char* coord[] = { // Array of coords
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// Enumarate colour index from name -- white: 0, black: 1, both: 2
enum {
    white, black, both
};

#define not_side(side) (side ? white : black)

enum { // Rook = 0, Bishop = 1
    rook, bishop
};

// U64 with all bits apart from a and h file on
const U64 not_a = (U64)0xFEFEFEFEFEFEFEFE;
const U64 not_h = (U64)0x7F7F7F7F7F7F7F7F;
const U64 not_ab = (U64)0xFCFCFCFCFCFCFCFC;
const U64 not_gh = (U64)0x3F3F3F3F3F3F3F3F;

// Square tables from CPW - All tables are for white but can be reversed easily

int pawn_square_value[64] = {
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

int knight_square_value[64] = { // Encourages knifht moves to centre of board
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

int bishop_square_value[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

int rook_square_value[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};

int queen_square_value[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

int king_square_early_value[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};

int king_square_late_value[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

// Count bits in a bitboard

static inline int count_bits(U64 bitboard) {
    int count;
    for (count = 0; bitboard; count++) {
        bitboard &= (bitboard - 1);
    }
    return count;
}

static inline U64 lsb(U64 b) {
    return (b & (~b + 1));
}

// Get the index of the least significant first bit

static inline int lsb_index(U64 bitboard) {
    if (!bitboard) {
        return -1;
    }
    return (count_bits((bitboard & (~bitboard + 1)) - 1));
}

void print_bitboard(U64 bitboard) {
    printf("\n    a  b  c  d  e  f  g  h\n"); // Print files
    for (int row = 0; row < 8; row++) {
        printf(" %d ", 8 - row); // Print rank
        for (int col = 0; col < 8; col++) {
            int square = row * 8 + col; // Get integer value of square
            printf(" %d ", (int)get_bit(bitboard, square));
        }
        printf("\n");
    }
    printf("Long Long Unsigned Decimal: %llud\n\n", bitboard);
}

U64 set_occupancy(int index, int relevant_bits, U64 attack_board) {
    U64 occupancy = 0ULL;
    for (int count = 0; count < relevant_bits; count++) {
        int square = lsb_index(attack_board);
        unset_bit(attack_board, square);
        if (index & (1 << count)) {
            set_bit(occupancy, square);
        }
    }
    return occupancy;
}

#define get_colour(piece) ((int)(piece/6))

// Magic Number Generator
/*
unsigned int state = 1804289383;

unsigned int get_random_U32() {
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    state = number;
    return number;
}

U64 get_random() {
    U64 n1, n2, n3, n4;
    n1 = (U64)(get_random_U32() & 0xFFFF);
    n2 = (U64)(get_random_U32() & 0xFFFF);
    n3 = (U64)(get_random_U32() & 0xFFFF);
    n4 = (U64)(get_random_U32() & 0xFFFF);
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 get_magic() {
    return get_random() & get_random() & get_random();
}

U64 find_magic(int square, int bishop_flag) {
    U64 attacks[4096];
    U64 used[4096];
    U64 occupancies[4096];
    U64 attack_board = bishop_flag ? generate_bishop_magic(square) : generate_rook_magic(square);
    int relevant_bits = bishop_flag ? bishop_bits[square] : rook_bits[square];
    int occupancy_indices = 1 << relevant_bits;
    for (int index = 0; index < occupancy_indices; index++) {
        occupancies[index] = set_occupancy(index, relevant_bits, attack_board);
        attacks[index] = bishop_flag ? get_bishop_attacks(square, occupancies[index]) : get_rook_attacks(square, occupancies[index]);
    }
    for (int randoms = 0; randoms < 10000000; randoms++) {
        U64 magic = get_magic();
        if (count_bits((attack_board * magic) & 0xFF00000000000000) < 6) continue;
        memset(used, 0ULL, sizeof(used));
        int index, fail;
        for (index = 0, fail = 0; !fail && index < occupancy_indices; index++) {
            int magic_index = (int)((occupancies[index] * magic) >> (64 - relevant_bits));
            if (!used[magic_index]) {
                used[magic_index] = attacks[index];
            }
            else if (used[magic_index] != attacks[index]) {
                fail = 1;
            }
        }
        if (!fail) {
            return magic;
        }
    }
    return 0ULL;
}

void init_magic() {
    for (int square = 0; square < 64; square++) {
        bishop_magic_numbers[square] = find_magic(square, bishop);
        rook_magic_numbers[square] = find_magic(square, rook);
    }
}

*/

U64 get_bishop_attacks(int square, U64 blockers) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(bitboard, square);

    int row = square / 8;
    int col = square % 8;

    int r, c;
    for (r = row + 1, c = col + 1; r <= 7 && c <= 7; r++, c++) {
        set_bit(attacks, r * 8 + c);
        if (get_bit(blockers, r * 8 + c)) break;
    }
    for (r = row - 1, c = col + 1; r >= 0 && c <= 7; r--, c++) {
        set_bit(attacks, r * 8 + c);
        if (get_bit(blockers, r * 8 + c)) break;
    }
    for (r = row + 1, c = col - 1; r <= 7 && c >= 0; r++, c--) {
        set_bit(attacks, r * 8 + c);
        if (get_bit(blockers, r * 8 + c)) break;
    }
    for (r = row - 1, c = col - 1; r >= 0 && c >= 0; r--, c--) {
        set_bit(attacks, r * 8 + c);
        if(get_bit(blockers, r * 8 + c)) break;
    }

    return attacks;
}

U64 get_rook_attacks(int square, U64 blockers) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(bitboard, square);

    int row = square / 8;
    int col = square % 8;

    int r, c;
    for (r = row + 1; r <= 7; r++) {
        set_bit(attacks, r * 8 + col);
        if (get_bit(blockers, r * 8 + col)) break;
    }
    for (r = row - 1; r >= 0; r--) {
        set_bit(attacks, r * 8 + col);
        if (get_bit(blockers, r * 8 + col)) break;
    }
    for (c = col + 1; c <= 7; c++) {
        set_bit(attacks, row * 8 + c);
        if (get_bit(blockers, row * 8 + c)) break;
    }
    for (c = col - 1; c >= 0; c--) {
        set_bit(attacks, row * 8 + c);
        if (get_bit(blockers, row * 8 + c)) break;
    }

    return attacks;
}

// All attacks calculated once, then looked up
U64 generate_pawn_attacks(int square, int side) {
    U64 attacks = 0ULL; // Attacks bitboard
    U64 bitboard = 0ULL; // Piece bitboard
    set_bit(bitboard, square);

    if (!side) { // White
        if (bitboard & not_a) { // Checks to stop pawns from attacking across the board
            set_bit(attacks, square - 9);
        }
        if (bitboard & not_h) {
            set_bit(attacks, square - 7);
        }
    }
    else { // Black
        if (bitboard & not_a) {
            set_bit(attacks, square + 7);
        }
        if (bitboard & not_h) {
            set_bit(attacks, square + 9);
        }
    }
    return attacks; 
}

U64 generate_knight_attacks(int square) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    set_bit(bitboard, square);
    if (bitboard & not_ab) {
        set_bit(attacks, square - 10);
        set_bit(attacks, square + 6);
    }
    if (bitboard & not_a) {
        set_bit(attacks, square - 17);
        set_bit(attacks, square + 15);
    }
    if (bitboard & not_gh) {
        set_bit(attacks, square - 6);
        set_bit(attacks, square + 10);
    }
    if (bitboard & not_h) {
        set_bit(attacks, square - 15);
        set_bit(attacks, square + 17);
    }
    return attacks;
}

U64 generate_king_attacks(int square) {
    U64 attacks = 0ULL;
    U64 bitboard = 0uLL;
    set_bit(bitboard, square);
    
    if (bitboard & not_a) {
        set_bit(attacks, square - 9);
        set_bit(attacks, square - 1);
        set_bit(attacks, square + 7);
    }
    if (bitboard & not_h) {
        set_bit(attacks, square + 9);
        set_bit(attacks, square + 1);
        set_bit(attacks, square - 7);
    }
    set_bit(attacks, square + 8);
    set_bit(attacks, square - 8);
    return attacks;
}

U64 generate_bishop_magic(int square) {
    U64 attacks = 0ULL;

    int row = square / 8;
    int col = square % 8;

    int r, c;
    for (r = row + 1, c = col + 1; r < 7 && c < 7; r++, c++) {
        set_bit(attacks, r * 8 + c);
    }
    for (r = row - 1, c = col + 1; r > 0 && c < 7; r--, c++) {
        set_bit(attacks, r * 8 + c);
    }
    for (r = row + 1, c = col - 1; r < 7 && c > 0; r++, c--) {
        set_bit(attacks, r * 8 + c);
    }
    for (r = row - 1, c = col - 1; r > 0 && c > 0; r--, c--) {
        set_bit(attacks, r * 8 + c);
    }

    return attacks;
}

U64 generate_rook_magic(int square) {
    U64 attacks = 0ULL;

    int row = square / 8;
    int col = square % 8;

    int r, c;
    for (r = row + 1; r < 7; r++) {
        set_bit(attacks, r * 8 + col);
    }
    for (r = row - 1; r > 0; r--) {
        set_bit(attacks, r * 8 + col);
    }
    for (c = col + 1; c < 7; c++) {
        set_bit(attacks, row * 8 + c);
    }
    for (c = col - 1; c > 0; c--) {
        set_bit(attacks, row * 8 + c);
    }

    return attacks;
}

// Ray piece attacks must be calculated for various occupancy bitboards. White and black pieces are not distinguished by these functions, but by the move generator.

U64 bishop_attacks[64][512], rook_attacks[64][4096];
U64 bishop_boards[64], rook_boards[64];

static inline U64 generate_bishop_attacks(int square, U64 occupancy) {
    occupancy &= bishop_boards[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= (64 - bishop_bits[square]);
    return bishop_attacks[square][occupancy];
}

static inline U64 generate_rook_attacks(int square, U64 occupancy) {
    occupancy &= rook_boards[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_bits[square];
    return rook_attacks[square][occupancy];
}

static inline U64 generate_queen_attacks(int square, U64 occupancy) {
    return generate_bishop_attacks(square, occupancy) | generate_rook_attacks(square, occupancy);
}

static inline U64 nortOne(U64 b) {
    return b >> 8;
}
static inline U64 soutOne(U64 b) {
    return b << 8;
}

// Pawn attacks: pawn_attaks[square][side] 2-dimensional 64x2 array of attack bitboards
U64 pawn_attacks[64][2];
// Other attacks are same for white and black, only 1D array
U64 knight_attacks[64];
U64 king_attacks[64];

void init_pawn_attacks() { // Initialise array
    for (int square = 0; square < 64; square++) {
        pawn_attacks[square][white] = generate_pawn_attacks(square, white);
        pawn_attacks[square][black] = generate_pawn_attacks(square, black);
    }
}

void init_knight_attacks() {
    for (int square = 0; square < 64; square++) {
        knight_attacks[square] = generate_knight_attacks(square);
    }
}

void init_king_attacks() {
    for (int square = 0; square < 64; square++) {
        king_attacks[square] = generate_king_attacks(square);
    }
}

void init_ray_attacks() {
    for (int square = 0; square < 64; square++) {
        U64 bishop_attack = generate_bishop_magic(square);
        U64 rook_attack = generate_rook_magic(square);
        bishop_boards[square] = bishop_attack;
        rook_boards[square] = rook_attack;
        int bishopbits = bishop_bits[square];
        int rookbits = rook_bits[square];
        int bishop_occupancies = 1 << bishopbits;
        int rook_occupancies = 1 << rookbits;
        for (int index = 0; index < bishop_occupancies; index++) {
            U64 occupancy = set_occupancy(index, bishopbits, bishop_attack);
            int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishopbits);
            bishop_attacks[square][magic_index] = get_bishop_attacks(square, occupancy);
        }
        for (int index = 0; index < rook_occupancies; index++) {
            U64 occupancy = set_occupancy(index, rookbits, rook_attack);
            int magic_index = (occupancy * rook_magic_numbers[square] >> (64 - rookbits));
            rook_attacks[square][magic_index] = get_rook_attacks(square, occupancy);
        }
    }
}

// Gamestate variables

int castle_rights;
int ep_target;
int side;
U64 bitboards[12]; // Piece bitboards
U64 occupancies[3]; // White, Black, Both

#define save_state() \
U64 c_bitboards[12], c_occupancies[3]; \
int c_side, c_castle_rights, c_ep_target; \
memcpy(c_bitboards, bitboards, 96); \
memcpy(c_occupancies, occupancies, 24); \
c_side = side; c_castle_rights = castle_rights; c_ep_target = ep_target; \

#define load_state() \
memcpy(bitboards, c_bitboards, sizeof(c_bitboards)); \
memcpy(occupancies, c_occupancies, sizeof(c_occupancies)); \
side = c_side; castle_rights = c_castle_rights; c_ep_target = ep_target; \

static inline U64 get_attacks(int square, int piece) {
    U64 a;
    switch (piece % 6) {
    case 0:
        a = king_attacks[square];
        break;
    case 1:
        a = generate_queen_attacks(square, occupancies[both]);
        break;
    case 2:
        a = generate_rook_attacks(square, occupancies[both]);
        break;
    case 3:
        a = generate_bishop_attacks(square, occupancies[both]);
        break;
    case 4:
        a = knight_attacks[square];
        break;
    case 5:
        a = pawn_attacks[square][piece / 6];
        break;
    }
    return a;
}

void add_occup(int square, int colour) {
    set_bit(occupancies[both], square);
    if (!colour) set_bit(occupancies[white], square);
    else set_bit(occupancies[black], square);
}

void reset_occup(int square) {
    for (int i = 0; i < 3; i++) {
        unset_bit(occupancies[i], square);
    }
    for (int piece = wK; piece < bP; piece++) {
        unset_bit(bitboards[piece], square);
    }
}

void print_board() {
    printf("\n    a  b  c  d  e  f  g  h");
    for (int square = 0; square < 64; square++) {
        if (!(square % 8)) printf("\n %d ", 8 - (int)(square / 8));
        if (!get_bit(occupancies[both], square)) printf(" - ");
        else {
            for (int i = 0; i < 12; i++) {
                if (get_bit(bitboards[i], square)) {
                    printf(" %c ", piece_chars[i]);
                    break;
                }
            }
        }
    }
    printf("\n    Side to move: %s\n", !side ? "white" : "black");
    printf("    En passant target: ");
    if (ep_target != no_sq) printf(coord[ep_target]);
    printf("\n");
    printf("    Castling rights: %c%c%c%c\n", (castle_rights & K) ? 'K' : '-', (castle_rights & Q) ? 'Q' : '-',
                                              (castle_rights & k) ? 'k' : '-', (castle_rights & q) ? 'q' : '-');
}

void init_fen(char fen[]) {
    memset(bitboards, 0ULL, 96); // 8 bytes per board, 12 boards
    memset(occupancies, 0ULL, 24); // 3 boards
    int index = 0;
    int square = 0;
    char c;
    while (1) {
        c = fen[index];
        index++;
        if (c == '/') continue;
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            set_bit(bitboards[piece_from_char[c]], square);
            add_occup(square, get_colour(piece_from_char[c]));
            square++;
        }
        else {
            square += c - '0';
        }
        if (square >= 64) break;
    }
    index++;
    side = (fen[index] == 'w') ? 0 : 1;
    index += 2;
    castle_rights = 0;
    if (fen[index] != '-') {
        if (fen[index] == 'K') {
            castle_rights |= K;
            index++;
        }
        if (fen[index] == 'Q') {
            castle_rights |= Q;
            index++;
        }
        if (fen[index] == 'k') {
            castle_rights |= k;
            index++;
        }
        if (fen[index] == 'q') {
            castle_rights |= q;
            index++;
        }
    }
    else index++;
    index++;
    if (fen[index] == '-') {
        ep_target = no_sq;
        index += 2;
    }
    else {
        int col = fen[index] - 'a';
        int rank = fen[index + 1] - '1';
        int row = 7 - rank;
        ep_target = row * 8 + col;
        index += 3;
    }
}

// Piece moves/attacks calculatiosn

static inline int is_attacked(int square, int attacking_side) {
    if ((pawn_attacks[square][attacking_side ? white : black] & bitboards[attacking_side ? bP : wP])) return 1;
    if ((knight_attacks[square] & bitboards[attacking_side ? bN : wN])) return 1;
    if ((king_attacks[square] & bitboards[attacking_side ? bK : wK])) return 1;
    if ((generate_bishop_attacks(square, occupancies[both]) & (bitboards[attacking_side ? bB : wB] | bitboards[attacking_side ? bQ : wQ]))) return 1;
    if ((generate_rook_attacks(square, occupancies[both]) & (bitboards[attacking_side ? bR : wR] | bitboards[attacking_side ? bQ : wQ]))) return 1;
    return 0;
}

// Moves
struct packed_move {
    unsigned int start : 6;
    unsigned int end : 6;
    unsigned int piece : 4;
    unsigned int promotion : 4;
    unsigned int capture : 1;
    unsigned int castle : 1;
    unsigned int doublepush : 1;
    unsigned int enpassant : 1;
    unsigned int index : 24;
} pack;
typedef struct packed_move move;

move encode(int start, int end, int piece, int promotion, int capture, int castle, int doublepush, int enpassant) {
    move m;
    m.start = start;
    m.end = end;
    m.piece = piece;
    m.promotion = promotion;
    m.capture = capture;
    m.castle = castle;
    m.doublepush = doublepush;
    m.enpassant = enpassant;
    m.index = start | end << 6 | piece << 12 | promotion << 16 | capture << 20 | castle << 21 | doublepush << 22 | enpassant << 23;
    return m;
}

void print_move(move move) {
    printf("\n%s %s -> %s  %s  Flags = %d%d%d%d", piece_strings[move.piece], coord[move.start], coord[move.end],
        move.promotion ? piece_strings[move.promotion] : "",
        move.capture, move.castle, move.doublepush, move.enpassant);
}

void print_move_list(move move_list[]) {
    for (int i = 0; i < 128; i++) {
        move m = move_list[i];
        if (m.start == m.end) break;
        print_move(m);
    }
}

move move_list[128];
int count;
static inline void add_move(move move) {
    move_list[count] = move;
    count++;
}

U64 rank2 = (U64)0x00FF000000000000;
U64 rank4 = (U64)0x000000FF00000000;
U64 rank5 = (U64)0x00000000FF000000;
U64 rank7 = (U64)0x000000000000FF00;
U64 wKingCastlePath = (U64)0x6000000000000000;
U64 wQueenCastlePath = (U64)0x0E00000000000000;
U64 bKingCastlePath = (U64)0x0000000000000060;
U64 bQueenCastlePath = (U64)0x000000000000000E;

static inline void generate_moves() {
    count = 0;
    int start, end;
    U64 b, a; // Bitboard, attacks
    for (int piece = (side ? bK : wK); piece <= (side ? bP : wP); piece++) { // Iterate through all pieces of side to move
        b = bitboards[piece];
        if (!b) continue; // Continue if none of piece type exists
        if (piece == wP) { // Get pawn moves
            while (b) { // Quiet pawn moves
                U64 pawn_board = lsb(b);
                U64 start = count_bits(pawn_board - 1);
                if (nortOne(pawn_board) & ~occupancies[both]) {
                    if (pawn_board & rank7) { // Promotion
                        add_move(encode(start, start - 8, wP, wQ, 0, 0, 0, 0));
                        add_move(encode(start, start - 8, wP, wR, 0, 0, 0, 0));
                        add_move(encode(start, start - 8, wP, wB, 0, 0, 0, 0));
                        add_move(encode(start, start - 8, wP, wN, 0, 0, 0, 0));
                    }
                    else {
                        add_move(encode(start, start - 8, wP, 0, 0, 0, 0, 0));
                        if (nortOne(nortOne(pawn_board)) & ~occupancies[both] & rank4) {
                            add_move(encode(start, start - 16, wP, 0, 0, 0, 1, 0));
                        }
                    }
                }
                a = pawn_attacks[start][side];
                while (a) {
                    U64 end_board = lsb(a);
                    end = count_bits(end_board - 1);
                    if (end_board & occupancies[not_side(side)]) {
                        if (pawn_board & rank7) { // Promotion
                            add_move(encode(start, end, wP, wQ, 1, 0, 0, 0));
                            add_move(encode(start, end, wP, wR, 1, 0, 0, 0));
                            add_move(encode(start, end, wP, wB, 1, 0, 0, 0));
                            add_move(encode(start, end, wP, wN, 1, 0, 0, 0));
                        }
                        else {
                            add_move(encode(start, end, wP, 0, 1, 0, 0, 0));
                        }
                    }
                    if (end == ep_target) {
                        add_move(encode(start, end, wP, 0, 1, 0, 0, 1));
                    }
                    unset_bit(a, end);
                }
                b ^= pawn_board;
            }
        }
        else if (piece == bP) { // Get pawn moves
            while (b) { // Quiet pawn moves
                U64 pawn_board = lsb(b);
                U64 start = count_bits(pawn_board - 1);
                if (soutOne(pawn_board) & ~occupancies[both]) {
                    if (pawn_board & rank2) { // Promotion
                        add_move(encode(start, start + 8, bP, bQ, 0, 0, 0, 0));
                        add_move(encode(start, start + 8, bP, bR, 0, 0, 0, 0));
                        add_move(encode(start, start + 8, bP, bB, 0, 0, 0, 0));
                        add_move(encode(start, start + 8, bP, bN, 0, 0, 0, 0));
                    }
                    else {
                        add_move(encode(start, start + 8, bP, 0, 0, 0, 0, 0));
                        if (soutOne(soutOne(pawn_board)) & ~occupancies[both] & rank5) {
                            add_move(encode(start, start + 16, bP, 0, 0, 0, 1, 0));
                        }
                    }
                }
                a = pawn_attacks[start][side];
                while (a) {
                    U64 end_board = lsb(a);
                    end = count_bits(end_board - 1);
                    if (end_board & occupancies[not_side(side)]) {
                        if (pawn_board & rank2) { // Promotion
                            add_move(encode(start, end, bP, bQ, 1, 0, 0, 0));
                            add_move(encode(start, end, bP, bR, 1, 0, 0, 0));
                            add_move(encode(start, end, bP, bB, 1, 0, 0, 0));
                            add_move(encode(start, end, bP, bN, 1, 0, 0, 0));
                        }
                        else {
                            add_move(encode(start, end, bP, 0, 1, 0, 0, 0));
                        }
                    }
                    if (end == ep_target) {
                        add_move(encode(start, end, bP, 0, 1, 0, 0, 1));
                    }
                    unset_bit(a, end);
                }
                b ^= pawn_board;
            }
        }
        else {
            while (b) { // Get attack moves (i.e. not quiet pawn moves, castling, 
                start = lsb_index(b);
                switch (piece % 6) {
                case 0:
                    a = king_attacks[start];
                    break;
                case 1:
                    a = generate_queen_attacks(start, occupancies[both]);
                    break;
                case 2:
                    a = generate_rook_attacks(start, occupancies[both]);
                    break;
                case 3:
                    a = generate_bishop_attacks(start, occupancies[both]);
                    break;
                case 4:
                    a = knight_attacks[start];
                    break;
                }
                a &= ~(occupancies[side]); //Restrict friendly fire 
                while (a) {
                    end = lsb_index(a);
                    add_move(encode(start, end, piece, 0, get_bit(occupancies[both], end), 0, 0, 0));
                    unset_bit(a, end);
                }
                unset_bit(b, start);
            }
        }
        if (piece == wK && !is_attacked(e1, black)) {
            if (castle_rights & K) {
                if (wKingCastlePath & ~occupancies[both]) { // Check if squares are free in path
                    if (!is_attacked(f1, black) && !is_attacked(g1, black)) {
                        add_move(encode(e1, g1, wK, 0, 0, 1, 0, 0));
                    }
                }
            }
            if (castle_rights & Q) {
                if (wQueenCastlePath & ~occupancies[both]) { // Check if squares are free in path
                    if (!is_attacked(c1, black) && !is_attacked(d1, black)) {
                        add_move(encode(e1, c1, wK, 0, 0, 1, 0, 0));
                    }
                }
            }
        }
        if (piece == bK && !is_attacked(e8, white)) {
            if (castle_rights & k) {
                if (bKingCastlePath & ~occupancies[both]) { // Check if squares are free in path
                    if (!is_attacked(f8, white) && !is_attacked(g8, white)) {
                        add_move(encode(e8, g8, bK, 0, 0, 1, 0, 0));
                    }
                }
            }
            if (castle_rights & q) {
                if (bQueenCastlePath & ~occupancies[both]) { // Check if squares are free in path
                    if (!is_attacked(c8, white) && !is_attacked(d8, white)) {
                        add_move(encode(e8, c8, bK, 0, 0, 1, 0, 0));
                    }
                }
            }
        }
    }
}

enum {
    all_moves, capture_moves
};

static inline int make_move(move m, int capture_flag) {
    if (!capture_flag) {
        reset_occup(m.start);
        reset_occup(m.end);
        if (m.piece == wK) castle_rights &= (k | q); // castling stuff
        if (m.piece == bK) castle_rights &= (K | Q);
        if (m.start == h1 || m.end == h1) castle_rights &= (Q | k | q);
        if (m.start == a1 || m.end == a1) castle_rights &= (K | k | q);
        if (m.start == h8 || m.end == h8) castle_rights &= (K | Q | q);
        if (m.start == a8 || m.end == a8) castle_rights &= (K | Q | k);
        if (m.enpassant) {
            if (side == white) reset_occup(m.end + 8);
            else reset_occup(m.end - 8);
        }
        if (m.doublepush) ep_target = (m.end + m.start) / 2;
        else ep_target = no_sq;
        unset_bit(bitboards[m.piece], m.start);
        if (!m.promotion) set_bit(bitboards[m.piece], m.end);
        else {
            set_bit(bitboards[m.promotion], m.end);
        }
        add_occup(m.end, get_colour(m.piece));
        if (m.castle) {
            if (m.end == g1) { // White kingside 
                unset_bit(bitboards[wR], h1);
                unset_bit(occupancies[both], h1);
                unset_bit(occupancies[white], h1);
                set_bit(bitboards[wR], f1);
                add_occup(f1, white);
            }
            else if (m.end == c1) { // White queenside 
                unset_bit(bitboards[wR], a1);
                unset_bit(occupancies[both], a1);
                unset_bit(occupancies[white], a1);
                set_bit(bitboards[wR], d1);
                add_occup(d1, white);
            }
            if (m.end == g8) { // Black kingside 
                unset_bit(bitboards[bR], h8);
                unset_bit(occupancies[both], h8);
                unset_bit(occupancies[black], h8);
                set_bit(bitboards[bR], f8);
                add_occup(f8, black);
            }
            else if (m.end == c8) { // Black queenside
                unset_bit(bitboards[bR], a8);
                unset_bit(occupancies[both], a8);
                unset_bit(occupancies[black], a8);
                set_bit(bitboards[bR], d8);
                add_occup(d8, black);
            }
        }
        side = not_side(side);
    }
    else {
        if (m.capture) {
            make_move(m, all_moves);
        }
        else return 0;
    }
}

move create_move(char str[]) {
    move invalid = encode(0, 0, 0, 0, 0, 0, 0, 0);
    int len = strlen(str);
    if (str[0] == 'O' || str[0] == '0') { //Castling
        if (len < 5) { //Kingside (O-O, O-O+)
            return encode(side ? e8 : e1, side ? g8 : g1, 6 * side, 0, 0, 1, 0, 0);
        }
        else { // Queenside (O-O-O, O-O-O+)
            return encode(side ? e8 : e1, side ? c8 : c1, 6 * side, 0, 0, 1, 0, 0);
        }
    }
    else if ('A' <= str[0] && str[0] <= 'Z') { // Piece moves
        int piece = piece_from_char[str[0]] + side * 6;
        int cap = 0;
        if (str[1] == 'x') cap = 1;
        if ((str[len - 1] == '+') || (str[len - 1] == '#')) len--;
        int end = 8 * (8 - (str[len-1] - '0')) + str[len-2] - 'a';
        if (!(0 <= end <= 63)) return invalid;
        U64 b = get_attacks(end, piece) & bitboards[piece];
        int piece_squares[10];
        int i = 0;
        U64 c = b;
        while (c) {
            piece_squares[i] = lsb_index(c);
            unset_bit(c, piece_squares[i]);
            i++;
        }
        if (!i) {
            return invalid;
        }
        if (i == 1) {
            return encode(piece_squares[0], end, piece, 0, cap, 0, 0, 0);
        }
        int disam = len - cap - 3;
        if (!disam) return invalid;
        char c1 = str[1 + cap];
        if ('a' <= c1 <= 'h') {
            b &= ((U64)0x0101010101010101 << (c1 - 'a'));
        }
        char c2 = (disam == 2) ? str[2 + cap] : str[1 + cap];
        if ('1' <= c2 <= '8') {
            b &= ((U64)0xFF << ((8 - c2 - '0') * 8));
        }
        int restricted_squares[10];
        int j = 0;
        while (b) {
            restricted_squares[j] = lsb_index(b);
            unset_bit(b, restricted_squares[j]);
            j++;
        }
        if (j == 1) {
            return encode(restricted_squares[j], end, piece, 0, cap, 0, 0, 0);
        }
        else return invalid;
    }
    else { // Pawn 
        int cap = 0;
        if (str[1] == 'x') cap = 1;
        if (str[len - 1] == '+' || str[len - 1] == "#") len--;
        int promo = 0;
        if (str[len - 2] == '=') {
            promo = piece_from_char[str[len - 1]] + side * 6;
            len -= 2;
        }
        int end = 8 * (8 - (str[len - 1] - '0')) + str[len - 2] - 'a';
        if (!(0 <= end <= 63)) return invalid;
        if (cap) {
            U64 b = pawn_attacks[end][not_side(side)];
            char c1 = str[0];
            b &= ((U64)0x0101010101010101 << (c1 - 'a'));
            int start = lsb_index(b);
            int ep = get_bit(occupancies[both], end) ? 0 : 1;
            return encode(start, end, wP + side * 6, promo, 1, 0, 0, ep);
        }
        else {
            U64 b = 0ULL;
            set_bit(b, end);
            if (!side) { // White quiet move
                if (soutOne(b) & bitboards[wP]) return encode(end + 8, end, wP, promo, 0, 0, 0, 0);
                else if (b & rank4) return encode(end + 16, end, wP, 0, 0, 0, 1, 0);
            }
            else { // Black quiet move
                if (nortOne(b) & bitboards[bP]) return encode(end - 8, end, bP, promo, 0, 0, 0, 0);
                else if (b & rank5) return encode(end - 16, end, bP, 0, 0, 0, 1, 0);
            }
        }
    }
}

// Evaluation

int piece_material_value[12] = {
    10000, 900, 500, 300, 300, 100, -10000, -900, -500, -300, -300, -100
};

int piece_square_value[64][14]; // 12 = wKLate 13 = bKLate

void init_piece_square_values() {
    for (int square = 0; square < 64; square++) {
        for (int piece = wK; piece <= bP; piece++) {
            int col = get_colour(piece);
            switch (piece % 6) {
            case 0:
                piece_square_value[square][piece] = col ? -king_square_early_value[63 - square] : king_square_early_value[square];
                break;
            case 1:
                piece_square_value[square][piece] = col ? -queen_square_value[63 - square] : queen_square_value[square];
                break;
            case 2:
                piece_square_value[square][piece] = col ? -rook_square_value[63 - square] : rook_square_value[square];
                break;
            case 3:
                piece_square_value[square][piece] = col ? -bishop_square_value[63 - square] : bishop_square_value[square];
                break;
            case 4:
                piece_square_value[square][piece] = col ? -knight_square_value[63 - square] : knight_square_value[square];
                break;
            case 5:
                piece_square_value[square][piece] = col ? -pawn_square_value[63 - square] : pawn_square_value[square];
                break;
            }
        }
        piece_square_value[square][12] = king_square_late_value[square];
        piece_square_value[square][13] = -king_square_late_value[63 - square];
    }
}

static inline int evaluate() {
    int score = 0;
    U64 b;
    for (int piece = wK; piece <= bP; piece++) {
        b = bitboards[piece];
        while (b) {
            int sq = lsb_index(b);
            score += piece_material_value[piece];
            score += piece_square_value[sq][piece];
            unset_bit(b, sq);
        }
    }
    return side ? -score : score; // Return positive for winning position, no matter the side
}

int string_loop() {
    init_fen(start_fen);
    while (1) {
        print_board();
        char move_str[7];
        gets(move_str);
        if (move_str[0] == 'q') return 0;
        generate_moves();
        move m = create_move(move_str);
        if (!m.index) {
            printf("Invalid move!\n");
            continue;
        }
        print_move(m);
        unsigned int index = m.index;
        int is_legal = 0;
        for (int i = 0; i < count; i++) {
            if (move_list[i].index == index) { is_legal = 1; break; }
        }
        if (is_legal) make_move(m, all_moves);
        else printf("Invalid move!\n");
    }
}

void init_all() {
    init_king_attacks();
    init_knight_attacks();
    init_pawn_attacks();
    init_ray_attacks();
    init_piece_square_values();
}

int main() {
    init_all();
    init_fen(start_fen);
    printf("%d\n", evaluate());
    return 0;
}
