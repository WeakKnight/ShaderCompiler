#pragma once
#include <filesystem>
#include <memory>
#include <unordered_map>
#include "slang.h"

namespace shc
{
	struct Variable
	{
		enum class Type
		{
			ConstantBuffer,
			Texture2D,
			Texture2DArray,
			Texture3D,
			RWTexture2D,
			RWTexture2DArray,
			RWTexture3D,
			Buffer,
			ByteAddressBuffer,
			StructuredBuffer,
			RWBuffer,
			RWByteAddressBuffer,
			RWStructuredBuffer,
			RaytracingAccelerationStructure,
			TextureHeap,
			BufferHeap,
		};

		std::string name;
		Type type;
		unsigned int index;
		unsigned int space;
	};

	class Compiler
	{
	public:
		
		class Result
		{
		public:
			Result(SlangCompileRequest* request);
			~Result();
			friend Compiler;
		private:
			std::unordered_map<std::string, Variable> mpVariables;
			SlangCompileRequest* mpRequest;
		};

		Compiler();
		~Compiler();
		
		std::shared_ptr<Result> Compile(const char* path, const char* entry, const char* profile);

	private:
		SlangSession* mpSession;
	};
}