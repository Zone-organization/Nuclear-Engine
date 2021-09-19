#include "Engine/Components\CameraComponent.h"
#include "Base/Math/gtc/matrix_inverse.hpp"
#include <Core\Engine.h>
#include <Engine/ECS\Scene.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>
#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>

namespace NuclearEngine
{
	using namespace Math;

	namespace Components
	{
		CameraComponent::CameraComponent()
			: CameraComponent(Math::Vector3(0.0f, 0.0f, 0.0f))
		{
		}
		CameraComponent::CameraComponent(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{
			
			position = __position;


		}
	
		CameraComponent::~CameraComponent()
		{
		}
	
		void CameraComponent::Initialize(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			Update();
		}

		void CameraComponent::Bake(const CameraBakingOptions& Opt)
		{
			mCameraBakingOpts = Opt;
			BakeRenderTarget(mCameraBakingOpts);
			BakePipeline(Opt);
		}

		void CameraComponent::ResizeRenderTarget(Uint32 Width, Uint32 Height)
		{
			mCameraBakingOpts.RTWidth = Width;
			mCameraBakingOpts.RTHeight = Height;
			BakeRenderTarget(mCameraBakingOpts);

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(GetCameraRT()->mShaderRTV);
		}

		void CameraComponent::Update()
		{
			Vector3 front;
			front.x = cos(radians(Yaw)) * cos(radians(Pitch));
			front.y = sin(radians(Pitch));
			front.z = sin(radians(Yaw)) * cos(radians(Pitch));
			Front = normalize(front);
			// Also re-calculate the Right and Up vector
			Right = normalize(Math::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = normalize(Math::cross(Right, Front));

			mCameraData.View = Math::lookAt(position, position + Front, Up);

			mCameraData.ModelViewProjection = mCameraData.Projection * mCameraData.View * mCameraData.Model;
			mCameraData.ModelInvTranspose = Math::inverseTranspose(mCameraData.Model);

		}

		void CameraComponent::UpdatePSO(bool ForceDirty)
		{
			int Dirty = 0;

			if(HDR_Enabled != HDR) { Dirty = Dirty + 1; }
			if(GammaCorrection_Enabled != GammaCorrection) { Dirty = Dirty + 1; }
			if(Bloom_Enabled != Bloom) { Dirty = Dirty + 1; }

			if (Dirty != 0 || ForceDirty == true)
			{
				RequiredHash = 0;

				if(HDR) { RequiredHash = RequiredHash + Utilities::Hash("NE_ENABLE_HDR"); }
				if(GammaCorrection) { RequiredHash = RequiredHash + Utilities::Hash("NE_ENABLE_GAMMA"); }
				if(Bloom) { RequiredHash = RequiredHash + Utilities::Hash("NE_ENABLE_BLOOM"); }


				auto PSO_SRB = mPipeline.GetPipeline(RequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				HDR_Enabled = HDR;
				GammaCorrection_Enabled = GammaCorrection;
				Bloom_Enabled = Bloom;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(GetCameraRT()->mShaderRTV);
			}

		}

		void CameraComponent::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			mCameraData.Model = modelMatrix;
			Update();
		}
		void CameraComponent::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			mCameraData.View = viewMatrix;
			Update();

		}
		void CameraComponent::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			Update();

		}
		void CameraComponent::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		Math::Matrix4 CameraComponent::GetModelMatrix()
		{
			return mCameraData.Model;
		}
		Math::Matrix4 CameraComponent::GetViewMatrix()
		{
			return this->mCameraData.Model;
		}
		Math::Matrix4 CameraComponent::GetProjectionMatrix()
		{
			return mCameraData.Projection;
		}
		Math::Vector3 CameraComponent::GetPosition()
		{
			return position;
		}

		Graphics::RenderTarget* CameraComponent::GetCameraRT()
		{
			return &CameraRT;
		}

		IPipelineState* CameraComponent::GetActivePipeline()
		{
			return mActivePSO.RawPtr();
		}

		IShaderResourceBinding* CameraComponent::GetActiveSRB()
		{
			return mActiveSRB.RawPtr();
		}

		void CameraComponent::BakeRenderTarget(const CameraBakingOptions& Desc)
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = Desc.RTWidth;
			RTDesc.Height = Desc.RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			CameraRT.Create(RTDesc);
		}

		void CameraComponent::BakePipeline(const CameraBakingOptions& Desc)
		{
			std::vector<LayoutElement> Layout;

		
			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::NeoPipelineDesc PSOCreateInfo;

			if (!Desc.Disable_HDR_Varient) { PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch("NE_ENABLE_HDR")); }
			if (!Desc.Disable_GammaCorrection_Varient) { PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch("NE_ENABLE_GAMMA")); }
			if (!Desc.Disable_Bloom_Varient) { PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch("NE_ENABLE_BLOOM")); }
	
			PSOCreateInfo.mVShaderPath = "Assets/NuclearEngine/Shaders/Camera.vs.hlsl";
			PSOCreateInfo.mPShaderPath = "Assets/NuclearEngine/Shaders/Camera.ps.hlsl";

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = Layout.size();
			std::vector<ShaderResourceVariableDesc> Vars;
			Vars.push_back({ SHADER_TYPE_PIXEL, "SceneTexture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });

			std::vector<ImmutableSamplerDesc> StaticSamplers;
			StaticSamplers.push_back({ SHADER_TYPE_PIXEL, "SceneTexture", Sam_LinearClamp });

			mPipeline.Create(PSOCreateInfo);

			//Create Blur pipeline
			if (!Desc.Disable_Bloom_Varient)
			{
				Managers::AutoVertexShaderDesc VertShaderDesc;
				VertShaderDesc.Use_Camera = false;
				VertShaderDesc.InNormals = false;
				VertShaderDesc.OutFragPos = false;
				VertShaderDesc.Name = "Bloom_VS";

				std::vector<LayoutElement> LayoutElems;
				RefCntAutoPtr<IShader> VShader;
				VShader = Graphics::GraphicsEngine::GetShaderManager()->CreateAutoVertexShader(VertShaderDesc, &LayoutElems);

				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.Desc.Name = "BlurPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				RefCntAutoPtr<IShader> PShader;
				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PShader);

				GraphicsPipelineStateCreateInfo PSOCreateInfo;

				PSOCreateInfo.PSODesc.Name = "CameraBloom_PSO";
				PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
				PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
				PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
				PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
				PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
				PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
				PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
				PSOCreateInfo.pVS = VShader;
				PSOCreateInfo.pPS = PShader;
				PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems.data();
				PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = LayoutElems.size();
				PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

				std::vector<ShaderResourceVariableDesc> Vars;
				Vars.push_back({ SHADER_TYPE_PIXEL, "Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });

				std::vector<ImmutableSamplerDesc> StaticSamplers;
				StaticSamplers.push_back({ SHADER_TYPE_PIXEL, "Texture", Sam_LinearClamp });

				PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = static_cast<Uint32>(Vars.size());
				PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars.data();
				PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = static_cast<Uint32>(StaticSamplers.size());
				PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = StaticSamplers.data();

				Graphics::Context::GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &mBloomPSO);

				mBloomPSO->CreateShaderResourceBinding(&mBloomSRB, true);
			}

			UpdatePSO(true);
		}

		void CameraComponent::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
		}
		void CameraComponent::ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == CAMERA_MOVEMENT_FORWARD)
				position += Front * velocity;
			if (direction == CAMERA_MOVEMENT_BACKWARD)
				position -= Front * velocity;
			if (direction == CAMERA_MOVEMENT_LEFT)
				position -= Right * velocity;
			if (direction == CAMERA_MOVEMENT_RIGHT)
				position += Right * velocity;
		}
		void CameraComponent::ProcessZoom(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		Math::Vector3 CameraComponent::GetFrontView()
		{
			return Front;
		}
	}
}