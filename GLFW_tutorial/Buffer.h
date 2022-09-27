#ifndef BUFFER_H
#define BUFFER_H

#include "Math.h"
#include <vector>

/// <summary>
/// GeneralBuffer
/// </summary>
class GeneralBuffer {
protected:

    friend class GlBuffer;

    class g_buffer {
        unsigned int ID;
    public:
        g_buffer() {
            glGenBuffers(1, &ID);
        }
        unsigned int operator *() {
            return ID;
        }
        ~g_buffer() {
            glDeleteBuffers(1, &ID);
        }
    };

    std::shared_ptr<g_buffer> ID;
    size_t cur_size = 0;
};

/// <summary>
/// GlBuffer
/// </summary>
class GlBuffer {

public:

    static  void resize(GeneralBuffer& buffer, size_t new_size, GLenum mode = GL_STATIC_DRAW);
    static void copy(GeneralBuffer& read, GeneralBuffer& write);
    static void copy(GeneralBuffer& read, int readOffset, GeneralBuffer& write, int writeOffset, size_t size);
    static  void bindVBO(unsigned int vbo);
    static  void bindEBO(unsigned int ebo);
    static  void bindVAO(unsigned int vao);

private:

    static unsigned int last_vbo, last_ebo, last_vao;
    static GeneralBuffer temp;
    std::map<GLenum, unsigned int> log;

    GlBuffer() {}
    ~GlBuffer() {}
};

/// <summary>
/// DataDraw
/// </summary>
class DataDraw { 
public:
    struct Data {
        size_t count_object;
        size_t count_vertex;
        GLenum  mode_primitive;
        Data(size_t Count_object, size_t Count_vertex, GLenum  Mode_primitive)
            :count_vertex(Count_vertex), count_object(Count_object), mode_primitive(Mode_primitive)
        {}
        Data() {}
    }data;
    enum DrawType:size_t
    {
        DrawArrays, DrawElements, DrawArraysInstanced, DrawElementsInstanced, DrawSize
    }cur_type;
   
    DataDraw();
    DataDraw(DrawType mode_draw, GLenum mode_primitive, size_t count_vertex);
    DataDraw(DrawType mode_draw, GLenum mode_primitive, size_t count_vertex, size_t count_object);
    DataDraw(const DataDraw& data_object, size_t count_object);
    inline void draw()const {
        type[cur_type]->draw(data);
    }
private:

    struct Draw {
        virtual void draw(const Data& data) = 0;
    };

    struct drawArrays :Draw {
        inline void draw(const Data& data) {
            glDrawArrays(data.mode_primitive, 0, data.count_vertex);
        }
    };

    struct drawElements :Draw {
        inline void draw(const Data& data) {
            glDrawElements(data.mode_primitive, data.count_vertex, GL_UNSIGNED_INT, 0);
        }
    };

    struct drawArraysInstanced :Draw {
        inline void draw(const Data& data) {
            glDrawArraysInstanced(data.mode_primitive, 0, data.count_vertex, data.count_object);
        }
    };

    struct drawElementsInstanced :Draw {
        inline void draw(const Data& data) {
            glDrawElementsInstanced(data.mode_primitive, data.count_vertex, GL_UNSIGNED_INT, 0, data.count_object);
        }
    };

    static std::unique_ptr<Draw> type[DrawSize];
};

/// <summary>
/// GBO
/// </summary>
class GBO:public GeneralBuffer {

public:

    enum  MODE_DRAW:GLenum
    {
        STATIC = 0x88E4, DYNAMIC = 0x88E8
    };

    GBO() {
        ID = std::make_unique<g_buffer>();
    }

    void create(size_t Size, const void* Data = NULL) {
        cur_size = Size;
        glBufferData(target, cur_size, Data, mode);
    }

    inline unsigned int get() {
        return **ID.get();
    }

    void setMode(GLenum Mode) {
        mode = Mode;
    }

    inline size_t size()const { return cur_size; }
    //data function

    void data(size_t Size, const void* data);

    void data(size_t offset, size_t Size, const void* data);
    //[]
    template<typename vector_type_data>
    void data(const std::vector<vector_type_data>& Data) {
        data(sizeof(vector_type_data) * Data.size(), Data.data());
    }

    template<typename vector_type_data>
    void data(size_t offset, const std::vector<vector_type_data>& Data) {
        data(offset, sizeof(vector_type_data) * Data.size(), Data.data());
    }
    //[][]
    template<typename vector_type_data>
    void data(const std::vector<std::vector<vector_type_data>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size();
        if (cur_size < Data.size() * step)  create(Data.size() * step);
        for (size_t i = 0; i < Data.size(); i++) data(step * i, step, Data[i].data());
    }

    template<typename vector_type_data>
    void data(size_t offset, const std::vector<std::vector<vector_type_data>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size();
        for (size_t i = 0; i < Data.size(); i++) data(step * i + offset, step, Data[i].data());
    }
    //[][][]
    template<typename vector_type_data>
    void data(const std::vector<std::vector<std::vector<vector_type_data>>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size() * Data[0][0].size();
        if (cur_size < Data.size() * step)  create(Data.size() * step);
        for (size_t i = 0; i < Data.size(); i++) data(step * i, Data[i]);
    }

    template<typename vector_type_data>
    void data(size_t offset, const std::vector<std::vector<std::vector<vector_type_data>>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size() * Data[0][0].size();
        for (size_t i = 0; i < Data.size(); i++) data(step * i + offset, Data[i]);
    }

    template<typename vector_type_data>
    void update(size_t begin, size_t count, const std::vector<vector_type_data>& _data) {
        size_t size = sizeof(vector_type_data) * _data.size();
        if (size > cur_size)
            data(size, _data.data());
        else {
            std::vector<vector_type_data> data_in_buffer(count);
            for (size_t i = 0; i < count; i++)
                data_in_buffer[i] = _data[begin + i];

            data(begin * sizeof(vector_type_data), data_in_buffer);
        }

    }

protected:
    GLenum mode = STATIC;
    GLenum target;

};

/// <summary>
/// ElementBufferObject
/// </summary>
class ElementBufferObject :public GBO {
    
public:

    ElementBufferObject() {
        target = GL_ELEMENT_ARRAY_BUFFER;
    }

    void begin() {
        GlBuffer::bindEBO(**ID);
    }

    static inline void end() {
        GlBuffer::bindEBO(0);
    }
};
class VertexBufferObject :public GBO {

public:

    VertexBufferObject() {
        target = GL_ARRAY_BUFFER;
    }

    void begin()const {
        GlBuffer::bindVBO(**ID);
    }

    static inline void end() {
        GlBuffer::bindVBO(0);
    }
};

/// <summary>
/// ArrayBufferObject
/// </summary>
class ArrayBufferObject {

    class buffer {
        unsigned int ID;
    public:
        buffer() {
            glGenVertexArrays(1, &ID);
        }
        unsigned int operator *() {
            return ID;
        }
        ~buffer() {
            glDeleteVertexArrays(1, &ID);
        }
    };

public:

    ArrayBufferObject() {
        ID = std::make_unique<buffer>();
    }

    inline void begin()const {
        GlBuffer::bindVAO(**ID);
    }

    void attrib(size_t attribute, size_t size, size_t step, size_t offset);
    void attribI(size_t attribute, size_t size, size_t step, size_t offset);
    void attribInstanceVBOMat4(size_t attribute);
    void attribInstance(size_t attribute, size_t size, size_t step, size_t offset);

    static inline void end() {
        GlBuffer::bindVAO(0);
    }

    size_t sizeAttribute()const {
        return size_attribute;
    }

private:

    size_t size_attribute = 0;
    std::shared_ptr<buffer> ID;

};

/// <summary>
/// DrawBuffer
/// </summary>
class DrawBuffer :public ArrayBufferObject {
public:
    DrawBuffer() {}
    DrawBuffer(const ArrayBufferObject& buffer,const DataDraw& Data_draw) 
        :ArrayBufferObject(buffer), data_draw(Data_draw) {}
    DataDraw data_draw;
    inline void setCountVertexDraw(size_t new_size) {
        data_draw.data.count_vertex = new_size;
    }
    inline void draw()const {
        begin();
        data_draw.draw();
    }
};


#endif