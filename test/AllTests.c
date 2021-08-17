#include "cutest/CuTest.h"
#include <stdio.h>

CuSuite* FileInfoLoadGetSuite();
CuSuite* JsonSerializationSuite();

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

void RunJsonTests(void)
{
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, JsonSerializationSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

void RunAllTests(void)
{
    RunFileInfoTest();
    RunJsonTests();
}

int main(void)
{
    RunAllTests();
}