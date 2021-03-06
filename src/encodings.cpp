/**
 *  \file src/encodings.cpp
 *
 *  \brief List of encodings
 *
 *  \author Olivia Quinet
 */

#include "encodings.hpp"

std::string_view
cppsas7bdat::INTERNAL::get_encoding(const uint8_t _e) noexcept {
  static constexpr const char *SAS_DEFAULT_STRING_ENCODING = "WINDOWS-1252";
  static constexpr const char *encoding[256] = {
      /*[0] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[1] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[2] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[3] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[4] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[5] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[6] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[7] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[8] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[9] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[10] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[11] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[12] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[13] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[14] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[15] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[16] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[17] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[18] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[19] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[20] = */ "UTF-8",
      /*[21] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[22] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[23] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[24] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[25] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[26] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[27] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[28] = */ "US-ASCII",
      /*[29] = */ "ISO-8859-1",
      /*[30] = */ "ISO-8859-2",
      /*[31] = */ "ISO-8859-3",
      /*[32] = */ "ISO-8859-4",
      /*[33] = */ "ISO-8859-5",
      /*[34] = */ "ISO-8859-6",
      /*[35] = */ "ISO-8859-7",
      /*[36] = */ "ISO-8859-8",
      /*[37] = */ "ISO-8859-9",
      /*[38] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[39] = */ "ISO-8859-11",
      /*[40] = */ "ISO-8859-15",
      /*[41] = */ "CP437",
      /*[42] = */ "CP850",
      /*[43] = */ "CP852",
      /*[44] = */ "CP857",
      /*[45] = */ "CP858",
      /*[46] = */ "CP862",
      /*[47] = */ "CP864",
      /*[48] = */ "CP865",
      /*[49] = */ "CP866",
      /*[50] = */ "CP869",
      /*[51] = */ "CP874",
      /*[52] = */ "CP921",
      /*[53] = */ "CP922",
      /*[54] = */ "CP1129",
      /*[55] = */ "CP720",
      /*[56] = */ "CP737",
      /*[57] = */ "CP775",
      /*[58] = */ "CP860",
      /*[59] = */ "CP863",
      /*[60] = */ "WINDOWS-1250",
      /*[61] = */ "WINDOWS-1251",
      /*[62] = */ "WINDOWS-1252",
      /*[63] = */ "WINDOWS-1253",
      /*[64] = */ "WINDOWS-1254",
      /*[65] = */ "WINDOWS-1255",
      /*[66] = */ "WINDOWS-1256",
      /*[67] = */ "WINDOWS-1257",
      /*[68] = */ "WINDOWS-1258",
      /*[69] = */ "MACROMAN",
      /*[70] = */ "MACARABIC",
      /*[71] = */ "MACHEBREW",
      /*[72] = */ "MACGREEK",
      /*[73] = */ "MACTHAI",
      /*[74] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[75] = */ "MACTURKISH",
      /*[76] = */ "MACUKRAINE",
      /*[77] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[78] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[79] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[80] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[81] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[82] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[83] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[84] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[85] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[86] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[87] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[88] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[89] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[90] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[91] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[92] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[93] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[94] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[95] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[96] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[97] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[98] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[99] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[100] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[101] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[102] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[103] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[104] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[105] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[106] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[107] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[108] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[109] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[110] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[111] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[112] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[113] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[114] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[115] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[116] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[117] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[118] = */ "CP950",
      /*[119] = */ "EUC-TW",
      /*[120] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[121] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[122] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[123] = */ "BIG5-HKSCS",
      /*[124] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[125] = */ "GB18030",
      /*[126] = */ "CP936",
      /*[127] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[128] = */ "CP1381",
      /*[129] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[130] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[131] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[132] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[133] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[134] = */ "EUC-JP",
      /*[135] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[136] = */ "CP949",
      /*[137] = */ "CP942",
      /*[138] = */ "CP932",
      /*[139] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[140] = */ "EUC-KR",
      /*[141] = */ "CP949",
      /*[142] = */ "CP949",
      /*[143] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[144] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[145] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[146] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[147] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[148] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[149] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[150] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[151] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[152] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[153] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[154] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[155] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[156] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[157] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[158] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[159] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[160] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[161] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[162] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[163] = */ "MACICELAND",
      /*[164] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[165] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[166] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[167] = */ "ISO-2022-JP",
      /*[168] = */ "ISO-2022-KR",
      /*[169] = */ "ISO-2022-CN",
      /*[170] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[171] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[172] = */ "ISO-2022-CN-EXT",
      /*[173] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[174] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[175] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[176] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[177] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[178] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[179] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[180] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[181] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[182] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[183] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[184] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[185] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[186] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[187] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[188] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[189] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[190] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[191] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[192] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[193] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[194] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[195] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[196] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[197] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[198] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[199] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[200] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[201] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[202] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[203] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[204] = */ SAS_DEFAULT_STRING_ENCODING, // any
      /*[205] = */ "GB18030",
      /*[206] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[207] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[208] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[209] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[210] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[211] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[212] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[213] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[214] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[215] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[216] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[217] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[218] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[219] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[220] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[221] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[222] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[223] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[224] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[225] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[226] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[227] = */ "ISO-8859-14",
      /*[228] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[229] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[230] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[231] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[232] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[233] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[234] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[235] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[236] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[237] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[238] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[239] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[240] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[241] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[242] = */ "ISO-8859-13",
      /*[243] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[244] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[245] = */ "MACCROATIAN",
      /*[246] = */ "MACCYRILLIC",
      /*[247] = */ "MACROMANIA",
      /*[248] = */ "SHIFT_JISX0213",
      /*[249] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[250] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[251] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[252] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[253] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[254] = */ SAS_DEFAULT_STRING_ENCODING,
      /*[255] = */ SAS_DEFAULT_STRING_ENCODING};
  return encoding[_e];
}
