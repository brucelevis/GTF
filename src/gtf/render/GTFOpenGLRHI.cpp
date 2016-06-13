//
//  MPOpenGLRHI.cpp
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#include <iostream>
#include <string> 
#include <cassert>

#include <string.h>

#include "GTFOpenGLRHI.h"
#include "glad.h"


static GLenum RHI_OGL_SHADER_TYPE[] =
{
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER
    
};

static GLenum RHI_OGL_TEX_INTERNAL_FORMAT[] =
{
    GL_RGB8,
    GL_RGBA8,
    GL_DEPTH_COMPONENT32,
    GL_RGBA32F,
    GL_RGB32F,
    GL_RGBA16F,
    GL_RGB16F,
    GL_RG16F,
    GL_R16F,
    GL_R8,
    GL_DEPTH_COMPONENT
};

static GLsizei RHI_OGL_TEX_INTERNAL_FORMAT_SIZES[] =
{
    8, //GL_RGB8
    8, //GL_RGBA8
    32, // GL_DEPTH_COMPONENT32
    32,  // GL_RGBA32F
    32, // GL_RGB32F,
    16, // GL_RGBA16F,
    16, // GL_RGB16F,
    16, // GL_RG16F
    16, // GL_R16F
    8,  // GL_R8
    8   //GL_DEPTH_COMPONENT
};


static GLenum RHI_OGL_TEX_FORMAT[] =
{
    GL_DEPTH_COMPONENT,
    GL_DEPTH_STENCIL,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_BGR,
    GL_BGRA
};

static unsigned int RHI_OGL_TEX_FORMAT_COMPONENTS[] =
{
    1, // GL_DEPTH_COMPONENT,
    2, // GL_DEPTH_STENCIL,
    1, // GL_RED,
    2, // GL_RG,
    3, // GL_RGB,
    4, // GL_RGBA,
    3, // GL_BGR
    4  // GL_BGRA
    
};

static GLenum RHI_OGL_VALUE_TYPE[] =
{
    GL_FLOAT,
    GL_INT,
    GL_UNSIGNED_BYTE
};

static size_t RHI_OGL_VALUE_TYPE_SIZES[] =
{
    4,
    4,
    1
};

static GLenum RHI_OGL_BLEND_FNC[] =
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_CONSTANT_COLOR,
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_ALPHA,
    GL_SRC_ALPHA_SATURATE
};

static GLenum RHI_OGL_BLEND_EQ[] =
{
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX
};

static GLenum RHI_OGL_TEX_PARAM_NAMES[] =
{
    GL_TEXTURE_MIN_FILTER,
    GL_TEXTURE_MAG_FILTER,
    GL_TEXTURE_WRAP_S,
    GL_TEXTURE_WRAP_T
};

static GLenum RHI_OGL_TEX_PARAM_VALUES[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR,
    GL_CLAMP_TO_EDGE,
    GL_MIRRORED_REPEAT,
    GL_REPEAT
};

/*
static GLenum RHI_OGL_COMPARE_FUNC[] =
{
    GL_LEQUAL,
    GL_GEQUAL,
    GL_LESS,
    GL_GREATER,
    GL_EQUAL,
    GL_NOTEQUAL,
    GL_ALWAYS,
    GL_NEVER
};*/

////// MPOpenGLTexture //////
void MPOpenGLTexture::create()
{
    glGenTextures(1, &m_name);
}

MPOpenGLTexture::~MPOpenGLTexture()
{
    glDeleteTextures(1, &m_name);
}

void MPOpenGLTexture::setup(EGTFRHITexInternalFormat inFormat,
                            unsigned int bufferW,
                            unsigned int bufferH,
                            EGTFRHITexFormat format,
                            EGTFRHIValueType type,
                            const void* buffer,
                            const GTFRHITexParam* texParams,
                            unsigned int numParams)
{
    m_internalFormat = inFormat;
    m_width = bufferW;
    m_height = bufferH;
    m_format = format;
    m_type = type;
    
    glBindTexture(GL_TEXTURE_2D, m_name);
    
    //apply default parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    for(unsigned int paramIndex = 0; paramIndex < numParams; paramIndex++)
    {
        glTexParameteri(GL_TEXTURE_2D,
                        RHI_OGL_TEX_PARAM_NAMES[texParams[paramIndex].name],
                        RHI_OGL_TEX_PARAM_VALUES[texParams[paramIndex].value]);
    }
    
    
    if(buffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, RHI_OGL_TEX_INTERNAL_FORMAT[m_internalFormat], bufferW, bufferH, 0, RHI_OGL_TEX_FORMAT[m_format], RHI_OGL_VALUE_TYPE[m_type], buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        clearWithColor(1.0, 1.0, 1.0, 0.0);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MPOpenGLTexture::bindAt(unsigned int texUnit)
{
    m_lastBind = texUnit;
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, m_name);
}


void MPOpenGLTexture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_lastBind);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int MPOpenGLTexture::getName() const
{
    return m_name;
}

void MPOpenGLTexture::getInfo(GTFRHITextureInfo & info) const
{
    info.internalFormat = m_internalFormat;
    info.bufferW = m_width;
    info.bufferH = m_height;
    info.format = m_format;
    info.type = m_type;
    info.size = m_width * m_height * RHI_OGL_TEX_INTERNAL_FORMAT_SIZES[m_internalFormat] * RHI_OGL_TEX_FORMAT_COMPONENTS[m_format];
}

void MPOpenGLTexture::copyToBuffer(void* buffer, EGTFRHITexFormat format, EGTFRHIValueType valType)
{
    glBindTexture(GL_TEXTURE_2D, m_name);
    glGetTexImage(GL_TEXTURE_2D, 0, RHI_OGL_TEX_FORMAT[format], RHI_OGL_VALUE_TYPE[valType], buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MPOpenGLTexture::copyFromBuffer(void* buffer)
{
    glBindTexture(GL_TEXTURE_2D, m_name);
    glTexImage2D(GL_TEXTURE_2D, 0, RHI_OGL_TEX_INTERNAL_FORMAT[m_internalFormat], m_width, m_height, 0, RHI_OGL_TEX_FORMAT[m_format], RHI_OGL_VALUE_TYPE[m_type], buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MPOpenGLTexture::clearWithColor(float r, float g, float b, float a)
{
    float compColor[] = { r, g, b, a };
    size_t numPixels = m_width * m_height;
    int numComponents = RHI_OGL_TEX_FORMAT_COMPONENTS[m_format];
    size_t componentSize = RHI_OGL_VALUE_TYPE_SIZES[m_type];
    
    unsigned char* colorBuff = new unsigned char[numPixels * numComponents * componentSize];
    
    for(size_t pix = 0; pix < numPixels; pix++)
    {
        for(int comp = 0; comp < numComponents; comp++)
        {
            colorBuff[(pix * numComponents) + comp] = (unsigned char)(compColor[comp] * 255);
        }
    }
    
    copyFromBuffer(colorBuff);
    delete [] colorBuff;
}

GTFRHITexture2DPtr MPOpenGLTexture::cloneTexture(bool withData)
{
    GTFRHITexture2DPtr newTex = RHI->createTexture();
    unsigned char* buffer = nullptr;
    if(withData)
    {
        size_t numPixels = m_width * m_height;
        int numComponents = RHI_OGL_TEX_FORMAT_COMPONENTS[m_format];
        size_t componentSize = RHI_OGL_VALUE_TYPE_SIZES[m_type];
        buffer = new unsigned char[numPixels * numComponents * componentSize];
        copyToBuffer(buffer, m_format, m_type);
    }
    newTex->setup( m_internalFormat, m_width, m_height, m_format, m_type, buffer);
    
    if(withData)
        delete [] buffer;
    return newTex;
}

void MPOpenGLTexture::flipY()
{
    size_t numPixels = m_width * m_height;
    int numComponents = RHI_OGL_TEX_FORMAT_COMPONENTS[m_format];
    size_t componentSize = RHI_OGL_VALUE_TYPE_SIZES[m_type];
    unsigned char* buffer = new unsigned char[numPixels * numComponents * componentSize];
    unsigned char* newBuffer = new unsigned char[numPixels * numComponents * componentSize];
    copyToBuffer(buffer, m_format, m_type);
    
    for (unsigned int row = 0; row < m_height; row++)
    {
        for (unsigned int col = 0; col < m_width-1; col++)
        {
            size_t leftImageIdx = componentSize * numComponents * (((m_height - (row + 1)) * m_width) + col);
            size_t rightImageIdx = componentSize * numComponents * ((row * m_width) + col);
           
            memcpy(&newBuffer[(leftImageIdx) + (0 * componentSize)], &buffer[(rightImageIdx) + (0 * componentSize)], componentSize * numComponents);
            memcpy(&newBuffer[(leftImageIdx) + (1 * componentSize)], &buffer[(rightImageIdx) + (1 * componentSize)], componentSize * numComponents);
            memcpy(&newBuffer[(leftImageIdx) + (2 * componentSize)], &buffer[(rightImageIdx) + (2 * componentSize)], componentSize * numComponents);
            memcpy(&newBuffer[(leftImageIdx) + (3 * componentSize)], &buffer[(rightImageIdx) + (3 * componentSize)], componentSize * numComponents);
            
            /*
            newBuffer[(numComponents * leftImageIdx) + 0] = buffer[(numComponents * rightImageIdx) + 0];
            newBuffer[(numComponents * leftImageIdx) + 1] = buffer[(numComponents * rightImageIdx) + 1];
            newBuffer[(numComponents * leftImageIdx) + 2] = buffer[(numComponents * rightImageIdx) + 2];
            newBuffer[(numComponents * leftImageIdx) + 3] = buffer[(numComponents * rightImageIdx) + 3];
             */
        }
    }
    
    copyFromBuffer(newBuffer);
    delete [] buffer;
    delete [] newBuffer;
}


/////// CUBE MAP ///////

void MPOpenGLCubeMap::create()
{
    glGenTextures(1, &m_name);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

MPOpenGLCubeMap::~MPOpenGLCubeMap()
{
    glDeleteTextures(1, &m_name);
}

void MPOpenGLCubeMap::setup(GTFRHITexture2DPtr * faces, int lodLvl)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
    
    for(int i = 0; i < 6; i++)
    {
        if(lodLvl <= 6)
        {
            m_sides[lodLvl][i] = faces[i];
        }
        
        GTFRHITextureInfo texInfo;
        faces[i]->getInfo(texInfo);
        unsigned char* image = (unsigned char*)malloc(texInfo.size);
        faces[i]->copyToBuffer(image, texInfo.format, texInfo.type);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     lodLvl,
                     RHI_OGL_TEX_INTERNAL_FORMAT[texInfo.internalFormat],
                     texInfo.bufferW,
                     texInfo.bufferH,
                     0,
                     RHI_OGL_TEX_FORMAT[texInfo.format],
                     RHI_OGL_VALUE_TYPE[texInfo.type],
                     image);
    }
    
    if(lodLvl == 0)
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void MPOpenGLCubeMap::bindAt(unsigned int texUnit)
{
    m_lastBind = texUnit;
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_name);
}

void MPOpenGLCubeMap::unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_lastBind);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

///// MPOpenGLFrameBuffer /////
void MPOpenGLFrameBuffer::create()
{
    glGenFramebuffers(1, &m_name);
    
}

MPOpenGLFrameBuffer::~MPOpenGLFrameBuffer()
{
    glDeleteFramebuffers(1, &m_name);
}

void MPOpenGLFrameBuffer::setup(GTFRHITexture2DPtr* attachments,
                                unsigned int numAttachments,
                                GTFRHITexture2DPtr depthAttach)
{
    assert(numAttachments <= 10);
    
    n_numAttachments = numAttachments;
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_name);
    
    for(unsigned int i = 0; i < n_numAttachments; ++i)
    {
        m_attachments[i] = attachments[i];
        
        if(m_attachments[i])
        {
            m_attachments[i]->getInfo(m_attachmentsInfo[i]);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                                   GL_COLOR_ATTACHMENT0 + i,
                                   GL_TEXTURE_2D,
                                   m_attachments[i]->getName(),
                                   0);
        }
    }
    
    if(depthAttach)
    {
        m_attachments[10] = depthAttach;
        depthAttach->getInfo(m_attachmentsInfo[4]);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D,
                               depthAttach->getName(),
                               0);
    }
    
    GLenum drawBuffers[] =
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
        GL_COLOR_ATTACHMENT4,
        GL_COLOR_ATTACHMENT5,
        GL_COLOR_ATTACHMENT6,
        GL_COLOR_ATTACHMENT7,
        GL_COLOR_ATTACHMENT8,
        GL_COLOR_ATTACHMENT9
    };
    
    glDrawBuffers(n_numAttachments, drawBuffers);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void MPOpenGLFrameBuffer::bindForDrawing()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_name);
}

void MPOpenGLFrameBuffer::bindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_name);
}

void MPOpenGLFrameBuffer::unbindForDrawing()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void MPOpenGLFrameBuffer::unbindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

unsigned int MPOpenGLFrameBuffer::getName() const
{
    return m_name;
}

void MPOpenGLFrameBuffer::getPixelAt(int x, int y, unsigned int attachIndex, void* buffer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_name);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachIndex);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, 1, 1,
                 RHI_OGL_TEX_FORMAT[m_attachmentsInfo[attachIndex].format],
                 RHI_OGL_VALUE_TYPE[m_attachmentsInfo[attachIndex].type],
                 buffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void MPOpenGLFrameBuffer::copyAttachmentToTexture(unsigned int attachmentIndex,
                                                  GTFRHITexture2DPtr outputTex)
{
    GTFRHITexture2DPtr attachment = m_attachments[attachmentIndex];
    GTFRHITextureInfo info;
    attachment->getInfo(info);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_name);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    outputTex->bindAt(0);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, RHI_OGL_TEX_FORMAT[info.format], 0, 0, info.bufferW, info.bufferH, 0);
    outputTex->unbind();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void MPOpenGLFrameBuffer::clearAttachment(unsigned int attachIndex, float r, float g, float b, float a)
{
    GLfloat colorAr[] = {r, g, b, a};
    
    if(attachIndex < 10)
    {
        glClearBufferfv(GL_COLOR, attachIndex, colorAr);
    }
}

void MPOpenGLFrameBuffer::clearDepthBuffer(float d)
{
    glClearBufferfv(GL_DEPTH, 0, &d);
}

////// MPOpenGLShaderSource //////
MPOpenGLShaderSource::~MPOpenGLShaderSource()
{
    if(m_name)
    {
        glDeleteShader(m_name);
    }
}

void MPOpenGLShaderSource::compileSource(std::string const & sourceStr, EGTFRHIShaderType type)
{
    m_type = type;
    m_name = glCreateShader(RHI_OGL_SHADER_TYPE[type]);
    m_sourceStr = sourceStr;
    
    std::string parsedSource = RHI->solveShaderHeaders(sourceStr);
    const char* src = parsedSource.c_str();
    glShaderSource(m_name, (GLsizeiptr)1, &src, NULL);
    glCompileShader(m_name);
    
    GLint status;
    glGetShaderiv(m_name, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        GLchar log[1024];
        GLsizei logLen;
        glGetShaderInfoLog(m_name, sizeof(GLchar)*1024, &logLen, log);
        std::cout << "ERROR: MPOpenGLShaderSource::compileSource : " << log << std::endl << parsedSource << std::endl;
    }
}

/////// MPOpenGLShaderProgram //////
void MPOpenGLShaderProgram::create()
{
    
}

MPOpenGLShaderProgram::~MPOpenGLShaderProgram()
{
    glUseProgram(0);
    glDeleteProgram(m_name);
}

void MPOpenGLShaderProgram::attachSource(GTFRHIShaderSourcePtr source)
{
    auto elem = std::find(m_attachedSources.begin(), m_attachedSources.end(), source);
    if(elem == m_attachedSources.end())
    {
        //m_attachedSources.erase(elem);
        m_attachedSources.push_back(source);
    }
}

void MPOpenGLShaderProgram::link()
{
    m_uniformNames.clear();
    glDeleteProgram(m_name);
    m_name = glCreateProgram();
    
    for(auto elem : m_attachedSources)
    {
        glAttachShader(m_name, elem->getName());
    }
    
    glLinkProgram(m_name);
    GLint status;
    glGetProgramiv(m_name, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        GLchar log[1024];
        GLsizei logLen;
        glGetProgramInfoLog(m_name, sizeof(GLchar)*1024, &logLen, log);
        std::cout << "ERROR : MPOpenGLShaderProgram::link : " << log << "\n";
    }
}

void MPOpenGLShaderProgram::active()
{
    glUseProgram(m_name);
}

int MPOpenGLShaderProgram::getUniformLocation(const std::string & ref)
{
    auto foundElement = m_uniformNames.find(ref);

    if(foundElement == m_uniformNames.end())
    {
        int uniformName = glGetUniformLocation(m_name, ref.c_str());
        m_uniformNames.insert(std::make_pair(ref, uniformName));
        return uniformName;
    }
    else
    {
        return foundElement->second;
    }
}

void MPOpenGLShaderProgram::setUniform1ui(const std::string & ref, unsigned int value)
{
    glUniform1ui(getUniformLocation(ref), value);
}

void MPOpenGLShaderProgram::setUniform1i(const std::string & ref, int value)
{
    glUniform1i(getUniformLocation(ref), value);
}

void MPOpenGLShaderProgram::setUniform1f(const std::string & ref, float v1)
{
    glUniform1f(getUniformLocation(ref), v1);
}

void MPOpenGLShaderProgram::setUniform2f(const std::string & ref, float v1, float v2)
{
    glUniform2f(getUniformLocation(ref), v1, v2);
}

void MPOpenGLShaderProgram::setUniform4f(const std::string & ref, float v1, float v2, float v3, float v4)
{
    glUniform4f(getUniformLocation(ref), v1, v2, v3, v4);
}

void MPOpenGLShaderProgram::setUniform4x4m(const std::string & ref, float* mat4x4)
{
    glUniformMatrix4fv(getUniformLocation(ref), 1, GL_FALSE, mat4x4);
}

void MPOpenGLShaderProgram::setUniform3fv(const std::string & ref, unsigned int numElems, float* vec3array)
{
    glUniform3fv(getUniformLocation(ref), numElems, vec3array);
}

//// MPOpenGLVAO /////
MPOpenGLVAO::~MPOpenGLVAO()
{
    glDeleteBuffers(1, &m_meshVBO);
    //glDeleteBuffers(1, &m_indexVBO);
    glDeleteVertexArrays(1, &m_name);
}

void MPOpenGLVAO::render()
{
    //glBindVertexArray(m_name);
    //glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(m_name);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

}

void MPOpenGLVAO::setup(GTFRHIVertex* vertexList,
                        unsigned int vertexCount)
{
    m_vertexCount = vertexCount;
    
    glGenVertexArrays(1, &m_name);
    glBindVertexArray(m_name);
    glGenBuffers(1, &m_meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GTFRHIVertex), vertexList, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GTFRHIVertex), (void *)offsetof(GTFRHIVertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GTFRHIVertex), (void *)offsetof(GTFRHIVertex, nx));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GTFRHIVertex), (void *)offsetof(GTFRHIVertex, tcx));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GTFRHIVertex), (void *)offsetof(GTFRHIVertex, tnx));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GTFRHIVertex), (void *)offsetof(GTFRHIVertex, bnx));
    glEnableVertexAttribArray(4);
    
    //glGenBuffers(1, &m_indexVBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexList, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}

//// MPOpenGLUVRegionVAO /////
MPOpenGLUVRegionVAO::~MPOpenGLUVRegionVAO()
{
    glDeleteBuffers(1, &m_meshVBO);
    glDeleteVertexArrays(1, &m_name);
}

void MPOpenGLUVRegionVAO::render()
{
    glBindVertexArray(m_name);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

void MPOpenGLUVRegionVAO::setup(GTFRHIUVRegionVertex* vertexList,
                                size_t vertexCount)
{
    m_vertexCount = (int)vertexCount;
    
    glGenVertexArrays(1, &m_name);
    glBindVertexArray(m_name);
    glGenBuffers(1, &m_meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GTFRHIUVRegionVertex), vertexList, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GTFRHIUVRegionVertex), (void *)offsetof(GTFRHIUVRegionVertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GTFRHIUVRegionVertex), (void *)offsetof(GTFRHIUVRegionVertex, faceId));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}


///// MPOpenGLRHI /////
int MPOpenGLRHI::init()
{
    if(!gladLoadGL())
    {
        std::cout << "gladLoadGL :: Something went wrong!";
        return -1;
    }
    else
    {
        std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << std::endl;
    }
    
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    
    if(GLAD_GL_EXT_texture_integer)
    {
        std::cout << "GL_EXT_texture_integer supported!" << std::endl;
    }
    else
    {
        std::cout << "GL_EXT_texture_integer NOT supported!" << std::endl;
    }
    
    if(GLAD_GL_ARB_seamless_cube_map)
    {
        std::cout << "GL_ARB_seamless_cube_map supported!" << std::endl;
    }
    else
    {
        std::cout << "GL_ARB_seamless_cube_map NOT supported!" << std::endl;
    }
    
    if(GLAD_GL_ARB_texture_cube_map)
    {
        std::cout << "GL_ARB_texture_cube_map supported!" << std::endl;
    }
    else
    {
        std::cout << "GL_ARB_texture_cube_map NOT supported!" << std::endl;
    }
    
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    glGenVertexArrays(1, &m_defaultVertexArray);
    
    return 0;
}

void MPOpenGLRHI::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}

void MPOpenGLRHI::clearColorAndDepthBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MPOpenGLRHI::viewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
}

void MPOpenGLRHI::setDepthTest(bool enabled)
{
    if(enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void MPOpenGLRHI::emitVertexCommand(unsigned int numVertex)
{
    glBindVertexArray(m_defaultVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, numVertex);
    glBindVertexArray(0);
}

void MPOpenGLRHI::setBlend(bool enabled)
{
    if(enabled)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void MPOpenGLRHI::setBackfaceCulling(bool enabled)
{
    if(enabled)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void MPOpenGLRHI::setBlendFnc(EGTFRHIBlendFnc colorSrcFactor,
                              EGTFRHIBlendFnc colorDstFactor,
                              EGTFRHIBlendFnc alphaSrcFactor,
                              EGTFRHIBlendFnc alphaDstFactor)
{
    glBlendFuncSeparate(RHI_OGL_BLEND_FNC[colorSrcFactor],
                        RHI_OGL_BLEND_FNC[colorDstFactor],
                        RHI_OGL_BLEND_FNC[alphaSrcFactor],
                        RHI_OGL_BLEND_FNC[alphaDstFactor]);
}

void MPOpenGLRHI::setBlendEquation(EGTFRHIBlendEquation colorEq,
                                   EGTFRHIBlendEquation alphaEq)
{
    glBlendEquationSeparate(RHI_OGL_BLEND_EQ[colorEq],
                            RHI_OGL_BLEND_EQ[alphaEq]);
}

GTFRHIVAOPtr MPOpenGLRHI::createVertexBufferObject()
{
    return GTFRHIVAOPtr(new MPOpenGLVAO);
}

GTFRHIUVRegionVAOPtr MPOpenGLRHI::createUVRegionVAO()
{
    return GTFRHIUVRegionVAOPtr(new MPOpenGLUVRegionVAO);
}

GTFRHIShaderProgramPtr MPOpenGLRHI::createProgram()
{
    GTFRHIShaderProgramPtr prog = GTFRHIShaderProgramPtr(new MPOpenGLShaderProgram());
    prog->create();
    return prog;
}

GTFRHIShaderSourcePtr MPOpenGLRHI::createSource()
{
    return GTFRHIShaderSourcePtr(new MPOpenGLShaderSource());
}

GTFRHITexture2DPtr MPOpenGLRHI::createTexture()
{
    GTFRHITexture2DPtr tmp = GTFRHITexture2DPtr(new MPOpenGLTexture);
    tmp->create();
    return tmp;
}

GTFRHICubeMapPtr MPOpenGLRHI::createCubeMap()
{
    GTFRHICubeMapPtr tmp = GTFRHICubeMapPtr(new MPOpenGLCubeMap);
    tmp->create();
    return tmp;
}

GTFRHIFrameBufferPtr MPOpenGLRHI::createFramebuffer()
{
    GTFRHIFrameBufferPtr tmp = GTFRHIFrameBufferPtr(new MPOpenGLFrameBuffer);
    tmp->create();
    return tmp;
}


//add a new shader header source
void MPOpenGLRHI::addShaderHeader(std::string const & headerName, std::string const & headerContent)
{
    m_shaderHeaders.insert(std::make_pair(headerName, headerContent));
}

//replaces all hader included in GLSL source file by it's content
std::string MPOpenGLRHI::solveShaderHeaders(std::string const & shaderSource)
{
    std::string newSource = shaderSource;
    
    for(auto headerPair : m_shaderHeaders)
    {
        std::string includeBody = "#include \"";
        includeBody += headerPair.first;
        includeBody += "\"";
        
        size_t start_pos = newSource.find(includeBody);
        while(start_pos != std::string::npos)
        {
            newSource = newSource.replace(start_pos, includeBody.length(), headerPair.second);
            start_pos = newSource.find(includeBody);
        }
    }
    
    return newSource;
}
