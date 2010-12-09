/*!
 * \file IOData.h
 * \brief Definition of I/O data for tests
 *
 *  \date 03-12-2010
 *  \author Kacper Szkudlarek
 */


#ifndef IODATA_H_
#define IODATA_H_

#include <stdint.h>

static const uint8_t input_data[] = {'c', 'c', 'c', 'a', 'a', 'a', 'd', 'd', 'd', 'b'};

static const uint8_t if_input_data[] = {'c', 'a', 'c', 'a', 'a', 'd', 'd', 'c', 'd', 'b'};

static const unsigned int output_if[] = {0x1, 0x1, 0x0, 0x9, 0x0, 0x0, 0x2, 0x5, 0x0, 0x0};

static const uint8_t output_mtf[] = {0x63, 0x00, 0x00, 0x62, 0x00, 0x00, 0x64, 0x00, 0x00, 0x64};

static const uint8_t output_rle_2[] = {0x63, 0x63, 0x61, 0x61, 0x64, 0x64, 0x62};

static const uint8_t output_ifc[] = {0x63, 0x00, 0x62, 0x01, 0x64, 0x02, 0x64};

static const unsigned int output_dc[] = {0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00};

static const int dict_dc[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1, - 1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x03, 0x09, 0x00, 0x06, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
							   -1, -1, -1, -1};

static const int num_elem_if[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3, 0x1, 0x3, 0x3, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const int rle_buff[] = {0x3, 0x3, 0x3};



#endif /* IODATA_H_ */
