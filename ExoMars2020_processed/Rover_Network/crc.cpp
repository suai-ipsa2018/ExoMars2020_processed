#include "crc.h"

template <size_t n>
sc_uint<n> crc(sc_uint<n> a, sc_uint<n> b)
{
    sc_uint<2*n> temp = (a,b); // Temporary 16-bits value, concatenation of the two 8-bits parameter to compute CRC for
	sc_uint<2 * n> padded_divisor;
	switch (n)
	{
	case 8:
		padded_divisor = CRC_POLY_9 << (n - 1); // Padded divisor
		break;
	case 16:
		padded_divisor = CRC_POLY_17 << (n - 1); // Padded divisor
		break;
	default:
		padded_divisor = sc_uint<n+1>(CRC_POLY_17) << (n - 1); // Not ideal, could be changed
		break;
	}
    
    int j = 0; // Counter
    while(temp.range(2*n - 1, n) != 0) // While the XORs haven't been applied to the whole temp value
    {
        while(temp[(2*n - 1) - j] == 0 && j != n) // Moves the divisor to the right until to find the highest significant non-zero bit. If j==8, the computation is applied to the whole 16-bit value
        {
            padded_divisor >>= 1; // Moves the divisor
            j++;
        }
        temp ^= padded_divisor; // Polynomial division, as described in the CRC algorithm
    }
    sc_uint<n> res = temp.range(n - 1, 0); // Final result, only non-zero bits, on 8-bit : the CRC remainder
    return(res);
}


template sc_uint<8> crc<8>(sc_uint<8>, sc_uint<8>);
template sc_uint<16> crc<16>(sc_uint<16>, sc_uint<16>);