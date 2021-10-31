#include "compiler.h"

using namespace shc;

static Variable::Type ConvertTypeNameToVariableType(const std::string& typeName);
static Variable::Usage ConvertCategoryToVariableUsage(slang::ParameterCategory category);

Compiler::Compiler()
{
	mpSession = spCreateSession(NULL);
}

Compiler::~Compiler()
{
	spDestroySession(mpSession);
}

void Compiler::AddSearchPath(const std::string& path)
{
    mSearchPaths.push_back(path);
}

std::shared_ptr<Compiler::Result> Compiler::Compile(const char* path, const char* entry, const char* profile, DefineList& defineList, Target target, MatrixLayout matrixLayout)
{
	SlangCompileRequest* request = spCreateCompileRequest(mpSession);
	std::shared_ptr<Compiler::Result> result = std::make_shared<Compiler::Result>(request);
    spSetMatrixLayoutMode(request, matrixLayout == MatrixLayout::RowMajor? SLANG_MATRIX_LAYOUT_ROW_MAJOR: SLANG_MATRIX_LAYOUT_COLUMN_MAJOR);
	spSetCodeGenTarget(request, target==Target::DXIL? SLANG_DXIL: SLANG_HLSL);
	int translationUnitIndex = spAddTranslationUnit(request, SLANG_SOURCE_LANGUAGE_HLSL, "");
	spAddTranslationUnitSourceFile(request, translationUnitIndex, path);

	SlangProfileID profileID = spFindProfile(mpSession, profile);
    for (auto& searchPath : mSearchPaths)
    {
        spAddSearchPath(request, searchPath.c_str());
    }
    
    for (auto& pair : defineList.defines)
    {
        spAddPreprocessorDefine(request, pair.first.c_str(), pair.second.c_str());
    }

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
        if (target == Target::DXIL)
        {
            result->data = spGetEntryPointCode(request, entryPointIndex, &result->size);
        }
        else
        {
            result->code = spGetEntryPointSource(request, entryPointIndex);
            printf("%s\n", result->code);
        }
        slang::ShaderReflection* shaderReflection = slang::ShaderReflection::get(request);
        unsigned parameterCount = shaderReflection->getParameterCount();
        for (unsigned pp = 0; pp < parameterCount; pp++)
        {
            Variable variable;
            slang::VariableLayoutReflection* parameter =
                shaderReflection->getParameterByIndex(pp);
            
            variable.name = parameter->getName();
            variable.index = parameter->getBindingIndex();
            variable.space = parameter->getBindingSpace();
            variable.usage = ConvertCategoryToVariableUsage(parameter->getCategory());
            std::string typeName;
            if (parameter->getType()->isArray())
            {
                variable.count = parameter->getTypeLayout()->getElementCount();
                typeName = parameter->getType()->unwrapArray()->getName();
            }
            else
            {
                typeName = parameter->getType()->getName();
                variable.count = 1;
            }
            variable.type = ConvertTypeNameToVariableType(typeName);
           
            result->variables[variable.name] = variable;
        }


        SlangUInt entryPointCount = shaderReflection->getEntryPointCount();
        for (SlangUInt ee = 0; ee < entryPointCount; ee++)
        {
            slang::EntryPointReflection* entryPoint =
                shaderReflection->getEntryPointByIndex(ee);
            std::string entryPointName = entryPoint->getName();
            if (entryPointName == entry && entryPoint->getStage() == SLANG_STAGE_COMPUTE)
            {
                SlangUInt threadGroupSize[3];
                entryPoint->getComputeThreadGroupSize(3, &threadGroupSize[0]);

                result->threadGroupSize[0] = threadGroupSize[0];
                result->threadGroupSize[1] = threadGroupSize[1];
                result->threadGroupSize[2] = threadGroupSize[2];
            }
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

void DefineList::AddDefine(const std::string& name, const std::string& value)
{
    defines[name] = value;
}

void DefineList::RemoveDefine(const std::string& name)
{
    defines.erase(name);
}

static Variable::Type ConvertTypeNameToVariableType(const std::string& typeName)
{
    Variable::Type type = Variable::Type::Unknown;

    if (typeName == "ConstantBuffer")
    {
        type = Variable::Type::ConstantBuffer;
    }
    else if (typeName == "ByteAddressBuffer")
    {
        type = Variable::Type::ByteAddressBuffer;
    }
    else if (typeName == "RWByteAddressBuffer")
    {
        type = Variable::Type::RWByteAddressBuffer;
    }
    else if (typeName == "Buffer")
    {
        type = Variable::Type::Buffer;
    }
    else if (typeName == "RWBuffer")
    {
        type = Variable::Type::RWBuffer;
    }
    else if (typeName == "StructuredBuffer")
    {
        type = Variable::Type::StructuredBuffer;
    }
    else if (typeName == "RWStructuredBuffer")
    {
        type = Variable::Type::RWStructuredBuffer;
    }
    else if (typeName == "SamplerState")
    {
        type = Variable::Type::SamplerState;
    }
    else if (typeName == "Texture2D")
    {
        type = Variable::Type::Texture2D;
    }
    else if (typeName == "RWTexture2D")
    {
        type = Variable::Type::RWTexture2D;
    }
    else if (typeName == "Texture2DArray")
    {
        type = Variable::Type::Texture2DArray;
    }
    else if (typeName == "RWTexture2DArray")
    {
        type = Variable::Type::RWTexture2DArray;
    }
    else if (typeName == "Texture3D")
    {
        type = Variable::Type::Texture3D;
    }
    else if (typeName == "RWTexture3D")
    {
        type = Variable::Type::RWTexture3D;
    }
    else if (typeName == "RaytracingAccelerationStructure")
    {
        type = Variable::Type::RaytracingAccelerationStructure;
    }

    return type;
}

Variable::Usage ConvertCategoryToVariableUsage(slang::ParameterCategory category)
{
    Variable::Usage usage = Variable::Usage::Unknown;

    switch (category)
    {
    case slang::ConstantBuffer:
        usage = Variable::Usage::ConstantBuffer;
        break;
    case slang::ShaderResource:
        usage = Variable::Usage::ShaderResource;
        break;
    case slang::UnorderedAccess:
        usage = Variable::Usage::UnorderedAccess;
        break;
    case slang::SamplerState:
        usage = Variable::Usage::SamplerState;
        break;
    }

    return usage;
}