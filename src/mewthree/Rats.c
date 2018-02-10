/**
 * @file Rats.c
 */
#include "common/types.h"
#include "common/ReadWrite.h"
#include "mewthree/Rats.h"

/**
 * @brief Write tagged data to dest ram
 *        The RATS rag is automatically added.
 *
 * @param dest dest ram pointer
 * @param src src data
 * @param length src data length(exclude rats tag)
 *
 * @return succeeded(true) / failure
 */
bool WriteRatsData(uint8* dest, const uint8* src, uint length)
{
	if((NULL==dest) || (NULL==src))
	{
		/* invalid parameter */
		return false;
	}
	if(0==length)
	{
		/* nothing to do */
		return true;
	}

	/* write RATS tag header */
	memcpy(dest, "STAR", 4);
	write16(dest+4, (uint16)(length-1));
	write16(dest+6, (uint16)((length-1)^0xffff));

	/* write data */
	memcpy(dest+8, src, length);

	return true;
}
