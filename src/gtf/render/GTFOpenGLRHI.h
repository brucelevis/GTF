//
//  MPOpenGLRHI.h
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#pragma once

#include "GTFRHI.h"
#include <map>
#include <vector>

class MPOpenGLTexture : public GTFRHITexture2D
{
public:
    virtual ~MPOpenGLTexture();
    virtual void create();
    virtual void setup(EGTFRHITexInternalFormat inFormat,
                       unsigned int bufferW,
                       unsigned int bufferH,
                       EGTFRHITexFormat format,
                       EGTFRHIValueType type,
                       const void* buffer = nullptr,
                       const GTFRHITexParam* texParams = nullptr,
                       unsigned int numParams = 0);
    virtual void bindAt(unsigned int texUnit);
    virtual void unbind();
    virtual unsigned int getName() const;
    virtual void getInfo(GTFRHITextureInfo & info) const;
    virtual void copyToBuffer(void* buffer, EGTFRHITexFormat format, EGTFRHIValueType valType);
    virtual void copyFromBuffer(void* buffer);
    virtual void clearWithColor(float r, float g = 0.0f, float b = 0.0f, float a = 0.0f);
    virtual GTFRHITexture2DPtr cloneTexture(bool withData = false);
    virtual void flipY();
private:
    unsigned int m_name;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_lastBind {0};
    EGTFRHITexInternalFormat m_internalFormat;
    EGTFRHITexFormat m_format;
    EGTFRHIValueType m_type;

};

class MPOpenGLCubeMap : public GTFRHICubeMap
{
public:
    virtual ~MPOpenGLCubeMap();
    virtual void create();
    virtual void setup(GTFRHITexture2DPtr * faces, int lodLvl);
    virtual void bindAt(unsigned int texUnit) ;
    virtual void unbind();
    virtual GTFRHITexture2DPtr getSide(unsigned int lodLvl, unsigned int side) { return m_sides[lodLvl][side]; };
private:
    unsigned int m_name;
    unsigned int m_lastBind {0};
    GTFRHITexture2DPtr m_sides[6][6];
};

class MPOpenGLFrameBuffer : public GTFRHIFrameBuffer
{
public:
    virtual ~MPOpenGLFrameBuffer();
    virtual void create();
    virtual void setup(GTFRHITexture2DPtr* attachments,
                       unsigned int numAttachments = 1,
                       GTFRHITexture2DPtr depthAttach = nullptr);
    virtual void bindForDrawing();
    virtual void bindForReading();
    virtual void unbindForDrawing();
    virtual void unbindForReading();
    virtual unsigned int getName() const;
    virtual void getPixelAt(int x, int y, unsigned int attachIndex, void* buffer);
    virtual void copyAttachmentToTexture(unsigned int attachment, GTFRHITexture2DPtr outputTex);
    virtual void clearAttachment(unsigned int attachIndex,
                                 float r = 0.0f,
                                 float g = 0.0f,
                                 float b = 0.0f,
                                 float a = 0.0f);
    virtual void clearDepthBuffer(float d = 1.0f);
private:
    unsigned int m_name;
    unsigned int n_numAttachments;
    GTFRHITexture2DPtr m_attachments[11];
    GTFRHITextureInfo m_attachmentsInfo[11];
};

class MPOpenGLShaderSource : public GTFRHIShaderSource
{
    friend class MPOpenGLShaderProgram;
public:
    virtual ~MPOpenGLShaderSource();
    virtual void compileSource(std::string const & sourceStr, EGTFRHIShaderType type);
    virtual unsigned int getName() const { return m_name; }
    virtual EGTFRHIShaderType getType() const { return m_type; }
    virtual std::string const & getSourceStr() const { return m_sourceStr; }
private:
    unsigned int m_name {0};
    EGTFRHIShaderType m_type;
    std::string m_sourceStr;
};

class MPOpenGLShaderProgram : public GTFRHIShaderProgram
{
public:
    virtual ~MPOpenGLShaderProgram();
    virtual void create();
    virtual void attachSource(GTFRHIShaderSourcePtr source);
    virtual void link();
    virtual void active();
    virtual std::vector<GTFRHIShaderSourcePtr> & getAttachedSourcesList(){ return m_attachedSources; }
    
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
    std::vector<GTFRHIShaderSourcePtr> m_attachedSources;
};

class MPOpenGLVAO : public GTFRHIVAO
{
public:
    virtual ~MPOpenGLVAO();
    virtual void render();
    virtual void setup(GTFRHIVertex* vertexList,
                       unsigned int vertexCount);
private:
    unsigned int m_name {0};
    unsigned int m_vertexCount {0};
    unsigned int m_meshVBO {0};
};

class MPOpenGLUVRegionVAO : public GTFRHIUVRegionVAO
{
public:
    virtual ~MPOpenGLUVRegionVAO();
    virtual void render();
    virtual void setup(GTFRHIUVRegionVertex* vertexList,
                       size_t vertexCount);
private:
    unsigned int m_name {0};
    int m_vertexCount {0};
    unsigned int m_meshVBO {0};
};



class MPOpenGLRHI : public GTFRHI
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
    virtual void setBlendFnc(EGTFRHIBlendFnc colorSrcFactor = EGTFRHIBlendFnc::RHI_SRC_ALPHA,
                             EGTFRHIBlendFnc colorDstFactor = EGTFRHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA,
                             EGTFRHIBlendFnc alphaSrcFactor = EGTFRHIBlendFnc::RHI_SRC_ALPHA,
                             EGTFRHIBlendFnc alphaDstFactor = EGTFRHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA);
    virtual void setBlendEquation(EGTFRHIBlendEquation colorEq = EGTFRHIBlendEquation::RHI_FUNC_ADD,
                                  EGTFRHIBlendEquation alphaEq = EGTFRHIBlendEquation::RHI_FUNC_ADD);
    
    virtual GTFRHIVAOPtr createVertexBufferObject();
    virtual GTFRHIUVRegionVAOPtr createUVRegionVAO();
    virtual GTFRHIShaderProgramPtr createProgram();
    virtual GTFRHIShaderSourcePtr createSource();
    virtual GTFRHITexture2DPtr createTexture();
    virtual GTFRHICubeMapPtr createCubeMap();
    virtual GTFRHIFrameBufferPtr createFramebuffer();
    
    //add a new shader header source
    virtual void addShaderHeader(std::string const & headerName, std::string const & headerContent);
    
    //replaces all hader included in shder source file by it's content
    virtual std::string solveShaderHeaders(std::string const & shaderSource);
private:
    unsigned int m_defaultVertexArray;
    std::map<std::string, std::string> m_shaderHeaders;
    
};


