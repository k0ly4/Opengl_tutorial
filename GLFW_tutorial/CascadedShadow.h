
#ifndef CASCADE_SHADOW_H
#define CASCADE_SHADOW_H
#include "ShadowFrameBuffer.h"

#define NUM_CASCADES 3
class CascadeViews {
    void getViewCorners(size_t i, float tanFovHorizontal, float tanFovVertical, glm::vec4* corner) {
        float xn = m_cascadeEnd[i] * tanFovHorizontal;
        float xf = m_cascadeEnd[i + 1] * tanFovHorizontal;
        float yn = m_cascadeEnd[i] * tanFovVertical;
        float yf = m_cascadeEnd[i + 1] * tanFovVertical;

        // Ѕлижн€€ плоскость
        corner[0] = glm::vec4(xn, yn, m_cascadeEnd[i], 1.0);
        corner[1] = glm::vec4(-xn, yn, m_cascadeEnd[i], 1.0);
        corner[2] = glm::vec4(xn, -yn, m_cascadeEnd[i], 1.0);
        corner[3] = glm::vec4(-xn, -yn, m_cascadeEnd[i], 1.0);
        // ƒальн€€ плоскость
        corner[4] = glm::vec4(xf, yf, m_cascadeEnd[i + 1], 1.0);
        corner[5] = glm::vec4(-xf, yf, m_cascadeEnd[i + 1], 1.0);
        corner[6] = glm::vec4(xf, -yf, m_cascadeEnd[i + 1], 1.0);
        corner[7] = glm::vec4(-xf, -yf, m_cascadeEnd[i + 1], 1.0);
    }
    void initBox(Box& box) {
        box.left = std::numeric_limits<float>::max();
        box.right = std::numeric_limits<float>::min();
        box.bottom = std::numeric_limits<float>::max();
        box.top = std::numeric_limits<float>::min();
        box.near = std::numeric_limits<float>::max();
        box.far = std::numeric_limits<float>::min();
    }
    void getProjection(Box& box, glm::vec4* viewCorner, const glm::mat4& from_view_space, const glm::mat4& to_light_space) {

        for (size_t j = 0; j < NUM_FRUSTUM_CORNERS; j++) {
            // ѕреобразуем координаты усеченоой пирамиды из пространства камеры в мировое пространство
            glm::vec4 vW = from_view_space * viewCorner[j];
            // » ещЄ раз из мирового в пространство света
            glm::vec4 lightCorner = to_light_space * vW;

            box.left = std::min(box.left, lightCorner.x);
            box.right = std::max(box.right, lightCorner.x);
            box.bottom = std::min(box.bottom, lightCorner.y);
            box.top = std::max(box.top, lightCorner.y);
            box.near = std::min(box.near, lightCorner.z);
            box.far = std::max(box.far, lightCorner.z);
        }
    }
    static const size_t NUM_FRUSTUM_CORNERS = 8;
public:
    float endClipZ[NUM_CASCADES];
    void setDirection(const glm::vec3& direction){
        for (size_t i = 0; i < NUM_CASCADES;i++) {
            view[i].setPosition(glm::vec3(0.f));
            view[i].Transform().look(-direction);
        }
    }
    void calcViewsMatrix(View& view_player) {
        // ѕолучаем обратные преобразовани€
        glm::mat4 cam = glm::lookAt(view_player.getPosition(), view_player.getPosition()+view_player.Transform().Basis().front, GAME::WORLD_UP);
        //glm::mat4 CamInv = view_player.Transform().Inverse();// glm::inverse(cam);
        glm::mat4 CamInv =  glm::inverse(cam);
        // ѕолучаем преобразовани€ света
        float ar = 1.f / view_player.getProjection().Setup().persp.ratio;
        float tanFovHorizontal = tanf(glm::radians(view_player.getProjection().Setup().persp.fov / 2.0f));
        float tanFovVertical = tanf(glm::radians((view_player.getProjection().Setup().persp.fov * ar) / 2.0f));

        for (size_t i = 0; i < NUM_CASCADES; i++) {

            glm::vec4 corner_in_view[NUM_FRUSTUM_CORNERS];
            getViewCorners(i, tanFovHorizontal, tanFovVertical, corner_in_view);

            Box box;
            initBox(box);
            getProjection(box, corner_in_view, CamInv, view[i].Transform().Matrix());
           
            //view[i].setProjection(Box(box.right,box.left,box.bottom,box.top,box.far,box.near));
            view[i].setProjection(box);
            std::cout << "Box:" << box<<'\n';
            glm::vec4 vView(0.0f, 0.0f, m_cascadeEnd[i + 1], 1.0f);
            glm::vec4 vClip = view_player.getProjection().Matrix() *vView;
            endClipZ[i] = vClip.z;
        }
    }
    void setCascadeEnds(const float *ends) {
        for (size_t i = 0; i < NUM_CASCADES; i++)
            m_cascadeEnd[i] = ends[i];
    }
    View3D& operator[](size_t index) {
        return view[index];
    }
private:
    float m_cascadeEnd[NUM_CASCADES + 1];
    View3D view[NUM_CASCADES];
};
class CascadeShadow  {
    RenderCascadedDepth fbo;
    CascadeViews views;
    struct DirLight {
        glm::vec3 direction;
    }light;
public:
    CascadeShadow() {
        fbo.create(NUM_CASCADES, 1024, 1024);    
    }
    Texture2D getTexture(size_t i) {
        return fbo.getTexture(i);
    }
    void create(View& view_player,const glm::vec3& direction) {
        light.direction = direction;

        float casc_ends[NUM_CASCADES + 1] = 
        { view_player.Projection().Setup().persp.near , 25.0f, 90.0f,view_player.Projection().Setup().persp.far };
        views.setCascadeEnds(casc_ends);
        views.setDirection(direction);
      }
    void setDirection(const glm::vec3& direction) {
        light.direction = direction;
        views.setDirection(direction);
    }
    const View& getView(View& view_player, size_t index_view) {
        views.calcViewsMatrix(view_player);
        return views[index_view];
    }
    void bind(size_t begin,const Shader& shader,const std::string& name) {       
        for (size_t i = 0; i < NUM_CASCADES; i++) {
            shader.uniform(name + ".cascadeEndClipSpace[" + std::to_string(i) + "]", views.endClipZ[i]);
            shader.uniform(name + ".spaceMatrix["+std::to_string(i)+"]", views[i].getVP());
            fbo.getTexture(i).use(begin + i);
        }
        
    }
    void bind(size_t begin,size_t index, const Shader& shader, const std::string& name) {   
            shader.uniform(name + ".spaceMatrix", views[index].getVP());
            fbo.getTexture(index).use(begin);
    }  
    void ShadowMapPass( View&view_player, void (*render_scene) (RenderTarget& target))
    {
        views.calcViewsMatrix(view_player);
        //  амера помещаетс€ на позицию источника света и не мен€ет на протежении этого этапа
   //     p.SetCamera(Vector3f(0.0f, 0.0f, 0.0f), m_dirLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));

        for (size_t i = 0; i < NUM_CASCADES; i++) {
            // ѕрив€зываем и очищаем текущий каскад
            fbo.bind_for_writing(i);
            glClear(GL_DEPTH_BUFFER_BIT);
            fbo.setView(views[i]);
            render_scene(fbo);
        }
    }
    void ShadowMapPass(View& view_player, RenderClass*render)
    {
        views.calcViewsMatrix(view_player);
        //  амера помещаетс€ на позицию источника света и не мен€ет на протежении этого этапа
   //     p.SetCamera(Vector3f(0.0f, 0.0f, 0.0f), m_dirLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));

        for (size_t i = 0; i < NUM_CASCADES; i++) {
            // ѕрив€зываем и очищаем текущий каскад
            fbo.bind_for_writing(i);
            glClear(GL_DEPTH_BUFFER_BIT);
            fbo.setView(views[i]);
            render->drawScene_shadow(fbo);
        }
    }
};
#endif
