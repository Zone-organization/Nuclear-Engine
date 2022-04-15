#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine\Graphics\RenderTarget.h>
#include <Engine\Graphics\Color.h>
#include <Engine\Graphics\CompoundPipeline.h>
#include <Engine\Rendering\ShaderEffect.h>
#include <Engine\Rendering\Skybox.h>
#include "Engine/Rendering/BlurEffect.h"

namespace Nuclear
{
	namespace Systems
	{
		class CameraSubSystem;
	}

	namespace Graphics
	{
		enum CAMERA_MOVEMENT {
			CAMERA_MOVEMENT_FORWARD,
			CAMERA_MOVEMENT_BACKWARD,
			CAMERA_MOVEMENT_LEFT,
			CAMERA_MOVEMENT_RIGHT
		};

		struct CameraBuffer
		{
			Math::Matrix4 Model = Math::Matrix4(1.0f);
			Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
			Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

			Math::Matrix4 View = Math::Matrix4(1.0f);
			Math::Matrix4 Projection = Math::Matrix4(1.0f);
		};

		// Default camera values
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVTY = 0.05f;
		const float ZOOM = 45.0f;

		/*
		*Only Supports
		* HDR
		* GAMMACORRECTION
		* BLOOM
		*/
		class NEAPI Camera
		{
		public:
			Camera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw , float pitch , float speed , float sensitivity , float Zoom );
			~Camera();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			void Initialize(Math::Matrix4 projectionMatrix);

			//void Bake(Uint32 RTWidth, Uint32 RTHeight);
			//void ResizeRenderTarget(Uint32 Width, Uint32 Height);

			void UpdateBuffer();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			/*Graphics::RenderTarget* GetSceneRT();
			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();*/

			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

	/*		void SetEffect(const Uint32& effectId, bool value);

			void SetForRender();

			void SetForScreenRender(Systems::CameraSubSystem* sys);

			std::unordered_map<Uint32, Rendering::ShaderEffect> mCameraEffects;*/

			CameraBuffer mCameraData;
		
			Graphics::Color RTClearColor;
			Rendering::Skybox* mSkybox = nullptr;
			bool RenderSkybox = false;
		protected:
			/*Graphics::RenderTarget SceneRT;
			Graphics::RenderTarget BloomRT;

			Graphics::CompoundPipeline mPipeline;

			RefCntAutoPtr<IPipelineState> mActivePSO;
			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;

			Rendering::BlurEffect mBloomBlur;

			bool mPipelineDirty = true;*/

			// Eular Angles
			float Yaw;
			float Pitch;

			Math::Vector3 Front, Up, Right, WorldUp;

			Math::Vector3 position, direction;

			//Uint32 RequiredHash = 0;

			//std::string VS_Path = "Assets/NuclearEngine/Shaders/Camera.vs.hlsl";
			//std::string PS_Path = "Assets/NuclearEngine/Shaders/Camera.ps.hlsl";

			//Uint32 RTWidth = 0;
			//Uint32 RTHeight = 0;
		private:
			//void BakeRenderTarget();
			//void BakePipeline();
		};
	}
}