#include "GameUi.h"
void GameUi::init(Player* player_) {
	player = player_;
	font.load("asset\\font\\UbuntuMono-R.ttf");
	text.setFont(font);
	text.transform.setOrigin(beg);
	text.transform.setPosition(0.f, 0.f);
	text.setSizeFont(25);
	text.style.color = Color::WHITE;
	//text.transform.setScale(2.f, 2.f);

	text.setString(L"Это техническая информация\nЧитай без запинки\nУрод\nПаршивая сволочь\n!!!");

}
void GameUi::drawDebugInfo(RenderTarget& target) {
	target.draw(text);
}