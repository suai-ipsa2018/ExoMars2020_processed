#pragma once
#include "systemc.h"

/*! @file crc_8.h
 * @brief Implementation of the CRC algorithm using a 8-th order polynomial (9-bits), and thus producing an 8-bit checksum.
 * @details This implementation is designed to be used easily on a packet, where bytes arrives one at a time. It computes the CRC on a fraction only of a packet (16 bits, ie. 2 consecutive bytes).
 * By using it recusively, one can compute the whole checksum, for a packet of unknown size.
 */

const sc_uint<9> CRC_POLY_9("10101001");
const sc_uint<17> CRC_POLY_17("0bus11100010111101011"); //!< Constant global polynomial, used to compute the checksum.

/**
 * @brief CRC_8 Implementation of the CRC-8 algorithm for two consecutive bytes
 * @param a First byte
 * @param b Second byte
 * @param divisor Any 8-th order polynomial (some are better, check this [Wikipedia page concernig CRC])
 * @return remainder of the algorithm
 * [Wikipedia page concernig CRC]: https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Designing_polynomials "Wikipedia page concernig CRC"
 */
template <size_t n>
sc_uint<n> crc(sc_uint<n> a, sc_uint<n> b);