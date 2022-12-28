#include "Command.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Scene/Scene.h"
namespace com {

	
	void UpdateMeshChunks::execute(Event& event) {
		ChunkPtrs& chunks = scene->world.chunks.chunks();
		for (size_t i = 0; i < chunks.size(); i++) chunks[i]->flag.modify();
	}
	namespace pl {
		void SwitchBlock::execute(Event& event) {
			if (event.mouseScroll.yOffset > 0) {
				if (player->input.curVoxel.e.id < vox::size-1)
				player->input.curVoxel.e.id++;
			}
			else if(player->input.curVoxel.e.id >0)
				player->input.curVoxel.e.id--;

		}
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