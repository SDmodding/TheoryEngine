#pragma once

int InitGameSystems();
int CloseGameSystems();

#ifdef THEORY_IMPL

int InitGameSystems()
{
	UFG::qInitParams init_params;
	UFG::qInit(&init_params);

	Illusion::gEngine.Init();

	Render::InitStateParams();

	UFG::RenderWorldState::Init();

	return 1;
}

int CloseGameSystems()
{
	UFG::qClose();
	return 1;
}

#endif