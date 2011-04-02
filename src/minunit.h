/* file: minunit.h */
/* originally from: http://www.jera.com/techinfo/jtns/jtn002.html */
/* robbywalker: modified to add setup and teardown */
/* robbywalker: modified to add mu_main */

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { setup(); char *message = test(); teardown(); tests_run++; \
                               if (message) return message; } while (0)
extern int tests_run;

extern void setup();

extern void teardown();
