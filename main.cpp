#include <iostream>
#include "slang.h"

int main(int, char**) 
{
    {
        SlangSession* session = spCreateSession(NULL);
        {
            // SlangCompileRequest* request = spCreateCompileRequest(session);
            // spSetCodeGenTarget(request, SLANG_DXIL);
            // // spAddSearchPath(request, "some/path/");
            // // spAddPreprocessorDefine(request, "ENABLE_FOO", "1")
            // spAddTranslationUnit(request, SLANG_SOURCE_LANGUAGE_HLSL, "");

            // spDestroyCompileRequest(request);
        }
        spDestroySession(session);
    }
    std::cout << "Hello, world!\n";
}
