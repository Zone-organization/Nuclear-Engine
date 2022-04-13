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
		Camera::Camera()
			: Camera(Math::Vector3(0.0f, 0.0f, 0.0f))
		{
		}
		Camera::Camera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{

			position = __position;


		}

		Camera::~Camera()
		{
		}

		void Camera::Initialize(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			Update();
		}

		void Camera::Bake(const CameraBakingOptions& Opt)
		{
			mCameraBakingOpts = Opt;
			BakeRenderTarget(mCameraBakingOpts);
			BakePipeline(Opt);
		}

		void Camera::ResizeRenderTarget(Uint32 Width, Uint32 Height)
		{
			mCameraBakingOpts.RTWidth = Width;
			mCameraBakingOpts.RTHeight = Height;
			BakeRenderTarget(mCameraBakingOpts);

			mActiveSRB.Release();
			mActivePSO->CreateShaderResourceBinding(&mActiveSRB, true);
			mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(GetCameraRT()->mShaderRTV);
		}

		void Camera::Update()
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
			if (mPipelineDirty || ForceDirty)
			{
				bool _hashzeroed = false;

				for (auto it : mCameraEffects)
				{
					if (it.GetValue() != it._InternalEnabled)
					{
						if (_hashzeroed)
						{
							RequiredHash = 0;
							_hashzeroed = true;
						}
						RequiredHash = RequiredHash + it.GetID();
						it._InternalEnabled = it.GetValue();
					}
				}

				auto PSO_SRB = mPipeline.GetVariant(RequiredHash);

				mActivePSO = PSO_SRB.PSO;
				mActiveSRB = PSO_SRB.SRB;

				mActiveSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, 0)->Set(GetCameraRT()->mShaderRTV);
			}
		}

		void Camera::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			mCameraData.Model = modelMatrix;
			Update();
		}
		void Camera::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			mCameraData.View = viewMatrix;
			Update();

		}
		void Camera::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			Update();

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

		Graphics::RenderTarget* Camera::GetCameraRT()
		{
			return &CameraRT;
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
			mPipelineDirty = true;
		}

		void Camera::BakeRenderTarget(const CameraBakingOptions& Desc)
		{
			Graphics::RenderTargetDesc RTDesc;
			RTDesc.Width = Desc.RTWidth;
			RTDesc.Height = Desc.RTHeight;
			RTDesc.ColorTexFormat = TEX_FORMAT_RGBA16_FLOAT;

			CameraRT.Create(RTDesc);
		}

		void Camera::BakePipeline(const CameraBakingOptions& Desc)
		{
			std::vector<LayoutElement> Layout;


			Layout.push_back(LayoutElement(0, 0, 3, VT_FLOAT32, false));
			Layout.push_back(LayoutElement(1, 0, 2, VT_FLOAT32, false));
			Graphics::CompoundPipelineDesc PSOCreateInfo;


			this->mCameraEffects = Desc.mCameraEffects;
			for (auto it : mCameraEffects)
			{
				PSOCreateInfo.Switches.push_back(Graphics::PipelineSwitch(it.GetName()));
			}

			PSOCreateInfo.mVShaderPath = Desc.VS_Path;
			PSOCreateInfo.mPShaderPath = Desc.PS_Path;

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
			std::vector<ShaderResourceVariableDesc> Vars;
			Vars.push_back({ SHADER_TYPE_PIXEL, "SceneTexture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE });

			std::vector<ImmutableSamplerDesc> StaticSamplers;
			StaticSamplers.push_back({ SHADER_TYPE_PIXEL, "SceneTexture", Sam_LinearClamp });

			mPipeline.Create(PSOCreateInfo);

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