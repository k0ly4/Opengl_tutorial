#include "Buffer.h"

void GlBuffer::resize(GeneralBuffer& buffer, size_t new_size, GLenum mode) {
        bindVBO(**temp.ID);
        glBufferData(GL_ARRAY_BUFFER, buffer.cur_size, NULL, GL_STATIC_DRAW);
        copy(buffer, temp);
        bindVBO(**buffer.ID);
        glBufferData(GL_ARRAY_BUFFER, new_size, NULL, mode);
        copy(temp, buffer);
    }
 void GlBuffer::copy(GeneralBuffer& read, GeneralBuffer& write) {
        glBindBuffer(GL_COPY_READ_BUFFER, **read.ID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, **write.ID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read.cur_size);
    }
      void GlBuffer::copy(GeneralBuffer& read, int readOffset, GeneralBuffer& write, int writeOffset, size_t size) {
        glBindBuffer(GL_COPY_READ_BUFFER, **read.ID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, **write.ID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
    }
      void GlBuffer::bindVBO(unsigned int vbo) {
        if (last_vbo != vbo) {
            last_vbo = vbo;
            glBindBuffer(GL_ARRAY_BUFFER, last_vbo);
        }
    }
      void GlBuffer::bindEBO(unsigned int ebo) {
        if (last_ebo != ebo) {
            last_ebo = ebo;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        }
    }
      void GlBuffer::bindVAO(unsigned int vao) {
        if (last_vao != vao) {
            last_vao = vao;
            glBindVertexArray(last_vao);
        }
    }
unsigned int GlBuffer::last_vao = 0;
unsigned int GlBuffer::last_vbo = 0;
unsigned int GlBuffer::last_ebo = 0;
GeneralBuffer GlBuffer::temp;

DataDraw::DataDraw() {
        data = Data(0, 0, GL_TRIANGLES);
        cur_type = DrawArrays;
    }
DataDraw::DataDraw(DrawType mode_draw, GLenum mode_primitive, size_t count_vertex) {
        data = Data(1, count_vertex, mode_primitive);
        cur_type = mode_draw;
    }
DataDraw::DataDraw(DrawType mode_draw, GLenum mode_primitive, size_t count_vertex, size_t count_object) {
        data = Data(count_object, count_vertex, mode_primitive);
        cur_type = mode_draw;
    }
DataDraw::DataDraw(const DataDraw& data_object, size_t count_object) {
        data = Data(count_object, data_object.data.count_vertex, data_object.data.mode_primitive);
        if (data_object.cur_type == DrawArrays) cur_type = DrawArraysInstanced;
        else if (data_object.cur_type == DrawElements) cur_type = DrawElementsInstanced;
        else cur_type = data_object.cur_type;
    }

std::unique_ptr<DataDraw::Draw> DataDraw::type[DataDraw::DrawSize] = {
    std::make_unique< DataDraw::drawArrays>(),
    std::make_unique< DataDraw::drawElements>(),
    std::make_unique< DataDraw::drawArraysInstanced>(),
    std::make_unique< DataDraw::drawElementsInstanced>()
};


  
    //data function
    //GBO
    void GBO::data(size_t Size, const void* data) {
        if (cur_size < Size) {
            cur_size = Size;
            glBufferData(target, Size, data, mode);
        }
        else glBufferSubData(target, 0, Size, data);
    }
    void GBO::data(size_t offset, size_t Size, const void* data) {
        if (cur_size < offset + Size) std::cout << "Error::Buffer full\n";
        else glBufferSubData(target, offset, Size, data);
    }
   




    /// <summary>
    /// ArrayBufferObject
    /// </summary>
    /// <param name="attribute"></param>
    /// <param name="size"></param>
    /// <param name="step"></param>
    /// <param name="offset"></param>
    void ArrayBufferObject::attrib(size_t attribute, size_t size, size_t step, size_t offset) {
        size_attribute++;
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, GL_FLOAT, GL_FALSE, step, (void*)(offset));
    }
     void ArrayBufferObject::attribI(size_t attribute, size_t size, size_t step, size_t offset) {
        size_attribute++;
        glEnableVertexAttribArray(attribute);
        glVertexAttribIPointer(attribute, size, GL_INT, step, (void*)(offset));
    }
     void ArrayBufferObject::attribInstanceVBOMat4(size_t attribute) {
        for (size_t i = 0; i < 4; i++)
            attribInstance(attribute + i, 4, sizeof(glm::mat4), i * sizeof(glm::vec4));
    }
     void ArrayBufferObject::attribInstanceMat4(size_t attribute,size_t step,size_t offset) {
         for (size_t i = 0; i < 4; i++)
             attribInstance(attribute + i, 4, step, i * sizeof(glm::vec4)+ offset);
     }
    void ArrayBufferObject::attribInstance(size_t attribute, size_t size, size_t step, size_t offset) {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, GL_FLOAT, GL_FALSE, step, (void*)(offset));
        glVertexAttribDivisor(attribute, 1);
    }
