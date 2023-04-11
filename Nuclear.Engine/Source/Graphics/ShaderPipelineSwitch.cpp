#include <Graphics\ShaderPipelineSwitch.h>
#include <Utilities/Hash.h>
#include <Graphics\ShaderPipeline.h>

namespace Nuclear
{
	namespace Graphics
	{	
		ShaderPipelineSwitch::ShaderPipelineSwitch(const std::string& name, bool InitialValue, Type type)
		{
			SetName(name);
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
		void ShaderPipelineSwitch::SetName(const std::string& name)
		{
			mName = name;
			mID = Utilities::Hash(mName);
		}
		void ShaderPipelineSwitch::SetType(Type type)
		{
			mType = type;
		}
		ShaderPipelineSwitch::Type ShaderPipelineSwitch::GetType() const
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
			mSwitches = std::unordered_map<Uint32, ShaderPipelineSwitch>();
			pActiveVariant = nullptr;
			pPipeline = nullptr;
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
				mSwitches[i.GetID()] = ShaderPipelineSwitch(i.GetName(), i.GetValue(), i.GetType());
			}
		}

		bool ShaderPipelineSwitchController::SetSwitch(Uint32 switchId, bool value, bool controllersupport)
		{
			if (controllersupport)
			{
				auto it = mSwitches.find(switchId);
				if (it != mSwitches.end())
				{
					if (it->second.GetValue() != value)
					{
						it->second.SetValue(value);
						mDirty = true;
					}
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		bool ShaderPipelineSwitchController::Update(bool ForceDirty)
		{
			if (pPipeline->GetVariants().size() == 0)
			{
				return false;
			}
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
				mDirty = false;
			}

			return true;
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
		std::unordered_map<Uint32, ShaderPipelineSwitch>& ShaderPipelineSwitchController::GetSwitches()
		{
			return mSwitches;
		}
	}
}