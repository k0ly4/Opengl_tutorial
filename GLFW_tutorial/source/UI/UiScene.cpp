#include "UiScene.h"
#include "Scene/Scene3D.h"

void UiScene::init(RenderWindow& window) {
	view2D.setProjection(FloatRect(0.f, 0.f, window.getSize()));
	//view2D.setProjection(Box(0.f, window.getSize().x, 0.f, window.getSize().y, -1.f, 1.f));
	texCowBoy.getPath("asset\\image\\The fastest camp in the west.png");
	texError.getPath("asset\\image\\FUrfZuZXoAEnZBE.jpg");

	texError.wrap(TextureWrap::ClampToEdge);

	gameUi.init(&main->player,&main->world);
}
void UiScene::inRender(RenderTarget& target) {
	target.setView(view2D);
	setContext();
	
	main->player.inventory.draw(target);
	gameUi.draw(target);
}