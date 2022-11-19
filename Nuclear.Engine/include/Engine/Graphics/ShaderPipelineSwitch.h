#pragma once
#include <Core\NE_Common.h>
#include <string>
#include <unordered_map>
namespace Nuclear
{
	namespace Graphics
	{
		class ShaderPipeline;
		class ShaderPipelineVariant;

		class ShaderPipelineSwitch
		{
		public:
			enum class Type {
				PostProcessingEffect,
				RenderingEffect,
				PostProcessingAndRenderingEffect,
				Unknown
			};
			ShaderPipelineSwitch(const std::string& name = "", bool InitialValue = true, Type type = Type::Unknown);
			~ShaderPipelineSwitch();

			void SetValue(bool value);
			bool GetValue() const;

			Type GetType();

			std::string GetName() const;
			Uint32 GetID() const;

		private:
			Uint32 mID = 0;
			std::string mName;
			Type mType;
			bool mValue;
		};

		struct NEAPI ShaderPipelineSwitchController
		{
		public:
			ShaderPipelineSwitchController();
			~ShaderPipelineSwitchController();

			void SetPipeline(ShaderPipeline* pipeline);
			bool SetSwitch(Uint32 switchId, bool value);
			void Update(bool ForceDirty = false);

			Uint32 GetRequiredHash();
			ShaderPipelineVariant* GetActiveVariant();
			ShaderPipeline* GetShaderPipeline();
			ShaderPipelineSwitch& GetSwitch(Uint32 SwitchID);

		protected:
			std::unordered_map<Uint32, ShaderPipelineSwitch> mSwitches;

			ShaderPipeline* pPipeline;
			ShaderPipelineVariant* pActiveVariant;

			Uint32 mRequiredHash = 0;
			bool mDirty = true;

		};
	}
}