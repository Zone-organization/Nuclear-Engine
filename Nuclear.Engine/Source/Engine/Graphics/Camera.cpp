#include "Engine/Graphics\Camera.h"
#include "Core/Math/gtc/matrix_inverse.hpp"
#include <Engine.h>
#include <Engine/ECS\Scene.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>
#include <Engine\Managers\ShaderManager.h>
#include <Core\FileSystem.h>

namespace Nuclear
{
	using namespace Math;

	namespace Graphics
	{
		Camera::Camera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: position(__position), Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{
			mCameraEffects.push_back(Rendering::ShaderEffect("HDR", Rendering::ShaderEffect::Type::CameraEffect, false));
			mCameraEffects.push_back(Rendering::ShaderEffect("GAMMACORRECTION", Rendering::ShaderEffect::Type::CameraEffect, false));
			mCameraEffects.push_back(Rendering::ShaderEffect("BLOOM", Rendering::ShaderEffect::Type::CameraAndRenderingEffect, false));
		}

		Camera::~Camera()
		{
		}

		void Camera::Initialize(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();
		}

		void Camera::Bake(Uint32 Width, Uint32 Height)
		{
			RTWidth = Width;
			RTHeight = Height;
			BakeRenderTarget();
			BakePipeline();
		}

		void Camera::ResizeRenderTarget(Uint32 Width, Uint32 Height)
		{
			RTWidth = Width;
			RTHeight = Height;
			BakeRenderTarget();

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
		}

		void Camera::UpdateBuffer()
		{
			Math::Vector3 front;
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

		void Camera::UpdatePSO(bool ForceDirty)
		{
			RequiredHash = 0;

			if (mPipelineDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto it : mCameraEffects)
				{
					if (it.GetValue())
					{
						if (_hashzeroed)
						{
							RequiredHash = 0;
							_hashzeroed = true;
						}
						auto val = RequiredHash + it.GetID();

						RequiredHash = val;
					}
				}

				auto PSO_SRB = mPipeline.GetVariant(RequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(SceneRT.mShaderRTV);
			}
		}

		void Camera::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			mCameraData.Model = modelMatrix;
			UpdateBuffer();
		}
		void Camera::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			mCameraData.View = viewMatrix;
			UpdateBuffer();

		}
		void Camera::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();

		}
		void Camera::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		Math::Matrix4 Camera::GetModelMatrix()
		{
			return mCameraData.Model;
		}
		Math::Matrix4 Camera::GetViewMatrix()
		{
			return this->mCameraData.Model;
		}
		Math::Matrix4 Camera::GetProjectionMatrix()
		{
			return mCameraData.Projection;
		}
		Math::Vector3 Camera::GetPosition()
		{
			return position;
		}

		Graphics::RenderTarget* Camera::GetSceneRT()
		{
			return &SceneRT;
		}

		IPipelineState* Camera::GetActivePipeline()
		{
			return mActivePSO.RawPtr();
		}

		IShaderResourceBinding* Camera::GetActiveSRB()
		{
			return mActiveSRB.RawPtr();
		}

		void Camera::SetEffect(const std::string& effectname, bool value)
		{

			for (auto it : mCameraEffects)
			{
				if (it.GetName() == effectname)
				{
					it.SetValue(value);
				}
			}
			mPipelineDirty = true;
		}

		void Camera::BakeBloom()
		{
			RefCntAutoPtr<IShader> VShader;
			RefCntAutoPtr<IShader> PShader;
			std::vector<LayoutElement> LayoutElems;

			{
				LayoutElems.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));//POS
				LayoutElems.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false)); //UV

				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_VERTEX;
				CreationAttribs.Desc.Name = "BlurVS";
				CreationAttribs.EntryPoint = "main";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/BasicVertex.vs.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &VShader);
			}
			{
				ShaderCreateInfo CreationAttribs;
				CreationAttribs.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
				CreationAttribs.UseCombinedTextureSamplers = true;
				CreationAttribs.Desc.ShaderType = SHADER_TYPE_PIXEL;
				CreationAttribs.Desc.Name = "BlurPS";

				auto source = Core::FileSystem::LoadShader("Assets/NuclearEngine/Shaders/Blur.hlsl", std::vector<std::string>(), std::vector<std::string>(), true);
				CreationAttribs.Source = source.c_str();

				Graphics::Context::GetDevice()->CreateShader(CreationAttribs, &PShader);
			}
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
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(LayoutElems.size());
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

			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			BloomRT.Create(RTDesc);
		}

		void Camera::BakeRenderTarget()
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = RTWidth;
			RTDesc.Height = RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			SceneRT.Create(RTDesc);
		}

		void Camera::BakePipeline()
		{
			std::vector<LayoutElement> Layout;

			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::CompoundPipelineDesc PSOCreateInfo;

			for (auto it : mCameraEffects)
			{
				PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.GetName()));
			}

			PSOCreateInfo.mVShaderPath = VS_Path;
			PSOCreateInfo.mPShaderPath = PS_Path;

			PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
			PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = Graphics::Context::GetSwapChain()->GetDesc().ColorBufferFormat;
			PSOCreateInfo.GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = false;
			PSOCreateInfo.GraphicsPipeline.DSVFormat = Graphics::Context::GetSwapChain()->GetDesc().DepthBufferFormat;
			PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
			PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
			PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
			PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = Layout.data();
			PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = static_cast<Uint32>(Layout.size());

			mPipeline.Create(PSOCreateInfo);

			bool bloomincluded = false;
			for (auto it : mCameraEffects)
			{
				if(it.GetName() == "BLOOM")
				{
					bloomincluded = true;
				}
			}

			//Create Blur pipeline
			if (bloomincluded)
			{
				BakeBloom();
			}

			UpdatePSO(true);
		}

		void Camera::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
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
		void Camera::ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime)
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
		void Camera::ProcessZoom(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		Math::Vector3 Camera::GetFrontView()
		{
			return Front;
		}
	}
}