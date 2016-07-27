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
    virtual ~OpenGLTexture();
    virtual void create();
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
    virtual RHITexture2DPtr cloneTexture(bool withData = false);
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
    virtual ~OpenGLCubeMap();
    virtual void create();
    virtual void setup(RHITexture2DPtr * faces, int lodLvl);
    virtual void bindAt(unsigned int texUnit) ;
    virtual void unbind();
    virtual RHITexture2DPtr getSide(unsigned int lodLvl, unsigned int side) { return m_sides[lodLvl][side]; };
private:
    unsigned int m_name;
    unsigned int m_lastBind {0};
    RHITexture2DPtr m_sides[6][6];
};

class OpenGLFrameBuffer : public RHIFrameBuffer
{
public:
    virtual ~OpenGLFrameBuffer();
    virtual void create();
    virtual void setup(RHITexture2DPtr* attachments,
                       unsigned int numAttachments = 1,
                       RHITexture2DPtr depthAttach = nullptr);
    virtual void bindForDrawing();
    virtual void bindForReading();
    virtual void unbindForDrawing();
    virtual void unbindForReading();
    virtual unsigned int getName() const;
    virtual void getPixelAt(int x, int y, unsigned int attachIndex, void* buffer);
    virtual void copyAttachmentToTexture(unsigned int attachment, RHITexture2DPtr outputTex);
    virtual void clearAttachment(unsigned int attachIndex,
                                 float r = 0.0f,
                                 float g = 0.0f,
                                 float b = 0.0f,
                                 float a = 0.0f);
    virtual void clearDepthBuffer(float d = 1.0f);
private:
    unsigned int m_name;
    unsigned int n_numAttachments;
    RHITexture2DPtr m_attachments[11];
    RHITextureInfo m_attachmentsInfo[11];
};

class OpenGLShaderSource : public RHIShaderSource
{
    friend class OpenGLShaderProgram;
public:
    virtual ~OpenGLShaderSource();
    virtual void compileSource(std::string const & sourceStr, ERHIShaderType type);
    virtual unsigned int getName() const { return m_name; }
    virtual ERHIShaderType getType() const { return m_type; }
    virtual std::string const & getSourceStr() const { return m_sourceStr; }
private:
    unsigned int m_name {0};
    ERHIShaderType m_type;
    std::string m_sourceStr;
};

class OpenGLShaderProgram : public RHIShaderProgram
{
public:
    virtual ~OpenGLShaderProgram();
    virtual void create();
    virtual void attachSource(RHIShaderSourcePtr source);
    virtual void link();
    virtual void active();
    virtual std::vector<RHIShaderSourcePtr> & getAttachedSourcesList(){ return m_attachedSources; }
    
    virtual void setUniform1ui(const std::string & ref, unsigned int value);
    virtual void setUniform1i(const std::string & ref, int value);
    virtual void setUniform1f(const std::string & ref, float v1);
    virtual void setUniform2f(const std::string & ref, float v1, float v2);
    virtual void setUniform4f(const std::string & ref, float v1, float v2, float v3, float v4);
    virtual void setUniform4x4m(const std::string & ref, float* mat4x4);
    virtual void setUniform3fv(const std::string & ref, unsigned int numElems, float* vec3array);
    
private:
    int getUniformLocation(const std::string & ref);
    unsigned int m_name;
    std::map<std::string, int> m_uniformNames;
    std::vector<RHIShaderSourcePtr> m_attachedSources;
};

class OpenGLVAO : public RHIVAO
{
public:
    virtual ~OpenGLVAO();
    virtual void render();
    virtual void setup(RHIVertex* vertexList,
                       unsigned int vertexCount);
private:
    unsigned int m_name {0};
    unsigned int m_vertexCount {0};
    unsigned int m_meshVBO {0};
};

class OpenGLUVRegionVAO : public RHIUVRegionVAO
{
public:
    virtual ~OpenGLUVRegionVAO();
    virtual void render();
    virtual void setup(RHIUVRegionVertex* vertexList,
                       size_t vertexCount);
private:
    unsigned int m_name {0};
    int m_vertexCount {0};
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
    
    virtual RHIVAOPtr createVertexBufferObject();
    virtual RHIUVRegionVAOPtr createUVRegionVAO();
    virtual RHIShaderProgramPtr createProgram();
    virtual RHIShaderSourcePtr createSource();
    virtual RHITexture2DPtr createTexture();
    virtual RHICubeMapPtr createCubeMap();
    virtual RHIFrameBufferPtr createFramebuffer();
    
    //add a new shader header source
    virtual void addShaderHeader(std::string const & headerName, std::string const & headerContent);
    
    //replaces all hader included in shder source file by it's content
    virtual std::string solveShaderHeaders(std::string const & shaderSource);
private:
    unsigned int m_defaultVertexArray;
    std::map<std::string, std::string> m_shaderHeaders;
    
};

} //namespace gtf
