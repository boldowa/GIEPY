#pragma once
/**
 * @file Exbytes.h
 */
#ifdef __cplusplus
extern "C" {
#endif

bool NormalizeSprDataLen(RomFile* rom, const uint8* before, const uint8* after, Observers* obs);

#ifdef __cplusplus
} /*extern "C" */
#endif
