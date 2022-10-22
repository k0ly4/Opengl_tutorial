
#include "Engine.h"
// ���������

int main()
{   
    
    srand(time(0));
    glShader::init();
    Engine* engine = new Engine;
    Clock clock;
    while (window.isOpen())
    {
        // ��������� ����
        float time = clock.restart();
        engine->update(time, clock);
        engine->render();
       // camera.update();
    
    }
    delete engine;
    glShader::free();
    glfwTerminate();
    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
   
    return 0;
}

