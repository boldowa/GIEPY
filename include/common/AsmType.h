#pragma once
/**
 * @file AsmType.h
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _AsmType {
	AsmType_Unknown = 0,
	AsmType_TRASM,
	AsmType_ASAR,
	AsmType_XKAS,		/* it isn't used */
	AsmType_WLA,		/* it isn't used */
	AsmType_BASS,		/* it isn't used */
}AsmType;

#ifdef __cplusplus
} /*extern "C" */
#endif
