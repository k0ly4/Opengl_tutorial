#include "Command.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Scene/Scene.h"
namespace com {

	void LightSeter::execute(Event& event) {

		if (event.type == Event::MouseButtonPressed) {
			scene->light.getDirs().setDirection(-scene->camera.getBasis().front);
			scene->light.getPoints()[0].setPosition(scene->camera.getPosition());
			scene->light.lightTest.setDirection(scene->camera);
		}

	}

	namespace pl {

		void Jump::execute(Event& event) {
			player->input.jump();
		}

		void Forward::execute(Event& event) {

		}

		void Left::execute(Event& event) {
		
		}

		void Right::execute(Event& event) {
		}


		void Back::execute(Event& event) {
		
		}

		void HandCreate::execute(Event& event) {
			if (event.type == Event::MouseButtonPressed) {
				player->input.setVoxel(scene->world, 1);
			}
		}

		void HandDestroy::execute(Event& event) {
			if (event.type == Event::MouseButtonPressed) {
				player->input.setVoxel(scene->world, 0);
			}
		}

	}
}