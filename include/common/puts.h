/**
 * @file puts.h
 * put macro utility
 */
#pragma once

#include "common/types.h"

/**
 * It requires this var.
 */
extern bool vdebug;

/**
 * macros
 */
#define putdebug(fmt, ...) if(vdebug) fprintf(stdout, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
/*
#define putinfo(fmt, ...)             fprintf(stdout, "[INFO ] " fmt "\n", ##__VA_ARGS__)
#define putwarn(fmt, ...)             fprintf(stderr, "[WARN ] " fmt "\n", ##__VA_ARGS__)
#define puterror(fmt, ...)            fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)
#define putfatal(fmt, ...)            fprintf(stderr, "[FATAL] " fmt "\n", ##__VA_ARGS__)
*/
