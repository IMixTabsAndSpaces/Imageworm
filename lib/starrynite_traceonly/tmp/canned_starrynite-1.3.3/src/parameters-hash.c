/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: /usr/bin/gperf --output-file=parameters-hash.c parameters-hash.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 19 "parameters-hash.gperf"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "parameters-hash.h"
#line 34 "parameters-hash.gperf"
struct parameter;
/* maximum key range = 55, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 10,
       0, 20, 15, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63,  0, 63, 45,
      40,  0, 25, 10, 63, 63, 63, 63, 40, 20,
       0, 63, 10, 63,  0,  5,  0, 63, 63, 63,
       0,  5,  5, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct parameter *
in_word_set (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 33,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 19,
      MIN_HASH_VALUE = 8,
      MAX_HASH_VALUE = 62
    };

  static const struct parameter wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 46 "parameters-hash.gperf"
      {"nuc_size", NUC_SIZE},
      {""},
#line 36 "parameters-hash.gperf"
      {"time_start", TIME_START},
#line 40 "parameters-hash.gperf"
      {"xy_res", XY_RES},
#line 59 "parameters-hash.gperf"
      {"noise_factor", NOISE_FACTOR},
#line 61 "parameters-hash.gperf"
      {"noise_factor2", NOISE_FACTOR2},
#line 53 "parameters-hash.gperf"
      {"noise_fraction", NOISE_FRACTION},
#line 41 "parameters-hash.gperf"
      {"z_res", Z_RES},
#line 50 "parameters-hash.gperf"
      {"nuc_size_factor2", NUC_SIZE_FACTOR2},
#line 44 "parameters-hash.gperf"
      {"neighborhood_size", NEIGHBORHOOD_SIZE},
#line 64 "parameters-hash.gperf"
      {"noise_time_switch2", NOISE_TIME_SWITCH2},
#line 57 "parameters-hash.gperf"
      {"nuc_density_cutoff2", NUC_DENSITY_CUTOFF2},
#line 47 "parameters-hash.gperf"
      {"polar_size", POLAR_SIZE},
#line 38 "parameters-hash.gperf"
      {"plane_start", PLANE_START},
#line 68 "parameters-hash.gperf"
      {"expand_elastisity", EXPAND_ELASTISITY},
#line 60 "parameters-hash.gperf"
      {"noise_factor1", NOISE_FACTOR1},
#line 43 "parameters-hash.gperf"
      {"graphic_output", GRAPHIC_OUTPUT},
      {""},
#line 49 "parameters-hash.gperf"
      {"nuc_size_factor1", NUC_SIZE_FACTOR1},
#line 67 "parameters-hash.gperf"
      {"shrink_elastisity", SHRINK_ELASTISITY},
#line 63 "parameters-hash.gperf"
      {"noise_time_switch1", NOISE_TIME_SWITCH1},
#line 56 "parameters-hash.gperf"
      {"nuc_density_cutoff1", NUC_DENSITY_CUTOFF1},
      {""},
#line 52 "parameters-hash.gperf"
      {"nuc_size_factor4", NUC_SIZE_FACTOR4},
#line 48 "parameters-hash.gperf"
      {"max_nuc_size", MAX_NUC_SIZE},
#line 62 "parameters-hash.gperf"
      {"noise_factor3", NOISE_FACTOR3},
      {""}, {""},
#line 51 "parameters-hash.gperf"
      {"nuc_size_factor3", NUC_SIZE_FACTOR3},
      {""},
#line 65 "parameters-hash.gperf"
      {"minimal_cell_cycle", MINIMAL_CELL_CYCLE},
#line 58 "parameters-hash.gperf"
      {"nuc_density_cutoff3", NUC_DENSITY_CUTOFF3},
      {""},
#line 66 "parameters-hash.gperf"
      {"ambiguity_cutoff", AMBIGUITY_CUTOFF},
      {""},
#line 55 "parameters-hash.gperf"
      {"nuc_density_cutoff", NUC_DENSITY_CUTOFF},
      {""}, {""}, {""}, {""},
#line 37 "parameters-hash.gperf"
      {"time_end", TIME_END},
      {""}, {""}, {""}, {""},
#line 42 "parameters-hash.gperf"
      {"time_interval", TIME_INTERVAL},
      {""}, {""}, {""}, {""},
#line 45 "parameters-hash.gperf"
      {"cell_ct_limit", CELL_CT_LIMIT},
#line 39 "parameters-hash.gperf"
      {"plane_end", PLANE_END},
      {""}, {""},
#line 54 "parameters-hash.gperf"
      {"max_weight_cutoff", MAX_WEIGHT_CUTOFF}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
