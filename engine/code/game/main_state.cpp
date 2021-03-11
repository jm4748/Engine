#include "states.h"


MainState::MainState()
{
	device = NULL;
	context = NULL;
}


void MainState::Startup()
{
	device = graphics->GetDevice();
	context = graphics->GetContext();
	
	D3D11_RASTERIZER_DESC rasterizerDesc;
	
	rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode              = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias             = 0;
	rasterizerDesc.DepthBiasClamp        = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias  = 0.0f;
	rasterizerDesc.DepthClipEnable       = TRUE;
	rasterizerDesc.ScissorEnable         = FALSE;
	rasterizerDesc.MultisampleEnable     = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	
	device->CreateRasterizerState( &rasterizerDesc, &rsWireframe );

	rasterizerDesc.FillMode              = D3D11_FILL_SOLID;

	device->CreateRasterizerState( &rasterizerDesc, &rsSolid );

	context->RSSetState( rsSolid );

	//----------

	textureManager.Startup( graphics );

	camera.SetLens( 70.0f, 800.0f / 600.0f, 0.5f, 10'000.0f );

	XMFLOAT3 startingPosition( 0.0f, 0.0f, -10.0f );
	camera.SetPosition( &startingPosition );

	modelEffect.Initialize( graphics, &textureManager );

	model.Load( graphics, "resources\\ninja.mdl" );
}

void MainState::Cleanup()
{
	model.Unload( graphics );

	modelEffect.Release();

	textureManager.Cleanup();
}


void MainState::Update( __int64 timeStep )
{
	float timeFactor = ( float )( ( double )timeStep / 1'000'000.0 );

	//-----

	if ( input->KeyDown( Key::K_ESCAPE ) ) system->Quit();
	if ( core->WindowClosed() ) system->Quit();

	XMVECTOR dir = XMVectorZero();

	XMFLOAT3 look, right;

	camera.GetLook( 1.0f, &look );
	camera.GetRight( 1.0f, &right );

	XMVECTOR _look, _right;

	_look = XMLoadFloat3( &look );
	_right = XMLoadFloat3( &right );

	if ( input->KeyDown( K_W ) )
	{
		dir += +_look;
	}
	if ( input->KeyDown( K_S ) )
	{
		dir += -_look;
	}
	if ( input->KeyDown( K_A ) )
	{
		dir += -_right;
	}
	if ( input->KeyDown( K_D ) )
	{
		dir += +_right;
	}

	XMFLOAT3 deltaPosition = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMStoreFloat3( &deltaPosition, XMVector3Normalize( dir ) * 50.0f * timeFactor );
	camera.Update( &deltaPosition,
	               input->MousePositionDeltaX() * 0.2f,
	               input->MousePositionDeltaY() * 0.2f );
}

void MainState::Render( float interpolation )
{
	XMFLOAT4X4 vp;
	camera.GetViewProjection( interpolation, &vp );

	XMFLOAT4X4 m;
	XMStoreFloat4x4( &m, XMMatrixTranslation( 0.0f, 0.0f, 0.0f ) );

	//-----

	graphics->Clear( 1.0f, 1.0f, 1.0f, 0.0f );

	modelEffect.Apply();
	modelEffect.SetVP( &vp );
	modelEffect.SetModel( &model );
	modelEffect.SetW( &m );
	modelEffect.Draw();

	graphics->Present();
}