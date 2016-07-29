//
//  OpenGLRHI.h
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#pragma once

#include <map>
#include <vector>

#include "gtf/RHI.h"

namespace gtf 
{
class OpenGLTexture : public RHITexture2D
{
public:
	OpenGLTexture();
    virtual ~OpenGLTexture();
    virtual void setup(ERHITexInternalFormat inFormat,
                       unsigned int bufferW,
                       unsigned int bufferH,
                       ERHITexFormat format,
                       ERHIValueType type,
                       const void* buffer = nullptr,
                       const RHITexParam* texParams = nullptr,
                       unsigned int numParams = 0);
    virtual void bindAt(unsigned int texUnit);
    virtual void unbind();
    virtual unsigned int getName() const;
    virtual void getInfo(RHITextureInfo & info) const;
    virtual void copyToBuffer(void* buffer, ERHITexFormat format, ERHIValueType valType);
    virtual void copyFromBuffer(void* buffer);
    virtual void clearWithColor(float r, float g = 0.0f, float b = 0.0f, float a = 0.0f);
    virtual RHITexture2D * cloneTexture(bool withData = false);
    virtual void flipY();
private:
    unsigned int m_name;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_lastBind {0};
    ERHITexInternalFormat m_internalFormat;
    ERHITexFormat m_format;
    ERHIValueType m_type;

};

class OpenGLCubeMap : public RHICubeMap
{
public:
	OpenGLCubeMap();
    virtual ~OpenGLCubeMap();
    virtual void setup(RHITexture2D ** faces, int lodLvl);
    virtual void bindAt(unsigned int texUnit) ;
    virtual void unbind();
private:
    unsigned int m_name;
    unsigned int m_lastBind {0};
};

class OpenGLFrameBuffer : public RHIFrameBuffer
{
public:
	OpenGLFrameBuffer();
    virtual ~OpenGLFrameBuffer();
    virtual void setup(RHITexture2D** attachments,
                       unsigned int numAttachments = 1,
                       RHITexture2D* depthAttach = nullptr);
    virtual void bindForDrawing();
    virtual void bindForReading();
    virtual void unbindForDrawing();
    virtual void unbindForReading();
    virtual unsigned int getName() const;
    virtual void getPixelAt(int x, int y, unsigned int attachIndex, void* buffer);
    virtual void copyAttachmentToTexture(unsigned int attachment, RHITexture2D* outputTex);
    virtual void clearAttachment(unsigned int attachIndex,
                                 float r = 0.0f,
                                 float g = 0.0f,
                                 float b = 0.0f,
                                 float a = 0.0f);
    virtual void clearDepthBuffer(float d = 1.0f);
private:
    unsigned int m_name;
    unsigned int n_numAttachments;
    RHITexture2D* m_attachments[11];
    RHITextureInfo m_attachmentsInfo[11];
};

class OpenGLShaderSource : public RHIShaderSource
{
    friend class OpenGLShaderProgram;
public:
    virtual ~OpenGLShaderSource();
    virtual void compileSource(const char * sourceStr, ERHIShaderType type);
    virtual unsigned int getName() const { return m_name; }
    virtual ERHIShaderType getType() const { return m_type; }
private:
    unsigned int m_name {0};
    ERHIShaderType m_type;
};

class OpenGLShaderProgram : public RHIShaderProgram
{
public:
	OpenGLShaderProgram();
    virtual ~OpenGLShaderProgram();
    virtual void link(RHIShaderSource** sources, size_t sourceCount);
    virtual void active();
    
    virtual void setUniform1ui(const char * ref, unsigned int value);
    virtual void setUniform1i(const char * ref, int value);
    virtual void setUniform1f(const char * ref, float v1);
    virtual void setUniform2f(const char * ref, float v1, float v2);
    virtual void setUniform4f(const char * ref, float v1, float v2, float v3, float v4);
    virtual void setUniform4x4m(const char * ref, float* mat4x4);
    virtual void setUniform3fv(const char * ref, unsigned int numElems, float* vec3array);
    
private:
    int getUniformLocation(const char * ref);
    unsigned int m_name;
    std::map<std::string, int> m_uniformNames;
};

class OpenGLVAO : public RHIVAO
{
public:
    virtual ~OpenGLVAO();
    virtual void render();
	virtual void setup(RHIVertexAttributeList const & list, void* vertexList, unsigned int vertexCount);
private:
    unsigned int m_name {0};
    unsigned int m_vertexCount {0};
    unsigned int m_meshVBO {0};
};


class OpenGLRHI : public RHI
{
public:
    virtual int init();
    virtual void setClearColor(float r, float g, float b, float a);
    virtual void clearColorAndDepthBuffers();
    virtual void viewport(int x, int y, int w, int h);
    virtual void setDepthTest(bool enabled);
    virtual void setBackfaceCulling(bool enabled);
    virtual void emitVertexCommand(unsigned int numVertex);
    virtual void setBlend(bool enabled);
    virtual void setBlendFnc(ERHIBlendFnc colorSrcFactor = ERHIBlendFnc::RHI_SRC_ALPHA,
                             ERHIBlendFnc colorDstFactor = ERHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA,
                             ERHIBlendFnc alphaSrcFactor = ERHIBlendFnc::RHI_SRC_ALPHA,
                             ERHIBlendFnc alphaDstFactor = ERHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA);
    virtual void setBlendEquation(ERHIBlendEquation colorEq = ERHIBlendEquation::RHI_FUNC_ADD,
                                  ERHIBlendEquation alphaEq = ERHIBlendEquation::RHI_FUNC_ADD);
    
    virtual RHIVAO* createVertexBufferObject();
    virtual RHIShaderProgram* createProgram();
    virtual RHIShaderSource* createSource();
    virtual RHITexture2D* createTexture();
    virtual RHICubeMap* createCubeMap();
    virtual RHIFrameBuffer* createFramebuffer();
    
    //add a new shader header source
    virtual void addShaderHeader(const char * headerName, const char * headerContent);
    
    //replaces all header included in shader source by it's content
    virtual std::string solveShaderHeaders(const char * shaderSource);
private:
    unsigned int m_defaultVertexArray;
    std::map<std::string, std::string> m_shaderHeaders;
    
};

} //namespace gtf
