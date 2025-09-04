#ifndef DEBUG_H
#define DEBUG_H

// Global debug flag
extern int DEBUG;

// Debug print macro
#define DEBUG_PRINT(fmt, ...) do { if (DEBUG) printf(fmt, ##__VA_ARGS__); } while(0)

#endif