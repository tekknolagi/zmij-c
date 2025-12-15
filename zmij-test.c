#include "zmij.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

// Minimal assert function
#define ASSERT_EQ(actual, expected) \
  do { \
    if (strcmp((actual), (expected)) != 0) { \
      fprintf(stderr, "FAIL: %s:%d: Expected '%s', got '%s'\n", \
              __FILE__, __LINE__, (expected), (actual)); \
      exit(1); \
    } \
  } while (0)

#define ASSERT_EQ_INT(actual, expected) \
  do { \
    if ((actual) != (expected)) { \
      fprintf(stderr, "FAIL: %s:%d: Expected %d, got %d\n", \
              __FILE__, __LINE__, (expected), (int)(actual)); \
      exit(1); \
    } \
  } while (0)

#define ASSERT_EQ_U64(actual, expected) \
  do { \
    if ((actual) != (expected)) { \
      fprintf(stderr, "FAIL: %s:%d: Expected 0x%llx, got 0x%llx\n", \
              __FILE__, __LINE__, (unsigned long long)(expected), (unsigned long long)(actual)); \
      exit(1); \
    } \
  } while (0)

#define TEST(name) \
  static void test_##name(void); \
  static void test_##name(void)

// Forward declarations for internal functions (for testing)
int zmij_count_lzero(uint64_t x);
size_t zmij_count_trailing_nonzeros(uint64_t x);
uint64_t zmij_umul192_upper64_inexact_to_odd(uint64_t x_hi, uint64_t x_lo, uint64_t y);

// Helper typedef from implementation
typedef struct {
  uint64_t hi;
  uint64_t lo;
} zmij_uint128;

extern const zmij_uint128 zmij_pow10_significands[];

static void dtoa_to_string(double value, char* buffer) {
  zmij_dtoa(value, buffer);
}

TEST(utilities) {
  ASSERT_EQ_INT(zmij_count_lzero(0), 64);
  ASSERT_EQ_INT(zmij_count_lzero(1), 63);
  ASSERT_EQ_INT(zmij_count_lzero(~0ull), 0);

  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3030303030303030ull), 0);
  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3030303030303031ull), 1);
  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3030303030303039ull), 1);
  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3039303039303030ull), 7);
  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3130303030303030ull), 8);
  ASSERT_EQ_INT(zmij_count_trailing_nonzeros(0x3930303030303030ull), 8);
}

TEST(umul192_upper64_inexact_to_odd) {
  zmij_uint128 pow10 = zmij_pow10_significands[0];
  ASSERT_EQ_U64(zmij_umul192_upper64_inexact_to_odd(pow10.hi, pow10.lo,
                                           0x1234567890abcdefull << 1),
            0x24554a3ce60a45f5ull);
  ASSERT_EQ_U64(zmij_umul192_upper64_inexact_to_odd(pow10.hi, pow10.lo,
                                           0x1234567890abce16ull << 1),
            0x24554a3ce60a4643ull);
}

TEST(normal) {
  char buffer[ZMIJ_BUFFER_SIZE];
  dtoa_to_string(6.62607015e-34, buffer);
  ASSERT_EQ(buffer, "6.62607015e-34");
}

TEST(zero) {
  char buffer[ZMIJ_BUFFER_SIZE];
  dtoa_to_string(0, buffer);
  ASSERT_EQ(buffer, "0");
  dtoa_to_string(-0.0, buffer);
  ASSERT_EQ(buffer, "-0");
}

TEST(inf) {
  char buffer[ZMIJ_BUFFER_SIZE];
  dtoa_to_string(INFINITY, buffer);
  ASSERT_EQ(buffer, "inf");
  dtoa_to_string(-INFINITY, buffer);
  ASSERT_EQ(buffer, "-inf");
}

TEST(shorter) {
  char buffer[ZMIJ_BUFFER_SIZE];
  // A possibly shorter underestimate is picked (u' in Schubfach).
  dtoa_to_string(-4.932096661796888e-226, buffer);
  ASSERT_EQ(buffer, "-4.932096661796888e-226");

  // A possibly shorter overestimate is picked (w' in Schubfach).
  dtoa_to_string(3.439070283483335e+35, buffer);
  ASSERT_EQ(buffer, "3.439070283483335e+35");
}

TEST(single_candidate) {
  char buffer[ZMIJ_BUFFER_SIZE];
  // Only an underestimate is in the rounding region (u in Schubfach).
  dtoa_to_string(6.606854224493745e-17, buffer);
  ASSERT_EQ(buffer, "6.606854224493745e-17");

  // Only an overestimate is in the rounding region (w in Schubfach).
  dtoa_to_string(6.079537928711555e+61, buffer);
  ASSERT_EQ(buffer, "6.079537928711555e+61");
}

int main(void) {
  printf("Running zmij tests...\n");

  test_utilities();
  printf("  utilities: PASS\n");

  test_umul192_upper64_inexact_to_odd();
  printf("  umul192_upper64_inexact_to_odd: PASS\n");

  test_normal();
  printf("  normal: PASS\n");

  test_zero();
  printf("  zero: PASS\n");

  test_inf();
  printf("  inf: PASS\n");

  test_shorter();
  printf("  shorter: PASS\n");

  test_single_candidate();
  printf("  single_candidate: PASS\n");

  printf("All tests passed!\n");
  return 0;
}
