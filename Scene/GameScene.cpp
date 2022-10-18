﻿#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "InputManager.h"
#include "./Header/Camera.h"
#include "./Header/Parameter.h"

namespace
{
int debugTex = FUNCTION_ERROR;
}

const std::wstring GameScene::gameResourcesDir = L"./Resources/Game/";
Player* GameScene::player = Player::Get();
Stage* GameScene::stage = Stage::Get();
bool GameScene::isClear = false;

GameScene::GameScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	background(Engine::FUNCTION_ERROR),
	clear(Engine::FUNCTION_ERROR)
{
	Init();
	Reset();
}

GameScene::~GameScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void GameScene::Init()
{
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((gameResourcesDir + L"background.png").c_str());
	}
	if (clear == FUNCTION_ERROR)
	{
		clear = draw->LoadTextrue((gameResourcesDir + L"Clear.png").c_str());
	}
	if (debugTex == FUNCTION_ERROR)
	{
		debugTex = Parameter::Get("white1x1");
	}

	player->Init(draw);
	stage->StaticInit(draw);
	stage->LoadStage("./Resources/Game/Stage/test.csv");

	Camera::targetRadius = 10.0f;
	Camera::longitude = Math::DEGREE_F * (-90.0f);
	Camera::latitude = Math::DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 7.0f, -3.0f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };
}

void GameScene::Update()
{
	if (isClear)
	{
		if (InputManager::Get()->DecisionTrigger())
		{
			sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
		}
	}
	else
	{
		player->Update(InputManager::Get());
		stage->Update();

		if (Stage::IsGoal())
		{
			isClear = true;
		}
	}

#ifdef _DEBUG
	if (Input::IsKeyTrigger(DIK_R))
	{
		Reset();
	}
	if (Input::IsKeyTrigger(DIK_F1))
	{
		stage->LoadStage("./Resources/Game/Stage/test.csv");
	}
#endif // _DEBUG
}

void GameScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
	for (int y = 0; y * 128 < w->windowHeight; y++)
	{
		for (int x = 0; x * 128 < w->windowWidth; x++)
		{
			draw->DrawTextrue(
				x * 128.0f,
				y * 128.0f,
				128.0f,
				128.0f,
				0.0f,
				background,
				DirectX::XMFLOAT2(0.0f, 0.0f)
			);
		}
	}

	// 3Dオブジェクト
	stage->Draw();
	player->Draw();

	// 前景
	DirectDrawing::ChangeSpriteShader();
	if (isClear)
	{
		draw->DrawTextrue(w->windowWidth / 2.0f, w->windowHeight / 2.0f, 160.0f * 4.0f, 48.0f * 4.0f, 0.0f, clear, DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else
	{
		draw->DrawTextrue(0.0f, 0.0f, 16.0f * 10.0f, 32.0f, 0.0f, debugTex,
						  DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
		draw->DrawString(0.0f, 0.0f, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						 "KeyCount:%d", player->GetKeyCount());

		draw->DrawTextrue(0.0f, w->windowHeight - (32.0f * 1.0f), 16.0f * 18.0f, 32.0f, 0.0f, debugTex,
						  DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
		draw->DrawString(0.0f, w->windowHeight - (32.0f * (1.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						 "Move:WASD or Arrow");
	}
	draw->DrawTextrue(0.0f, w->windowHeight - (32.0f * 0.0f), 16.0f * 12.0f, 32.0f, 0.0f, debugTex,
					  DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	draw->DrawString(0.0f, w->windowHeight - (32.0f * (0.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Action:Space");

	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

void GameScene::Reset()
{
	isClear = false;
	player->Reset();
	stage->Reset();
}
