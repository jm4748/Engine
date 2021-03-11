#pragma once


#include "..\primary\system.h"

#include "..\secondary\texture_manager.h"
#include "..\secondary\camera.h"
#include "..\secondary\model.h"


class MainState : public State
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11RasterizerState* rsWireframe;
	ID3D11RasterizerState* rsSolid;

	//-----

	TextureManager textureManager;

	Camera camera;

	ModelEffect modelEffect;
	Model model;

public:
	MainState();

	void Startup();
	void Cleanup();

	void Update( __int64 timeStep );
	void Render( float interpolation );
};