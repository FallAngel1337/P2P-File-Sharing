#include "../cutest-1.5/CuTest.h"
#include <stdio.h>

CuSuite* FileInfoGetSuite();

void RunAllTests()
{
    CuString *output = CuStringNew();
    CuSuite *suite =  CuSuiteNew();

    CuSuiteAddSuite(suite, FileInfoGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
}