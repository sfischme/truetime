#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* random.c */
#ifndef sgi
extern void srandom P_((unsigned int x));
extern char *initstate P_((unsigned int seed, char *arg_state, int n));
extern char *setstate P_((char *arg_state));
extern long random P_((void));
#endif
