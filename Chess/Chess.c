#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U64 unsigned long long

// Operations on U64 bitboard by square
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

#define start_fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define empty_fen "8/8/8/8/8/8/8/8 w KQkq - 0 1"

// Constants:

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

int char_pieces[] = {
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

enum { // Rook = 0, Bishop = 1
    rook, bishop
};

// U64 with all bits apart from a and h file on
const U64 not_a = 18374403900871474942ULL;
const U64 not_h = 9187201950435737471ULL;
const U64 not_ab = 18229723555195321596ULL;
const U64 not_gh = 4557430888798830399ULL;


// Count bits in a bitboard

static inline int count_bits(U64 bitboard) {
    int count;
    for (count = 0; bitboard; count++) {
        bitboard &= (bitboard - 1);
    }
    return count;
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
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("Long Long Unsigned Decimal: %llud\n\n", bitboard);
}

U64 set_occupancy(int index, int relevant_bits, U64 attack_board) {
    U64 occupancy = 0ULL;
    for (int count = 0; count < relevant_bits; count++) {
        int square = lsb_index(attack_board);
        pop_bit(attack_board, square);
        if (index & (1 << count)) {
            set_bit(occupancy, square);
        }
    }
    return occupancy;
}

int get_colour(int piece) {
    return (piece < 6) ? 0 : 1;
}

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
        if ((1ULL << r * 8 + c) & blockers) break;
    }
    for (r = row - 1, c = col + 1; r >= 0 && c <= 7; r--, c++) {
        set_bit(attacks, r * 8 + c);
        if ((1ULL << r * 8 + c) & blockers) break;
    }
    for (r = row + 1, c = col - 1; r <= 7 && c >= 0; r++, c--) {
        set_bit(attacks, r * 8 + c);
        if ((1ULL << r * 8 + c) & blockers) break;
    }
    for (r = row - 1, c = col - 1; r >= 0 && c >= 0; r--, c--) {
        set_bit(attacks, r * 8 + c);
        if ((1ULL << r * 8 + c) & blockers) break;
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
        U64 attack = 1ULL << r * 8 + col;
        attacks |= attack;
        if (attack & blockers) break;
    }
    for (r = row - 1; r >= 0; r--) {
        U64 attack = 1ULL << r * 8 + col;
        attacks |= attack;
        if (attack & blockers) break;
    }
    for (c = col + 1; c <= 7; c++) {
        U64 attack = 1ULL << row * 8 + c;
        attacks |= attack;
        if (attack & blockers) break;
    }
    for (c = col - 1; c >= 0; c--) {
        U64 attack = 1ULL << row * 8 + c;
        attacks |= attack;
        if (attack & blockers) break;
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
    occupancy >>= 64 - bishop_bits[square];
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

int castle;
int ep_target;
int side;
int draw_clock;
int move_number;
U64 bitboards[12]; // Piece bitboards
U64 occupancies[3]; // White, Black, Both

void add_occup(int square, int colour) {
    set_bit(occupancies[both], square);
    if (!colour) set_bit(occupancies[white], square);
    else set_bit(occupancies[black], square);
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
    printf("    Castling rights: %c%c%c%c\n", (castle & K) ? 'K' : '-', (castle & Q) ? 'Q' : '-',
                                              (castle & k) ? 'k' : '-', (castle & q) ? 'q' : '-');
}

void init_attacks() {
    init_king_attacks();
    init_knight_attacks();
    init_pawn_attacks();
    init_ray_attacks();
}

void init_fen(char fen[]) {
    memset(bitboards, 0ULL, 12);
    memset(occupancies, 0ULL, 3);
    int index = 0;
    int square = 0;
    char c;
    while (1) {
        c = fen[index];
        index++;
        if (c == '/') continue;
        if ('a' <= c && c <= 'z') {
            set_bit(bitboards[char_pieces[c]], square);
            add_occup(square, get_colour(char_pieces[c]));
            square++;
        }
        else if ('A' <= c && c <= 'Z') {
            set_bit(bitboards[char_pieces[c]], square);
            add_occup(square, get_colour(char_pieces[c]));
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
    castle = 0;
    if (fen[index] != '-') {
        if (fen[index] == 'K') {
            castle |= K;
            index++;
        }
        if (fen[index] == 'Q') {
            castle |= Q;
            index++;
        }
        if (fen[index] == 'k') {
            castle |= k;
            index++;
        }
        if (fen[index] == 'q') {
            castle |= q;
            index++;
        }
    }
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
#define move unsigned long
/*
start square = 6 bits
end square = 6 bits
piece = 4 bits
promotion = 4 bits
capture flag = 1 bit
castle flag = 1 bit
double push flag = 1 bit
en passant flag = 1 bit
total = 24 bits = 3 bytes = 6 nibbles
*/

static inline void generate_moves() {

}

int main() {
    init_attacks();
    init_fen(start_fen);
    print_board();
    return 0;
}