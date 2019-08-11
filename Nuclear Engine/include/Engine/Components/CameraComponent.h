#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <Engine\Graphics\RenderTarget.h>
#include <Engine\ECS\Entity.h>
#include <Engine\Graphics\Color.h>
#include <Engine\Graphics\Skybox.h>
#include <Engine\Graphics\NeoPipeline.h>

namespace NuclearEngine
{
	namespace Components
	{
		enum CAMERA_MOVEMENT {
			CAMERA_MOVEMENT_FORWARD,
			CAMERA_MOVEMENT_BACKWARD,
			CAMERA_MOVEMENT_LEFT,
			CAMERA_MOVEMENT_RIGHT
		};

		// Default camera values
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVTY = 0.05f;
		const float ZOOM = 45.0f;

		struct CameraBakingOptions
		{
			Uint32 RTWidth = 0;
			Uint32 RTHeight = 0;

			bool Disable_HDR_Varient = false;
			bool Disable_GammaCorrection_Varient = false;
			bool Disable_Bloom_Varient = true;
		};

		struct CameraBuffer
		{
			Math::Matrix4 Model = Math::Matrix4(1.0f);
			Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
			Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

			Math::Matrix4 View = Math::Matrix4(1.0f);
			Math::Matrix4 Projection = Math::Matrix4(1.0f);
		};

		class NEAPI CameraComponent : public ECS::Component<CameraComponent>
		{
		public:
			CameraComponent();
			CameraComponent(Math::Vector3 __position, Math::Vector3 _Worldup = Math::Vector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float speed = SPEED, float sensitivity = SENSITIVTY, float Zoom = ZOOM);
			~CameraComponent();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			void Initialize(Math::Matrix4 projectionMatrix);

			void Bake(const CameraBakingOptions& Opt);
			void ResizeRenderTarget(Uint32 Width, Uint32 Height);

			void Update();

			//Call on the beginning of every frame
			void UpdatePSO(bool ForceDirty  = false);


			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			Graphics::RenderTarget* GetCameraRT();
			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

			//Postprocessing
			bool HDR = false;
			bool GammaCorrection = false;
			bool Bloom = false;

			CameraBuffer mCameraData;
			CameraBakingOptions mCameraBakingOpts;
			Graphics::Color RTClearColor;
			Graphics::Skybox* mSkybox = nullptr;
			bool RenderSkybox = false;
		protected:
			Graphics::RenderTarget CameraRT;

			Graphics::NeoPipeline mPipeline;

			RefCntAutoPtr<IPipelineState> mActivePSO;
			RefCntAutoPtr<IShaderResourceBinding> mActiveSRB;

			RefCntAutoPtr<IPipelineState> mBloomPSO;
			RefCntAutoPtr<IShaderResourceBinding> mBloomSRB;

			// Eular Angles
			float Yaw;
			float Pitch;

			Math::Vector3 Front, Up, Right, WorldUp;

			Math::Vector3 position, direction;

			bool HDR_Enabled = false;
			bool GammaCorrection_Enabled = false;
			bool Bloom_Enabled = false;
			Uint32 RequiredHash = 0;

		private:
			void BakeRenderTarget(const CameraBakingOptions& Desc);
			void BakePipeline(const CameraBakingOptions& Desc);
		};

		typedef CameraComponent Camera;
	}
}