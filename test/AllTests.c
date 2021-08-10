#include "cutest/CuTest.h"

CuSuite* FileInfoLoadGetSuite();

void RunFileInfoTest(void)
{
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, FileInfoLoadGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

void RunAllTests(void)
{
    RunFileInfoTest();
}

int main(void)
{
    RunAllTests();
}