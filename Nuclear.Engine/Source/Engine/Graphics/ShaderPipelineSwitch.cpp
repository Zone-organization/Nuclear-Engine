#include <Engine\Graphics\ShaderPipelineSwitch.h>
#include <Core/Utilities/Hash.h>
#include <Engine\Graphics\ShaderPipeline.h>

namespace Nuclear
{
	namespace Graphics
	{	
		ShaderPipelineSwitch::ShaderPipelineSwitch(const std::string& name, bool InitialValue, Type type)
		{
			mName = name;
			mID = Utilities::Hash(mName);
			mValue = InitialValue;
			mType = type;
		}
		ShaderPipelineSwitch::~ShaderPipelineSwitch()
		{
		}
		void ShaderPipelineSwitch::SetValue(bool value)
		{
			mValue = value;
		}
		bool ShaderPipelineSwitch::GetValue() const
		{
			return mValue;
		}
		ShaderPipelineSwitch::Type ShaderPipelineSwitch::GetType()
		{
			return mType;
		}
		std::string ShaderPipelineSwitch::GetName() const
		{
			return mName;
		}
		Uint32 ShaderPipelineSwitch::GetID() const
		{
			return mID;
		}


		ShaderPipelineSwitchController::ShaderPipelineSwitchController()
		{
			mRequiredHash = 0;
		}

		ShaderPipelineSwitchController::~ShaderPipelineSwitchController()
		{
			mRequiredHash = 0;
		}

		void ShaderPipelineSwitchController::SetPipeline(ShaderPipeline* pipeline)
		{
			pPipeline = pipeline;
			mSwitches.clear();

			for (auto& i : pipeline->GetSwitches())
			{
				mSwitches[i.GetID()] = ShaderPipelineSwitch(i.GetName());
			}
		}

		bool ShaderPipelineSwitchController::SetSwitch(Uint32 switchId, bool value, bool controllersupport)
		{
			if (controllersupport)
			{
				auto it = mSwitches.find(switchId);
				if (it != mSwitches.end())
				{
					it->second.SetValue(value);
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		void ShaderPipelineSwitchController::Update(bool ForceDirty)
		{
			mRequiredHash = 0;

			if (mDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto& it : mSwitches)
				{
					if (it.second.GetValue())
					{
						if (_hashzeroed)
						{
							mRequiredHash = 0;
							_hashzeroed = true;
						}
						auto val = mRequiredHash + it.second.GetID();

						mRequiredHash = val;
					}
				}

				pActiveVariant = pPipeline->GetVariant(mRequiredHash);
			}
		}

		Uint32 ShaderPipelineSwitchController::GetRequiredHash()
		{
			return mRequiredHash;
		}

		ShaderPipelineVariant* ShaderPipelineSwitchController::GetActiveVariant()
		{
			return pActiveVariant;
		}

		ShaderPipeline* ShaderPipelineSwitchController::GetShaderPipeline()
		{
			return pPipeline;
		}
		ShaderPipelineSwitch& ShaderPipelineSwitchController::GetSwitch(Uint32 SwitchID)
		{
			auto it = mSwitches.find(SwitchID);
			if (it != mSwitches.end())
			{
				return it->second;
			}
			else
			{
				assert(false);
			}
		}
	}
}