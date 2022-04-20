mkdir Bin\DebugX64
mkdir Bin\ReleaseX64

::FreeImage              (We dont use debug version for performance)
copy External\Freeimage\Dist\x64\FreeImage.dll	 Bin\DebugX64
copy External\Freeimage\Dist\x64\FreeImage.lib	 Bin\DebugX64
copy External\Freeimage\Dist\x64\FreeImage.dll	 Bin\ReleaseX64
copy External\Freeimage\Dist\x64\FreeImage.lib	 Bin\ReleaseX64

::Assimp (We dont use debug version for performance)
copy External\CmakeProjects\Assimp\bin\Release\assimp-vc143-mt.dll	 Bin\DebugX64
copy External\CmakeProjects\Assimp\lib\Release\assimp-vc143-mt.lib	 Bin\DebugX64

copy External\CmakeProjects\Assimp\bin\Release\assimp-vc143-mt.dll	 Bin\ReleaseX64
copy External\CmakeProjects\Assimp\lib\Release\assimp-vc143-mt.lib	 Bin\ReleaseX64

::FMod
copy External\FMOD\lib\x64\fmodL.dll	 Bin\DebugX64
copy External\FMOD\lib\x64\fmodL_vc.lib	 Bin\DebugX64

copy External\FMOD\lib\x64\fmodL.dll	 Bin\ReleaseX64
copy External\FMOD\lib\x64\fmodL_vc.lib	 Bin\ReleaseX64

::GLFW Debug
copy External\CmakeProjects\GLFW\src\Debug\glfw3.lib	 Bin\DebugX64
::GLFW Release
copy External\CmakeProjects\GLFW\src\Release\glfw3.lib	 Bin\ReleaseX64

::PhysX Debug
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysX_64.dll	 Bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXCommon_64.dll	 Bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXFoundation_64.dll	 Bin\DebugX64

copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysX_64.lib	 Bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXCommon_64.lib bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXFoundation_64.lib	 Bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXExtensions_static_64.lib	 Bin\DebugX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\debug\PhysXPvdSDK_static_64.lib	 Bin\DebugX64

::PhysX Release
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysX_64.dll	         Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXCommon_64.dll	     Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXFoundation_64.dll	 Bin\ReleaseX64

copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysX_64.lib	               Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXCommon_64.lib             Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXFoundation_64.lib	       Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXExtensions_static_64.lib  Bin\ReleaseX64
copy External\PhysX\physx\bin\win.x86_64.vc142.mt\release\PhysXPvdSDK_static_64.lib	   Bin\ReleaseX64

::Diligent Debug
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D11\Debug\GraphicsEngineD3D11_64d.lib          	 Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D12\Debug\GraphicsEngineD3D12_64d.lib          	 Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineOpenGL\Debug\GraphicsEngineOpenGL_64d.lib          Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineVulkan\Debug\GraphicsEngineVk_64d.lib          	 Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsAccessories\Debug\Diligent-GraphicsAccessories.lib                Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsTools\Debug\Diligent-GraphicsTools.lib            			     Bin\DebugX64
copy External\CmakeProjects\Diligent\Platforms\Basic\Debug\Diligent-BasicPlatform.lib      					         Bin\DebugX64
copy External\CmakeProjects\Diligent\Platforms\Win32\Debug\Diligent-Win32Platform.lib      					         Bin\DebugX64
copy External\CmakeProjects\Diligent\Common\Debug\Diligent-Common.lib            									     Bin\DebugX64

copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D11\Debug\GraphicsEngineD3D11_64d.dll          	 Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D12\Debug\GraphicsEngineD3D12_64d.dll          	 Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineOpenGL\Debug\GraphicsEngineOpenGL_64d.dll            Bin\DebugX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineVulkan\Debug\GraphicsEngineVk_64d.dll          	     Bin\DebugX64

::Diligent Release
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D11\Release\GraphicsEngineD3D11_64r.lib          	 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D12\Release\GraphicsEngineD3D12_64r.lib          	 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineOpenGL\Release\GraphicsEngineOpenGL_64r.lib          Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineVulkan\Release\GraphicsEngineVk_64r.lib          	 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsAccessories\Release\Diligent-GraphicsAccessories.lib                Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsTools\Release\Diligent-GraphicsTools.lib            				 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Platforms\Basic\Release\Diligent-BasicPlatform.lib      					         Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Platforms\Win32\Release\Diligent-Win32Platform.lib      					         Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Common\Release\Diligent-Common.lib            									     Bin\ReleaseX64

copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D11\Release\GraphicsEngineD3D11_64r.dll          	 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineD3D12\Release\GraphicsEngineD3D12_64r.dll          	 Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineOpenGL\Release\GraphicsEngineOpenGL_64r.dll          Bin\ReleaseX64
copy External\CmakeProjects\Diligent\Graphics\GraphicsEngineVulkan\Release\GraphicsEngineVk_64r.dll 		     Bin\ReleaseX64

pause
exit 0