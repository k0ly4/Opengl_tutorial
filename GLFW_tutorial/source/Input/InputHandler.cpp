#include "InputHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Game/Modules/Scene.h"

void InputHandler::solveKey(Event& event) {
	switch (event.key.code)
	{

	case Keyboard::W:
		command[kW]->execute(event);
	break;

	case Keyboard::S:
		command[kS]->execute(event);
	break;

	case Keyboard::A:
		command[kA]->execute(event);
	break;

	case Keyboard::D:
		command[kD]->execute(event);
	break;

	case Keyboard::Space:
		command[kSpace]->execute(event);
	break;

	case Keyboard::U:
		command[kU]->execute(event);
	break;

	default:
	break;
	}
}

void InputHandler::solveMouse(Event& event) {
	switch (event.mouseButton.button)
	{

	case Mouse::Left:
		command[mLeft]->execute(event);
		break;
	case Mouse::Right:
		command[mRight]->execute(event);
		break;
	case Mouse::Middle:
		command[mMiddle]->execute(event);
		break;
	
	default:
		break;
	}
}

void InputHandler::solveScroll(Event& event) {
	command[mScroll]->execute(event);
}

void sCommandHandler::init(InputHandler& input, GlobalScene* scene, Player* player) {
	
	input.command[input.kA] =		new com::pl::Left(player);
	input.command[input.kD] =		new com::pl::Right(player);
	input.command[input.kS] =		new com::pl::Back(player);
	input.command[input.kW] =		new com::pl::Forward(player);
	input.command[input.kU] =		new com::UpdateMeshChunks(scene);
	input.command[input.kSpace] =	new com::pl::Jump(player);
	input.command[input.mLeft] =	new com::pl::HandDestroy(player, scene);
	input.command[input.mRight] =	new com::pl::HandCreate(player, scene);
	input.command[input.mMiddle] =	&com::Null;
	input.command[input.mScroll] =	new com::pl::SwitchBlock(player);
}