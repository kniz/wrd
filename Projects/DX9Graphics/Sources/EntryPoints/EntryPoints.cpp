#include "Headers.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

namespace NE
{
	NEModuleList _declspec(dllexport) _stdcall _EXECUTE_ENTRYPOINT
	{	
		NEModuleList modulelist;

		modulelist.push(	DX9Graphics::AngleDirectionConverter()	);

		modulelist.push(	DX9Graphics::BufferClearer()			);

		modulelist.push(	DX9Graphics::DX9()						);

		modulelist.push(	DX9Graphics::PerspectiveCamera()		);
		modulelist.push(	DX9Graphics::AnimatedModel()			);	
		modulelist.push(	DX9Graphics::SpriteTexter()				);	

		modulelist.push(	DX9Graphics::ModelMediator()			);
		modulelist.push(	DX9Graphics::ParticleObserver()			);
		modulelist.push(	DX9Graphics::ParticleVelocity()			);
		modulelist.push(	DX9Graphics::Picker());

		modulelist.push(	DX9Graphics::TabledTexture()			);
		modulelist.push(	DX9Graphics::Sprite()					);

		modulelist.push(	DX9Graphics::DXFont()					);

		modulelist.push(	DX9Graphics::BloomProgram()				);
		modulelist.push(	DX9Graphics::BlurProgram()				);
		modulelist.push(	DX9Graphics::FixedProgram()				);

		return modulelist;
	}
}