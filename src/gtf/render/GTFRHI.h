//
//  GTFRHI.h
//  MeshPainter
//
//  Created by David Gallardo on 17/10/15.
//  Copyright (c) 2015 David Gallardo. All rights reserved.
//

#pragma once

#include <stdlib.h>
#include <memory>
#include <string>
#include <vector>

enum EGTFRHIShaderType
{
    RHI_VERTEX_SHADER = 0,
    RHI_FRAGMENT_SHADER = 1
};

enum EGTFRHITexTarget
{
    RHI_TEXTURE_1D = 0,
    RHI_TEXTURE_2D = 1
};

enum EGTFRHITexInternalFormat
{
    RHI_RGB8 = 0,
    RHI_RGBA8,
    RHI_DEPTH32,
    RHI_RGBA32F,
    RHI_RGB32F,
    RHI_RGBA16F,
    RHI_RGB16F,
    RHI_RG16F,
    RHI_R16F,
    RHI_R8,
    RHI_DEPTH8
};

enum EGTFRHITexFormat
{
    RHI_DEPTH_COMPONENT = 0,
    RHI_DEPTH_STENCIL = 1,
    RHI_RED = 2,
    RHI_RG = 3,
    RHI_RGB = 4,
    RHI_RGBA = 5,
    RHI_BGR,
    RHI_BGRA
};

enum EGTFRHIValueType
{
    RHI_FLOAT = 0,
    RHI_INT = 1,
    RHI_UNSIGNED_BYTE = 2
};

enum EGTFRHIBlendFnc
{
    RHI_ZERO = 0,
    RHI_ONE = 1,
    RHI_SRC_COLOR = 2,
    RHI_ONE_MINUS_SRC_COLOR = 3,
    RHI_DST_COLOR = 4,
    RHI_ONE_MINUS_DST_COLOR = 5,
    RHI_SRC_ALPHA = 6,
    RHI_ONE_MINUS_SRC_ALPHA = 7,
    RHI_DST_ALPHA = 8,
    RHI_ONE_MINUS_DST_ALPHA = 9,
    RHI_CONSTANT_COLOR = 10,
    RHI_ONE_MINUS_CONSTANT_COLOR = 11,
    RHI_CONSTANT_ALPHA = 12,
    RHI_ONE_MINUS_CONSTANT_ALPHA = 13,
    RHI_SRC_ALPHA_SATURATE = 14
};

enum EGTFRHIBlendEquation
{
    RHI_FUNC_ADD = 0,
    RHI_FUNC_SUBTRACT = 1,
    RHI_FUNC_REVERSE_SUBTRACT = 2,
    RHI_MIN = 3,
    RHI_MAX = 4
};

enum EGTFRHITexParamName
{
    RHI_TEXTURE_MIN_FILTER = 5,
    RHI_TEXTURE_MAG_FILTER = 6,
    RHI_TEXTURE_WRAP_S = 14,
    RHI_TEXTURE_WRAP_T = 15,
};

enum EGTFRHITexParamValues
{
    RHI_NEAREST = 1,
    RHI_LINEAR = 2,
    RHI_NEAREST_MIPMAP_NEAREST = 3,
    RHI_LINEAR_MIPMAP_NEAREST = 4,
    RHI_NEAREST_MIPMAP_LINEAR = 5,
    RHI_LINEAR_MIPMAP_LINEAR = 6,
    RHI_CLAMP_TO_EDGE = 7,
    RHI_MIRRORED_REPEAT = 8,
    RHI_REPEAT = 9
};

enum EGTFRHICompareFunc
{
    RHI_LEQUAL = 0,
    RHI_GEQUAL = 1,
    RHI_LESS = 2,
    RHI_GREATER = 3,
    RHI_EQUAL = 4,
    RHI_NOTEQUAL = 5,
    RHI_ALWAYS = 6,
    RHI_NEVER = 7
};

struct GTFRHIVertex
{
    float x = 0, y = 0, z = 0;          //position
    float nx = 0, ny = 0, nz = 0;       //normal
    float tcx = 0, tcy = 0;             //texcoord
    float tnx = 0, tny = 0, tnz = 0;    //tangent
    float bnx = 0, bny = 0, bnz = 0;    //binormal
};

struct GTFRHIUVRegionVertex
{
    float x, y;
    float faceId, regionId;
};

struct GTFRHITextureInfo
{
    EGTFRHITexInternalFormat internalFormat;
    unsigned int bufferW;
    unsigned int bufferH;
    EGTFRHITexFormat format;
    EGTFRHIValueType type;
    unsigned int size;
};

struct GTFRHITexParam
{
    EGTFRHITexParamName name;
    EGTFRHITexParamValues value;
};

struct GTFRHITexturePreset : public GTFRHITextureInfo
{
    const void* buffer = nullptr;
    GTFRHITexParam texParams[4];
    unsigned int numParams = 0;
};

typedef std::shared_ptr<class GTFRHITexture2D> GTFRHITexture2DPtr;
class GTFRHITexture2D
{
public:
    virtual ~GTFRHITexture2D(){};
    virtual void create() = 0;
    void setup(GTFRHITextureInfo const & info);
    void setup(GTFRHITexturePreset const & preset);
    virtual void setup(EGTFRHITexInternalFormat inFormat,
                       unsigned int bufferW,
                       unsigned int bufferH,
                       EGTFRHITexFormat format,
                       EGTFRHIValueType type,
                       const void* buffer = nullptr,
                       const GTFRHITexParam* texParams = nullptr,
                       unsigned int numParams = 0) = 0;
    virtual void bindAt(unsigned int texUnit) = 0;
    virtual void unbind() = 0;
    virtual unsigned int getName() const = 0;
    virtual void getInfo(GTFRHITextureInfo & info) const = 0;
    virtual void copyToBuffer(void* buffer, EGTFRHITexFormat format, EGTFRHIValueType valType) = 0;
    virtual void copyFromBuffer(void* buffer) = 0;
    virtual void clearWithColor(float r, float g = 0.0f, float b = 0.0f, float a = 0.0f) = 0;
    virtual GTFRHITexture2DPtr cloneTexture(bool withData = false) = 0;
    virtual void flipY() = 0;
};

typedef std::shared_ptr<class GTFRHICubeMap> GTFRHICubeMapPtr;
class GTFRHICubeMap
{
public:
    virtual ~GTFRHICubeMap(){};
    virtual void create() = 0;
    virtual void setup(GTFRHITexture2DPtr * faces, int lodLvl) = 0;
    virtual void bindAt(unsigned int texUnit) = 0;
    virtual void unbind() = 0;
    virtual GTFRHITexture2DPtr getSide(unsigned int lodLvl, unsigned int side) = 0;
};

class GTFRHIFrameBuffer
{
public:
    virtual ~GTFRHIFrameBuffer(){};
    virtual void create() = 0;
    virtual void setup(GTFRHITexture2DPtr* attachments,
                       unsigned int numAttachments = 1,
                       GTFRHITexture2DPtr depthAttach = nullptr) = 0;
    virtual void bindForDrawing() = 0;
    virtual void bindForReading() = 0;
    virtual void unbindForDrawing() = 0;
    virtual void unbindForReading() = 0;
    virtual unsigned int getName() const = 0;
    virtual void getPixelAt(int x, int y, unsigned int attachIndex, void* buffer) = 0;
    virtual void copyAttachmentToTexture(unsigned int attachment, GTFRHITexture2DPtr outputTex) = 0;
    virtual void clearAttachment(unsigned int attachIndex,
                                 float r = 0.0f,
                                 float g = 0.0f,
                                 float b = 0.0f,
                                 float a = 0.0f) = 0;
    
    virtual void clearDepthBuffer(float d = 1.0f) = 0;
};

typedef std::shared_ptr<GTFRHIFrameBuffer> GTFRHIFrameBufferPtr;

class GTFRHIShaderSource
{
public:
    virtual ~GTFRHIShaderSource(){};
    virtual void compileSource(std::string const & sourceStr, EGTFRHIShaderType type) = 0;
    virtual unsigned int getName() const = 0;
    virtual EGTFRHIShaderType getType() const = 0;
    virtual std::string const & getSourceStr() const = 0;
};

typedef std::shared_ptr<GTFRHIShaderSource> GTFRHIShaderSourcePtr;

class GTFRHIShaderProgram
{
public:
    virtual ~GTFRHIShaderProgram(){};
    virtual void create() = 0;
    virtual void attachSource(GTFRHIShaderSourcePtr source) = 0;
    virtual void link() = 0;
    virtual void active() = 0;
    virtual std::vector<GTFRHIShaderSourcePtr> & getAttachedSourcesList() = 0;
    
    virtual void setUniform1ui(const std::string & ref, unsigned int value) = 0;
    virtual void setUniform1i(const std::string & ref, int value) = 0;
    virtual void setUniform1f(const std::string & ref, float v1) = 0;
    virtual void setUniform2f(const std::string & ref, float v1, float v2) = 0;
    virtual void setUniform4f(const std::string & ref, float v1, float v2, float v3, float v4) = 0;
    virtual void setUniform4x4m(const std::string & ref, float* mat4x4) = 0;
    virtual void setUniform3fv(const std::string & ref, unsigned int numElems, float* vec3array) = 0;
};

typedef std::shared_ptr<GTFRHIShaderProgram> GTFRHIShaderProgramPtr;

class GTFRHIVAO
{
public:
    virtual ~GTFRHIVAO(){};
    virtual void render() = 0;
    virtual void setup(GTFRHIVertex* vertexList,
                       unsigned int vertexCount) = 0;
};

typedef std::shared_ptr<GTFRHIVAO> GTFRHIVAOPtr;

class GTFRHIUVRegionVAO
{
public:
    virtual ~GTFRHIUVRegionVAO(){};
    virtual void render() = 0;
    virtual void setup(GTFRHIUVRegionVertex* vertexList,
                       size_t vertexCount) = 0;
};

typedef std::shared_ptr<GTFRHIUVRegionVAO> GTFRHIUVRegionVAOPtr;

class GTFRHI
{
public:
    static GTFRHI* instance;
    
    virtual int init() = 0;
    
    //render api methods
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clearColorAndDepthBuffers() = 0;
    virtual void viewport(int x, int y, int w, int h) = 0;
    virtual void setDepthTest(bool enabled) = 0;
    virtual void setBackfaceCulling(bool enabled) = 0;
    virtual void emitVertexCommand(unsigned int numVertex) = 0;
    virtual void setBlend(bool enabled) = 0;
    virtual void setBlendFnc(EGTFRHIBlendFnc colorSrcFactor = EGTFRHIBlendFnc::RHI_SRC_ALPHA,
                             EGTFRHIBlendFnc colorDstFactor = EGTFRHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA,
                             EGTFRHIBlendFnc alphaSrcFactor = EGTFRHIBlendFnc::RHI_SRC_ALPHA,
                             EGTFRHIBlendFnc alphaDstFactor = EGTFRHIBlendFnc::RHI_ONE_MINUS_SRC_ALPHA) = 0;
    virtual void setBlendEquation(EGTFRHIBlendEquation colorEq = EGTFRHIBlendEquation::RHI_FUNC_ADD,
                                  EGTFRHIBlendEquation alphaEq = EGTFRHIBlendEquation::RHI_FUNC_ADD) = 0;
    
    //factory methods
    virtual GTFRHIVAOPtr createVertexBufferObject() = 0;
    virtual GTFRHIUVRegionVAOPtr createUVRegionVAO() = 0;
    virtual GTFRHIShaderProgramPtr createProgram() = 0;
    virtual GTFRHIShaderSourcePtr createSource() = 0;
    virtual GTFRHITexture2DPtr createTexture() = 0;
    virtual GTFRHICubeMapPtr createCubeMap() = 0;
    virtual GTFRHIFrameBufferPtr createFramebuffer() = 0;
    
    //shader utils
    //add a new shader header source
    virtual void addShaderHeader(std::string const & headerName, std::string const & headerContent) = 0;
    //replaces all hader included in shder source file by it's content
    virtual std::string solveShaderHeaders(std::string const & shaderSource) = 0;
};

extern GTFRHI* RHI;
