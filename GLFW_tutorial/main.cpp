
#include"Camera.h"
#include "Mesh.h"
#include"Text.h"
#include "Primitives.h"
#include "Shapes.h"
#include "RenderWindow.h"
#include<list>
#include"Sprite.h"
#include "Terrain.h"
#include"LightManager.h"
// ���������
struct TempVar {
    size_t cur_animation = 0;
    bool need_shadow_update = 0;
    int shadow_level = 0;
    // b_Model model;
    bool shadow_view = 0;
    int cur_bone = 0;
    int mode_bone = 0;

};

 class Engine:public RenderClass {
     void setupWindow() {
         window.create(GAME::RENDER_SIZE, u8"���� ������");
        
         camera.setProjection((float)GAME::RENDER_SIZE.x / (float)GAME::RENDER_SIZE.y, 90.f);

         camera.cur_pos_mouse = Mouse::getPosition();
         window.setView(camera);
       
             stbi_set_flip_vertically_on_load(false);
             Image image("asset\\image\\favor.jpg");
             window.setIcon(1, &image);
 
         window.setFrameInterval(1);
         window.setLimitSizeable(true, glm::ivec2(GLFW_DONT_CARE), glm::ivec2(GLFW_DONT_CARE));
         GlRender::setClearColor(Color::ColorU(150, 199, 196));
        
     }
     Event event;
     Texture2D filin;
     Camera camera;
     Primitive::Cube cube;
     bool rawMotionCursor = 0;
     Terrain terrain;
     bool checkCollision = 1;
     Texture2D texture_terrain;
     Texture2D texture_grass;

    // TerrainGreeneryModel greenize;
     TerrainGreenery grass_in;
     Grass grass;
     Brush brush;
    
     FrameBuffer frame;
     FrameBuffer gBuffer;
     Primitive::Filter filter;
     LightSystem light;
     Primitive::Skybox skybox;
     TempVar t;
 public:
     Engine() {
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
         GlRender::DepthTest(true);
        // GlRender::Blend(true);
       //  glEnable(GL_ALPHA_TEST);
       //  glAlphaFunc(GL_GREATER, 0.8);
       // GlRender::Blend(true);
       
         setupWindow();
   
        //  model.load("asset\\obj\\characters\\redguard.fbx");
         //model.load("asset\\obj\\model\\untitled.fbx",1,1); //meshes: 14, vertices : 2534, indices : 3906, bones : 546
        // model.setScale(glm::vec3(0.5f));
         filin.loadFromFile("asset\\image\\favor.jpg",1,1,1);
         
         cube.setTexture(filin);
         cube.setPosition(glm::vec3(1.f, 1.f, 1.f));
         cube.setRotate(Angle3D(50.f, glm::vec3(1.f, 0.f, 0.f)));
         light.add(P_Light(glm::vec3(1.f), glm::vec3(25.f, 10.f, 25.f),glm::vec2(0.032f, 0.09f)));
         light.global() = D_Light(glm::vec3(0.9f), glm::vec3(1.f, 1.f, 0.f),camera.getPosition());
         light.global().createMap(1024, 1024,camera);
         light.setAmbientFactor(0.05f);
        
        texture_terrain.loadFromFile("asset\\image\\image.jpg",1,1,1);     
        texture_terrain.wrap(GL_REPEAT);
        texture_terrain.filter(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
        terrain.generate(glm::uvec2(50), "asset\\image\\consider_this_question.jpg");
        terrain.setPosition(glm::vec3(0, -10.f, 0));
        terrain.setTexture(texture_terrain);
       
        texture_grass.filter(GL_NEAREST);
        texture_grass.loadFromFile("asset\\image\\grass.png",1,1,1);
        
      //  model.load("asset/tree.obj");
        //model.setScale(glm::vec3(0.1f));

        grass.setTexture(texture_grass);
        grass.setPosition(glm::vec3(0.f, 0.5f, 0.f));
      //  model.setScale(glm::vec3(0.15f));
        //greenize.setObject(model, glShader::m_light_uniform_color_instance);
        //greenize.create(terrain, 70);

        grass_in.setObject(&grass, glShader::m_texture_instance);
        grass_in.create(terrain, 5000);
        brush.setTerrain(&terrain);

        
        frame.addTexture(GL_RGBA16F, GL_RGBA, GL_NEAREST);
        frame.create(window.getSize());

        gBuffer.addTexture(GL_RGBA16F, GL_RGBA, GL_NEAREST);
        gBuffer.addTexture(GL_RGBA16F, GL_RGBA, GL_NEAREST);
        gBuffer.addTexture(GL_RGBA, GL_RGBA, GL_NEAREST);
        gBuffer.create(window.getSize());

        filter.setExposure(2.2f);
       
     }
     void move_camera(float time) {
         const float speed = 10.f;
         glm::vec3 vec_move(0.f);

         if (Keyboard::getKey(GLFW_KEY_W).action) {
             vec_move.z = 1;
         }
         else if (Keyboard::getKey(GLFW_KEY_S).action) {
                 vec_move.z = -1;
             }
         if (Keyboard::getKey(GLFW_KEY_D).action) {
             vec_move.x = 1;
         }
         else if (Keyboard::getKey(GLFW_KEY_A).action) {
             vec_move.x = -1;
         }
         
       //  camera.move(glm::vec3(vec_move.x * time * speed, 0.f, vec_move.z * time * speed));
             float s =  sqrt(abs(vec_move.z) + abs(vec_move.x));
             if (s > 0.f) {
                 s = (speed * time) / s;
                 camera.move(glm::vec3(vec_move.x * s, 0.f, vec_move.z * s));
                 if (checkCollision) {
                     float posY;
                     if (terrain.getHeight(camera.getPosition().x, camera.getPosition().z, posY))
                         camera.setPosition(camera.getPosition().x, posY + 1.5f, camera.getPosition().z);
                 }
             }

     }
     void update(float time, Clock& clock) {
         while (window.pollEvent(event))
         {
             if (event.action == Event::KEY_PRESSED) {
                 if (event.key == GLFW_KEY_ESCAPE) window.close();
                 else if (event.key == GLFW_KEY_LEFT_CONTROL) {
                     rawMotionCursor = !rawMotionCursor;
                     Mouse::setRawMotion(rawMotionCursor);
                 }
                 else if (event.key == GLFW_KEY_LEFT_SHIFT) {
                     checkCollision = !checkCollision;
                 }
                 else if (event.key == GLFW_KEY_R) {
                     grass_in.create(terrain, 5000);
                 }
                 else if (event.key == GLFW_KEY_F) {
                     t.shadow_view = !t.shadow_view;
                 }
                 else if (event.key == GLFW_KEY_T) {
                
                     light.shadow_demonstration = ~(light.shadow_demonstration);
                 }
                 else if (event.key == GLFW_KEY_N) {
                     t.shadow_level ++ ;
                     if (t.shadow_level > 2)t.shadow_level = 2;
                     printf("Shadow_Level:%d\n", t.shadow_level);
                 }
                 else if (event.key == GLFW_KEY_M) {
                     t.shadow_level--;
                     if (t.shadow_level < 0)t.shadow_level = 0;
                     printf("Shadow_Level:%d\n", t.shadow_level);
                 }
                 /*else if (event.key == GLFW_KEY_V) {         
                     cur_bone++;
                     model.setBoneID(cur_bone);
                 }
                 else if (event.key == GLFW_KEY_C) {                  
                     cur_bone--;
                     if (cur_bone < 0)cur_bone = 0;
                     model.setBoneID(cur_bone);
                 }
                 else if (event.key == GLFW_KEY_B) {                   
                     mode_bone = ~mode_bone;
                     model.setBoneMode(mode_bone);
                 }
                 else if (event.key == GLFW_KEY_G) {
                     cur_animation++;
                     model.Animation().switchAnimation(cur_animation);
                 }
                 else if (event.key == GLFW_KEY_H) {
                     if (cur_animation !=0 ) {
                         cur_animation--;
                         model.Animation().switchAnimation(cur_animation);
                     }
                 }*/
             }
             else if (event.action == Event::MOUSE_BUTTON_PRESSED) {
                 if (event.key == GLFW_MOUSE_BUTTON_RIGHT) {
                     camera.cur_pos_mouse = Mouse::getPosition();
                 }
                 else if (event.key == GLFW_MOUSE_BUTTON_MIDDLE) {
                     light.global().setOrient(camera);
                    
                     t.need_shadow_update = 1;
                 }
                 else if (event.key == GLFW_MOUSE_BUTTON_LEFT) {
                     light.global().setDirection(-camera.Transform().Basis().front);

                    
                 }
                
             }
            
             else if (event.action == Event::MOUSE_SCROLL) {
                 if (brush.getMode() == Brush::UP) {
                     brush.setColor(glm::vec3(1.f, 0.f, 0.f));
                     brush.setMode(Brush::DOWN);
                 }
                 else {
                     brush.setColor(glm::vec3(0.f, 0.f, 1.f));
                     brush.setMode(Brush::UP);
                 }
             }
             else if (event.action == Event::WINDOW_RESIZE) {
                 gBuffer.create(window.getSize());
                 frame.create(window.getSize());
             }

         }


         if (Keyboard::getKey(GLFW_KEY_LEFT_SHIFT).action) {
             cube.Rotate(time * 50.f);
         }
         else if (Keyboard::getKey(GLFW_KEY_LEFT_ALT).action) {
             cube.Scale(glm::vec3(time));
         }
         else if (Keyboard::getKey(GLFW_KEY_RIGHT_ALT).action) {
             cube.Scale(glm::vec3(-time));
         }

         if (Mouse::isAction(GLFW_MOUSE_BUTTON_RIGHT, Key::press)) {
             Cursor::setMode(GLFW_CURSOR_DISABLED);
             camera.mouse_move(Mouse::getPosition());
         }
         move_camera(time);
         
             //model.tick(time);
     }
     void render(float time) {
         //shadow
         //if (need_shadow_update) {
             //light.global().cascaded_shadow_maps.ShadowMapPass(camera, this);
             light.global().render(this);
             
         //    glClear(GL_DEPTH_BUFFER_BIT);
         //   // GlRender::CullFace::Enable(true);
         //   
         //   // GlRender::CullFace::Mode(GL_FRONT);
         //    drawScene_shadow();
         //   // GlRender::CullFace::Mode(GL_BACK);
         //   
         //    need_shadow_update = 0;
         //}
         //gbuffer
         GlRender::setClearColor(Color(0.4f));
         GlRender::bind(gBuffer);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
         drawScene_gBuffer();
         //scene
         GlRender::bind(frame);
         glClear(GL_COLOR_BUFFER_BIT);
        
         light.draw(gBuffer, camera,t.shadow_level);        
         gBuffer.implementDepth(frame);
        
         if (!Mouse::isAction(GLFW_MOUSE_BUTTON_RIGHT, Key::press)) {
             Cursor::setMode(GLFW_CURSOR_NORMAL);
             brush.upPosition(camera);
             if (Mouse::isAction(GLFW_MOUSE_BUTTON_LEFT, Key::press)) {
                 const float strength = 5.f;
                 grass_in.upArray(brush.paint(time * strength));
             }
         }
        // window.draw(model.getSkellet());
       
         window.draw(brush);
         window.draw(light);

         //exposure
         GlRender::unbind();
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         if(t.shadow_view)
             filter.displayRed(light.global().getMap(0));
         else 
             filter.drawExposure(frame.getTexture());
        
         window.display();
     }
     private:
         void drawScene_shadow() {
             window.drawDepth(terrain);
         }
         void drawScene_shadow(RenderTarget& target) {
             target.drawDepth(terrain);
         }
         void drawScene_gBuffer() {
             window.draw(terrain);
           //  window.draw(model);
           
         }
 };
int main()
{   
    
    srand(time(0));
    glShader::init();
    Primitive::init();
    Engine* engine = new Engine;
    Clock clock;
    while (window.isOpen())
    {
        // ��������� ����
        float time = clock.restart();
        engine->update(time, clock);
        engine->render(time);
       // camera.update();
    
    }
    delete engine;
    glShader::free();
    glfwTerminate();
    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
   
    return 0;
}

