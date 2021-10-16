#include <iostream>
#include "slang.h"

int main(int, char**) 
{
    {
        SlangSession* session = spCreateSession(NULL);
        {
            SlangCompileRequest* request = spCreateCompileRequest(session);
            spSetCodeGenTarget(request, SLANG_DXIL);
            // spAddSearchPath(request, "some/path/");
            // spAddPreprocessorDefine(request, "ENABLE_FOO", "1")
            {
                int translationUnitIndex = spAddTranslationUnit(request, SLANG_SOURCE_LANGUAGE_HLSL, "");
                spAddTranslationUnitSourceFile(request, translationUnitIndex, "test.hlsl");

                SlangProfileID profileID = spFindProfile(session, "cs_6_5");
                int entryPointIndex = spAddEntryPoint(
                    request,
                    translationUnitIndex,
                    "main",
                    profileID);

                int anyErrors = spCompile(request);
                if (anyErrors)
                {
                    char const* diagnostics = spGetDiagnosticOutput(request);
                    printf("%s\n", diagnostics);
                }
                else
                {
                    size_t dataSize = 0;
                    void const* data = spGetEntryPointCode(request, entryPointIndex, &dataSize);

                    slang::ShaderReflection* shaderReflection = slang::ShaderReflection::get(request);
                    unsigned parameterCount = shaderReflection->getParameterCount();
                    for (unsigned pp = 0; pp < parameterCount; pp++)
                    {
                        slang::VariableLayoutReflection* parameter =
                            shaderReflection->getParameterByIndex(pp);
                        char const* parameterName = parameter->getName();
                        slang::ParameterCategory category = parameter->getCategory();
                        unsigned index = parameter->getBindingIndex();
                        unsigned space = parameter->getBindingSpace();
                        char const* semanticName = parameter->getSemanticName();
                    }
                }
            }
            
            spDestroyCompileRequest(request);
        }
        spDestroySession(session);
    }
    std::cout << "Hello, world!\n";
}
