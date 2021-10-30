#include "compiler.h"

using namespace shc;

Compiler::Compiler()
{
	mpSession = spCreateSession(NULL);
}

Compiler::~Compiler()
{
	spDestroySession(mpSession);
}

std::shared_ptr<Compiler::Result> Compiler::Compile(const char* path, const char* entry, const char* profile)
{
	SlangCompileRequest* request = spCreateCompileRequest(mpSession);
	std::shared_ptr<Compiler::Result> result = std::make_shared<Compiler::Result>(request);

	spSetCodeGenTarget(request, SLANG_DXIL);
	int translationUnitIndex = spAddTranslationUnit(request, SLANG_SOURCE_LANGUAGE_HLSL, "");
	spAddTranslationUnitSourceFile(request, translationUnitIndex, path);

	SlangProfileID profileID = spFindProfile(mpSession, profile);
	int entryPointIndex = spAddEntryPoint(
		request,
		translationUnitIndex,
		entry,
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
            Variable variable;
            slang::VariableLayoutReflection* parameter =
                shaderReflection->getParameterByIndex(pp);
            variable.name = parameter->getName();
            slang::ParameterCategory category = parameter->getCategory();
            std::string typeName = parameter->getType()->getName();
            if (typeName == "ConstantBuffer")
            {
                variable.type = Variable::Type::ConstantBuffer;
            }
            else if (typeName == "StructuredBuffer")
            {
                variable.type = Variable::Type::StructuredBuffer;
            }
            else if (typeName == "RWStructuredBuffer")
            {
                variable.type = Variable::Type::RWStructuredBuffer;
            }
            variable.index = parameter->getBindingIndex();
            variable.space = parameter->getBindingSpace();

            result->mpVariables[variable.name] = variable;
        }
    }

	return result;
}

Compiler::Result::Result(SlangCompileRequest* request):
mpRequest(request)
{
}

Compiler::Result::~Result()
{
	spDestroyCompileRequest(mpRequest);
}
