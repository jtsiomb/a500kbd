#ifndef DEFS_H_
#define DEFS_H_

#include <avr/io.h>

/* pin assignments:
 * D2	PS/2 clock (INT0)
 * D3	amikbd clock (INT1)
 * D4	PS/2 data
 * D5	amikbd data
 * D6	amiga reset
 * D7	amiga drvled in
 */

#define PCLK	2
#define ACLK	3
#define PDATA	4
#define ADATA	5
#define ARST	6
#define ADRVLED	7

#define PCLK_BIT	(1 << PCLK)
#define ACLK_BIT	(1 << ACLK)
#define PDATA_BIT	(1 << PDATA)
#define ADATA_BIT	(1 << ADATA)
#define ARST_BIT	(1 << ARST)
#define ADRVLED_BIT	(1 << ADRVLED)

#endif	/* DEFS_H_ */
