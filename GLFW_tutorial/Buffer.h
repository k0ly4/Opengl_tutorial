#ifndef BUFFER_H
#define BUFFER_H
#include "Math.h"

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
class GlBuffer {
    static unsigned int last_vbo, last_ebo,last_vao;
    static GeneralBuffer temp;
    std::map<GLenum, unsigned int> log;
    GlBuffer() {}
    ~GlBuffer() {}
public:
    static inline void resize(GeneralBuffer& buffer,size_t new_size,GLenum mode = GL_STATIC_DRAW) {
        bindVBO(** temp.ID);
        glBufferData(GL_ARRAY_BUFFER, buffer.cur_size, NULL, GL_STATIC_DRAW);
        copy(buffer, temp);
        bindVBO(**buffer.ID);
        glBufferData(GL_ARRAY_BUFFER, new_size, NULL, mode);
        copy(temp, buffer);
    }
    static inline void copy(GeneralBuffer& read, GeneralBuffer& write) {
        glBindBuffer(GL_COPY_READ_BUFFER, **read.ID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, **write.ID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read.cur_size);
    }
    static inline void copy(GeneralBuffer& read, int readOffset, GeneralBuffer& write, int writeOffset,size_t size) {
        glBindBuffer(GL_COPY_READ_BUFFER, **read.ID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, **write.ID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
    }
    static inline void bindVBO (unsigned int vbo) {
        if (last_vbo != vbo) {
            last_vbo = vbo;
            glBindBuffer(GL_ARRAY_BUFFER, last_vbo);
        }
    }
    static inline void bindEBO(unsigned int ebo) {
        if (last_ebo != ebo) {
            last_ebo = ebo;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        }
    }
    static inline void bindVAO(unsigned int vao) {
        if (last_vao != vao) {
            last_vao = vao;
            glBindVertexArray(last_vao);
        }
    }
};
unsigned int GlBuffer::last_vao = 0;
unsigned int GlBuffer::last_vbo = 0;
unsigned int GlBuffer::last_ebo = 0;
GeneralBuffer GlBuffer::temp;
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
   
    DataDraw(){
        data = Data(0, 0, GL_TRIANGLES);
        cur_type = DrawArrays;
    }
    DataDraw(DrawType mode_draw,GLenum mode_primitive, size_t count_vertex) {
        data = Data(1, count_vertex, mode_primitive);
        cur_type = mode_draw;
    }
    DataDraw(DrawType mode_draw,GLenum mode_primitive, size_t count_vertex,size_t count_object) {
        data = Data(count_object, count_vertex, mode_primitive);
        cur_type = mode_draw;
    }
    DataDraw(const DataDraw& data_object, size_t count_object) {
        data = Data(count_object, data_object.data.count_vertex, data_object.data.mode_primitive);
        if (data_object.cur_type == DrawArrays) cur_type = DrawArraysInstanced;
        else if (data_object.cur_type == DrawElements) cur_type = DrawElementsInstanced;
        else cur_type = data_object.cur_type;
    } 
    inline void draw() {
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
std::unique_ptr<DataDraw::Draw> DataDraw::type[DataDraw::DrawSize] = {
    std::make_unique< DataDraw::drawArrays>(),
    std::make_unique< DataDraw::drawElements>(),
    std::make_unique< DataDraw::drawArraysInstanced>(),
    std::make_unique< DataDraw::drawElementsInstanced>()
};

class GBO:public GeneralBuffer {
protected:
    GLenum mode = STATIC;
    GLenum target;
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

   void data(size_t Size, const void* data) {
        if (cur_size < Size) {
            cur_size = Size;
            glBufferData(target, Size, data, mode);
        }
        else glBufferSubData(target, 0, Size, data);
    }
   void data(size_t offset, size_t Size, const void* data) {
    if (cur_size < offset + Size) std::cout << "Error::Buffer full\n";
    else glBufferSubData(target, offset, Size, data);
}
   //[]
   template<typename vector_type_data>
   inline void data(const std::vector<vector_type_data>& Data) {
       data(sizeof(vector_type_data) * Data.size(), Data.data());
   }
   template<typename vector_type_data>
   inline void data(size_t offset, const std::vector<vector_type_data>& Data) {
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
    inline void data(size_t offset,const std::vector<std::vector<vector_type_data>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size();
        for (size_t i = 0; i < Data.size(); i++) data(step*i+ offset, step, Data[i].data());
    }
    //[][][]
    template<typename vector_type_data>
    void data(const std::vector<std::vector<std::vector<vector_type_data>>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size() * Data[0][0].size();
        if (cur_size < Data.size() * step)  create(Data.size() * step);
        for (size_t i = 0; i < Data.size(); i++) data(step * i, Data[i]);
    }
    template<typename vector_type_data>
    inline void data(size_t offset, const std::vector<std::vector<std::vector<vector_type_data>>>& Data) {
        size_t step = sizeof(vector_type_data) * Data[0].size()*Data[0][0].size();
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
};
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
    void begin() {
        GlBuffer::bindVBO(**ID);
    }
    static inline void end() {
        GlBuffer::bindVBO(0);
    }
};

class ArrayBufferObject {
    size_t size_attribute = 0;
    class g_buffer {
        unsigned int ID;
    public:
        g_buffer() {
            glGenVertexArrays(1, &ID);
        }
        unsigned int operator *() {
            return ID;
        }
        ~g_buffer() {
            glDeleteVertexArrays(1, &ID);
        }
    };
    std::shared_ptr<g_buffer> ID;
public:
    ArrayBufferObject() {
        ID = std::make_unique<g_buffer>();
    }
    inline void begin() {
        GlBuffer::bindVAO(**ID);
    }
    inline void attrib(size_t attribute, size_t size, size_t step, size_t offset) {
        size_attribute++;
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, GL_FLOAT, GL_FALSE, step, (void*)(offset));
    }
    inline void attribI(size_t attribute, size_t size, size_t step, size_t offset) {
        size_attribute++;
        glEnableVertexAttribArray(attribute);
        glVertexAttribIPointer(attribute, size, GL_INT, step, (void*)(offset));
    }
    inline void attribInstanceVBOMat4(size_t attribute) {
        for (size_t i = 0; i < 4; i++) 
            attribInstance(attribute + i, 4, sizeof(glm::mat4), i * sizeof(glm::vec4));
    }
    void attribInstance(size_t attribute, size_t size, size_t step, size_t offset) {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, GL_FLOAT, GL_FALSE, step, (void*)(offset));
        glVertexAttribDivisor(attribute, 1);
    }
    static inline void end() {
        GlBuffer::bindVAO(0);
    }
    size_t sizeAttribute()const {
        return size_attribute;
    }
};
class DrawBuffer :public ArrayBufferObject {
public:
    DrawBuffer() {}
    DrawBuffer(const ArrayBufferObject& buffer,const DataDraw& Data_draw) 
        :ArrayBufferObject(buffer), data_draw(Data_draw) {}
    DataDraw data_draw;
    inline void setCountVertexDraw(size_t new_size) {
        data_draw.data.count_vertex = new_size;
    }
    inline void draw() {
        begin();
        data_draw.draw();
    }
};
#endif