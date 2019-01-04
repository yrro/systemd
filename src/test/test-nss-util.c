/* SPDX-License-Identifier: LGPL-2.1+ */

#include <errno.h>

#include "log.h"
#include "nss-util.h"
#include "util.h"

static void test_assign_errnop(void) {
        log_info("/* %s */", __func__);

        errno = 12;
        int *errnop = &errno;

        {
                PROTECT_ERRNO;

                *errnop = ASSIGN_ERRNOP(25);
        }

        assert_se(errno == 25);
        assert_se(*errnop == 25);
}

static void test_assign_errnop_static(void) {
        log_info("/* %s */", __func__);

        errno = 12;
        /*
         * "In statically linked programs, the main application and NSS
         * modules do not share the same thread-local variable errno,
         * which is the reason why there is an explicit errnop function
         * argument."
         */
        int errno2 = 15, *errnop = &errno2;

        {
                PROTECT_ERRNO;

                errno = 13;
                *errnop = ASSIGN_ERRNOP(25);
        }

        assert_se(errno == 12);
        assert_se(*errnop == 25);
}

int main(int argc, char *argv[]) {
        test_assign_errnop();
        test_assign_errnop_static();

        return 0;
}
