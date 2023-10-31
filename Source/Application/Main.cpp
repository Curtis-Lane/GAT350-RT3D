#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

#include "World05.h"

int main(int argc, char* argv[])
{
	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");
	
	std::cout << "\n" << std::endl;

	std::cout << nc::StringUtils::ToUpper("aaaaaaaa") << std::endl;
	std::cout << nc::StringUtils::ToUpper("AAAAAAAA") << std::endl;

	std::cout << "\n" << std::endl;

	std::cout << nc::StringUtils::ToLower("AAAAAAAA") << std::endl;
	std::cout << nc::StringUtils::ToLower("aaaaaaaa") << std::endl;

	std::cout << "\n" << std::endl;

	// True
	std::cout << nc::StringUtils::IsEqualIgnoreCase("AaaaAAaA", "aaaaaaaa") << std::endl;
	// True
	std::cout << nc::StringUtils::IsEqualIgnoreCase("AaaaAAaA", "AAAAAaaa") << std::endl;
	// False
	std::cout << nc::StringUtils::IsEqualIgnoreCase("AaaaAAaA", "AAAWWaaa") << std::endl;

	std::cout << "\n" << std::endl;

	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;
	std::cout << nc::StringUtils::CreateUnique("AAAAAAA") << std::endl;

	/* ENGINE.Initialize();

	auto world = std::make_unique<nc::World05>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown(); */

	return 0;
}
