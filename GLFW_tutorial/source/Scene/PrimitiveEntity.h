#ifndef PRIMITIVY_ENTITY_H
#define PRIMITIVY_ENTITY_H
#include"Graphic/Framebuffer.h"

class sBuffer {

    class Resource {

       public:
           const VertexBufferObject& getVBO() {
              return VBO_;
           }
           virtual void initialize() {
               VBO_.begin();
               initializeVBO();
               initializeVAO();
           }

       protected:

           VertexBufferObject VBO_;
           virtual void initializeVBO() = 0;
           virtual void initializeVAO() = 0;
    };

    class Quad2D :public Resource {

    public:
        Quad2D() {
            initialize();
        }
        const DrawBuffer& getVAO() {
            return VAO;
        }

    protected:
        DrawBuffer VAO;

        void initializeVAO();
        void initializeVBO();
    };

    class Quad :public Resource {

    public:
        Quad() {
            initialize();
        }
        const DrawBuffer& getVAO() {
            return VAO;
        }

    protected:
        DrawBuffer VAO;

        void initializeVAO();
        void initializeVBO();
    };

    class Cube : public Resource {

    public:
        Cube() {
            initialize();
        }
        const DrawBuffer& getVAO_GB() {
            return VAO_gb;
        }

        const DrawBuffer& getVAO() {
            return VAO;
        }

    protected:
        DrawBuffer VAO,VAO_gb;

        void initializeVAO();
        void initializeVBO();
    };

    class Plane :public Resource {

    public:
        Plane() {
            initialize();
        }
        const DrawBuffer& getVAO() {
            return VAO;
        }

        const DrawBuffer& getVAO_gb() {
            return VAO_gb;
        }

    protected:
        DrawBuffer VAO,VAO_gb;

        void initializeVAO();
        void initializeVBO();
    };

public:   
  
    static Quad* quad;
    static Quad2D* quad2D;
    static Cube* cube;
    static Plane* plane;

    static void init() {

        quad = new Quad;
        quad2D = new Quad2D;
        cube = new Cube;
        plane = new Plane;

    }
    static void free() {
        delete quad;
        delete quad2D;
        delete cube;
        delete plane;
    }
private:

    sBuffer() {}
    ~sBuffer() {}

};

#endif