#include "../cutest/CuTest.h"
#include "../../src/server/include/net/rtable/rtable.h"

#include <stdio.h>
#include <stdlib.h>

void TestColision(CuTest *tc)
{
    printf("Testing hash colision ...\n");
    int colided = 0;
    float colidedpercent = 0;
    int rep[1000], ok = 0;
    memset(rep, 0, sizeof(rep));
    
    for (int i=0; i < 1000; i++) {
        uint64_t n = mulhashing(i, 1000);
        for (int j=0; j < 1000; j++) {
            if (n == rep[j]) colided++;
        }
        rep[i] = n;
    }

    colidedpercent = (colided-1000)/100;
    printf("Total: 1000\nColided: %d\nColided%%: %.2f%%\n", colided, colidedpercent);
    if (colidedpercent > 5) ok = 1;

    CuAssertIntEquals(tc, 0, ok);
}

CuSuite* ColisionSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestColision);
    return suite;
}