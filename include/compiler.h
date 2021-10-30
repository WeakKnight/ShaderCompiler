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
			SamplerState,
			RaytracingAccelerationStructure,
			TextureHeap,
			BufferHeap,
			Unknown
		};

		enum class Usage
		{
			ConstantBuffer,
			ShaderResource,
			UnorderedAccess,
			SamplerState,
			Unknown
		};

		std::string name;
		Type type;
		Usage usage;

		unsigned int index;
		unsigned int space;
		unsigned int count;
	};

	struct DefineList
	{
		void AddDefine(const std::string& name, const std::string& value = "");
		void RemoveDefine(const std::string& name);
		std::unordered_map<std::string, std::string> defines;
	};

	class Compiler
	{
	public:
		
		class Result
		{
		public:
			Result(SlangCompileRequest* request);
			~Result();
			std::unordered_map<std::string, Variable> variables;
		private:
			SlangCompileRequest* mpRequest;
		};

		Compiler();
		~Compiler();
		
		void AddSearchPath(const std::string& path);
		std::shared_ptr<Result> Compile(const char* path, const char* entry, const char* profile, DefineList& defineList = DefineList());

	private:
		std::vector<std::string> mSearchPaths;
		SlangSession* mpSession;
	};
}