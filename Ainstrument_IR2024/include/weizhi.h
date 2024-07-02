#ifndef WEIZHI_H_
#define WEIZHI_H_

static constexpr unsigned short XS2[] = { 61, 61, 61, 61, 257, 257 };
static constexpr unsigned short YS2[] = { 27, 72, 117, 162, 27, 143 };
static constexpr unsigned short XE2[] = { 218, 218, 218, 218, 422, 422 };
static constexpr unsigned short YE2[] = { 68, 113, 158, 248, 132, 248 };
static constexpr unsigned short KEY2[] = { 0XF0, 0XF1, 0X23, 0X27, 0X25, 0X26 };

static constexpr unsigned short XS3[] = { 31, 31, 159, 159, 159 };
static constexpr unsigned short YS3[] = { 77, 159, 14, 100, 183 };
static constexpr unsigned short XE3[] = { 123, 123, 458, 458, 458 };
static constexpr unsigned short YE3[] = { 83, 164, 90, 176, 259 };
static constexpr unsigned short KEY3[] = { 10, 11, 0X33, 0X34, 0XAA };

static constexpr unsigned short XS4[] = {
		29, 146, 249, 353,
		29, 146, 249, 353,
		29, 146,246, 342 };
static constexpr unsigned short YS4[] = {
		78, 78, 78, 78,
		146,146,146,146,
		220,220, 189, 172 };
static constexpr unsigned short XE4[] = {
		106, 223, 326, 430,
		106, 223, 326, 430,
		106, 223, 332, 443 };
static constexpr unsigned short YE4[] = {
		84, 84, 84, 84,
		152,152,152,152,
		226,226,225, 245 };
static constexpr unsigned short KEY4[] = { 12, 13, 14, 15, 5, 6, 17, 16, 7, 8, 0XA0,
		0XAA };

static constexpr unsigned short XS6[] = {
		30,142,254,366,
		30,142,254,355,
		30,142,254,355,
		30,142,254,355
		};
static constexpr unsigned short YS6[] = {
		72,72,72,72,
		72+57,72+57,72+57,86,
		72+57*2,72+57*2,72+57*2,145,
		72+57*3,72+57*3,72+57*3,204
		 };
static constexpr unsigned short XE6[] = {
		30+77,142+77,254+77,366+77,
		30+77,142+77,254+77,450,
		30+77,142+77,254+77,450,
		30+77,142+77,254+77,450
		 };
static constexpr unsigned short YE6[] = {
		72+6,72+6,72+6,72+6,
		72+57+6,72+57+6,72+57+6,128,
		72+57*2+6,72+57*2+6,72+57*2+6,187,
		72+57*3+6,72+57*3+6,72+57*3+6,246
		 };
static constexpr unsigned short KEY6[] = {
		18,22,24,25,
		19,23,0,0XA0,
		20,3,1,0XA1,
		21,4,2,0XAA
};

static constexpr unsigned short XS7[] = { 54, 136, 218, 54, 136, 218, 54, 136, 218,
		54, 136, 218, 362, 362, 362 };
static constexpr unsigned short YS7[] = { 71, 71, 71, 115, 115, 115, 159, 159, 159,
		203, 203, 203, 72, 135, 199 };
static constexpr unsigned short XE7[] = { 54 + 81, 136 + 81, 218 + 81, 54 + 81, 136
		+ 81, 218 + 81, 54 + 81, 136 + 81, 218 + 81, 54 + 81, 136 + 81, 218
		+ 81, 362 + 67, 362 + 67, 362 + 67 };
static constexpr unsigned short YE7[] = { 71 + 43, 71 + 43, 71 + 43, 115 + 43, 115
		+ 43, 115 + 43, 159 + 43, 159 + 43, 159 + 43, 203 + 43, 203 + 43, 203
		+ 43, 72 + 50, 135 + 50, 199 + 50 };

static constexpr unsigned short XS10[] = {
0,383,11,91,171,11,91,171,270,387
};
static constexpr unsigned short YS10[] = {
2,2,189,189,189,232,232,232,219,219
};
static constexpr unsigned short XE10[] = {
95,480,70,150,230,70,150,230,361,460
};
static constexpr unsigned short YE10[] = {
182,182,222,222,222,265,265,265,262,262
};
static constexpr unsigned short KEY10[] = {0x31,0x32,0x24,0x22,0x21,0x11,0x12,0x13,0x02,0x01};


static constexpr unsigned short XS31[] = {89+12,257+12};
static constexpr unsigned short YS31[] = {159, 159};
static constexpr unsigned short XE31[] = {194+12,362+12};
static constexpr unsigned short YE31[] = {219,219};
static constexpr unsigned short KEY31[] = {0xf0,0xf1};

static constexpr unsigned short XS40[] = { 132, 170, 208, 244, 281, 318 };
static constexpr unsigned short YS40[] = { 180, 180, 180, 180, 180, 180 };
static constexpr unsigned short XE40[] = { 132 + 24, 170 + 24, 208 + 24, 244 + 24,
		281 + 24, 318 + 24 };
static constexpr unsigned short YE40[] = { 180 + 4, 180 + 4, 180 + 4, 180 + 4, 180
		+ 4, 180 + 4 };


#endif
