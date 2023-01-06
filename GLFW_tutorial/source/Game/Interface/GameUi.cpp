#include "GameUi.h"
void GameUi::init(Player* player_, World* world_) {
	world = world_;
	player = player_;
	font.load("asset\\font\\UbuntuMono-R.ttf");
	text.setFont(font);
	text.transform.setOrigin(beg);
	text.transform.setPosition(0.f, 0.f);
	text.setSizeFont(16);
	text.style.color = Color::WHITE;
	back.setFillColor(Color::ColorU(100,100,100,180));
	back.setSize(390.f, 180.f);
	back.transform.setOrigin(beg);
}
std::wstring to_wstring(const glm::vec3& data) {
	return +L"x:" + std::to_wstring(data.x) + L", y:" + std::to_wstring(data.y) +L", z:" + std::to_wstring(data.z);
}
std::wstring to_wstring(Voxel vox) {
	return +L"id:" + std::to_wstring(vox.e.id) + L", m1:" + std::to_wstring(vox.e.m1) + L", m2:" + std::to_wstring(vox.e.m2);
}
void GameUi::drawDebugInfo(RenderTarget& target) {
	target.draw(back);
	const Voxel* voxel = world->chunks.getVoxel(player->input.cursor.pos);
	info = L"player:\npos." +
		to_wstring(player->getBasis().position) + L"\n" +
		L"front." + to_wstring(player->getBasis().front) + L"\n" +
		L"cursor:\n" +
		L"pos." + to_wstring(player->input.cursor.pos) + L"\n" +
		L"norm." + to_wstring(player->input.cursor.norm) + L"\n"+
		L"voxel." + (voxel == 0 ? L"NuN" : to_wstring(*voxel)) + L"\n";
	text.setString(info);
	target.draw(text);

}