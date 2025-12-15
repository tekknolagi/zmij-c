// A double-to-string conversion algorithm based on Schubfach.
// Copyright (c) 2025 - present, Victor Zverovich
// Distributed under the MIT license (see LICENSE) or alternatively
// the Boost Software License, Version 1.0.

#ifndef ZMIJ_H
#define ZMIJ_H

#ifdef __cplusplus
extern "C" {
#endif

#define ZMIJ_BUFFER_SIZE 25

/// Writes the shortest correctly rounded decimal representation of `value` to
/// `buffer`. `buffer` should point to a buffer of size `ZMIJ_BUFFER_SIZE` or larger.
void zmij_dtoa(double value, char* buffer);

#ifdef __cplusplus
}
#endif

#endif  // ZMIJ_H
