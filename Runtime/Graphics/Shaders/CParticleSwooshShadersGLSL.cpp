#include "CParticleSwooshShaders.hpp"
#include "Particle/CParticleSwoosh.hpp"
#include "Particle/CSwooshDescription.hpp"

namespace urde
{

static const char* VS =
"#version 330\n"
BOO_GLSL_BINDING_HEAD
"layout(location=0) in vec4 posIn;\n"
"layout(location=1) in vec4 uvIn;\n"
"layout(location=2) in vec4 colorIn;\n"
"\n"
"UBINDING0 uniform SwooshUniform\n"
"{\n"
"    mat4 mvp;\n"
"};\n"
"\n"
"struct VertToFrag\n"
"{\n"
"    vec4 color;\n"
"    vec2 uv;\n"
"};\n"
"\n"
"SBINDING(0) out VertToFrag vtf;\n"
"void main()\n"
"{\n"
"    vtf.color = colorIn;\n"
"    vtf.uv = uvIn;\n"
"    gl_Position = mvp * vec4(posIn.xyz, 1.0);\n"
"}\n";

static const char* FS_TEX =
"#version 330\n"
BOO_GLSL_BINDING_HEAD
"struct VertToFrag\n"
"{\n"
"    vec4 color;\n"
"    vec2 uv;\n"
"};\n"
"\n"
"SBINDING(0) in VertToFrag vtf;\n"
"layout(location=0) out vec4 colorOut;\n"
"TBINDING0 uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"    colorOut = vtf.color * texture(tex, vtf.uv);\n"
"}\n";

static const char* FS_NOTEX =
"#version 330\n"
BOO_GLSL_BINDING_HEAD
"struct VertToFrag\n"
"{\n"
"    vec4 color;\n"
"    vec2 uv;\n"
"};\n"
"\n"
"SBINDING(0) in VertToFrag vtf;\n"
"layout(location=0) out vec4 colorOut;\n"
"void main()\n"
"{\n"
"    colorOut = vtf.color;\n"
"}\n";

struct OGLParticleSwooshDataBindingFactory : TShader<CParticleSwooshShaders>::IDataBindingFactory
{
    boo::IShaderDataBinding* BuildShaderDataBinding(boo::IGraphicsDataFactory::Context& ctx,
                                                    CParticleSwooshShaders& shaders)
    {
        CParticleSwoosh& gen = shaders.m_gen;
        CSwooshDescription* desc = gen.GetDesc();

        CUVElement* texr = desc->x3c_TEXR.get();
        boo::ITexture* textures[] = {texr->GetValueTexture(0).GetObj()->GetBooTexture()};

        const boo::VertexElementDescriptor VtxFmt[] =
        {
            {gen.m_vertBuf, nullptr, boo::VertexSemantic::Position4},
            {gen.m_vertBuf, nullptr, boo::VertexSemantic::UV4},
            {gen.m_vertBuf, nullptr, boo::VertexSemantic::Color},
        };
        boo::IVertexFormat* vtxFmt = ctx.newVertexFormat(3, VtxFmt);

        boo::IGraphicsBuffer* uniforms[] = {gen.m_uniformBuf};
        gen.m_dataBind = ctx.newShaderDataBinding(shaders.m_pipeline, vtxFmt, gen.m_vertBuf,
                                                  nullptr, nullptr, 1, uniforms,
                                                  nullptr, texr ? 1 : 0, textures, nullptr, nullptr);
        return nullptr;
    }
};

static const char* UniNames[] = {"SwooshUniform"};
static const char* TexNames[] = {"tex"};

TShader<CParticleSwooshShaders>::IDataBindingFactory* CParticleSwooshShaders::Initialize(boo::GLDataFactory::Context& ctx)
{
    m_texZWrite = ctx.newShaderPipeline(VS, FS_TEX, 1, TexNames, 1, UniNames,
                                        boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                        boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                        true, false, boo::CullMode::None);
    m_texNoZWrite = ctx.newShaderPipeline(VS, FS_TEX, 1, TexNames, 1, UniNames,
                                          boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                          boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                          true, false, boo::CullMode::None);
    m_texAdditiveZWrite = ctx.newShaderPipeline(VS, FS_TEX, 1, TexNames, 1, UniNames,
                                                boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                                true, false, boo::CullMode::None);
    m_texAdditiveNoZWrite = ctx.newShaderPipeline(VS, FS_TEX, 1, TexNames, 1, UniNames,
                                                  boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                  boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                                  true, false, boo::CullMode::None);

    m_noTexZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, 0, nullptr, 1, UniNames,
                                          boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                          boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                          true, false, boo::CullMode::None);
    m_noTexNoZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, 0, nullptr, 1, UniNames,
                                            boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                            boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                            true, false, boo::CullMode::None);
    m_noTexAdditiveZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, 0, nullptr, 1, UniNames,
                                                  boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                  boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                                  true, false, boo::CullMode::None);
    m_noTexAdditiveNoZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, 0, nullptr, 1, UniNames,
                                                    boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                    boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                                    true, false, boo::CullMode::None);

    return new struct OGLParticleSwooshDataBindingFactory;
}

#if BOO_HAS_VULKAN
struct VulkanParticleSwooshDataBindingFactory : TShader<CParticleSwooshShaders>::IDataBindingFactory
{
    boo::IShaderDataBinding* BuildShaderDataBinding(boo::IGraphicsDataFactory::Context& ctx,
                                                    CParticleSwooshShaders& shaders)
    {
        CParticleSwoosh& gen = shaders.m_gen;
        CSwooshDescription* desc = gen.GetDesc();

        CUVElement* texr = desc->x3c_TEXR.get();
        boo::ITexture* textures[] = {texr->GetValueTexture(0).GetObj()->GetBooTexture()};

        boo::IGraphicsBuffer* uniforms[] = {gen.m_uniformBuf};
        gen.m_dataBind = ctx.newShaderDataBinding(shaders.m_pipeline, CParticleSwooshShaders::m_vtxFormat,
                                                  gen.m_vertBuf, nullptr, nullptr, 1, uniforms,
                                                  nullptr, texr ? 1 : 0, textures, nullptr, nullptr);
        return nullptr;
    }
};

TShader<CParticleSwooshShaders>::IDataBindingFactory* CParticleSwooshShaders::Initialize(boo::VulkanDataFactory::Context& ctx)
{
    static const boo::VertexElementDescriptor VtxFmt[] =
    {
        {nullptr, nullptr, boo::VertexSemantic::Position4},
        {nullptr, nullptr, boo::VertexSemantic::UV4},
        {nullptr, nullptr, boo::VertexSemantic::Color},
    };
    m_vtxFormat = ctx.newVertexFormat(3, VtxFmt);

    m_texZWrite = ctx.newShaderPipeline(VS, FS_TEX, m_vtxFormat,
                                        boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                        boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                        true, false, boo::CullMode::None);
    m_texNoZWrite = ctx.newShaderPipeline(VS, FS_TEX, m_vtxFormat,
                                          boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                          boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                          true, false, boo::CullMode::None);
    m_texAdditiveZWrite = ctx.newShaderPipeline(VS, FS_TEX, m_vtxFormat,
                                                boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                                true, false, boo::CullMode::None);
    m_texAdditiveNoZWrite = ctx.newShaderPipeline(VS, FS_TEX, m_vtxFormat,
                                                  boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                  boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                                  true, false, boo::CullMode::None);

    m_noTexZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, m_vtxFormat,
                                          boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                          boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                          true, false, boo::CullMode::None);
    m_noTexNoZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, m_vtxFormat,
                                            boo::BlendFactor::SrcAlpha, boo::BlendFactor::InvSrcAlpha,
                                            boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                            true, false, boo::CullMode::None);
    m_noTexAdditiveZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, m_vtxFormat,
                                                  boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                  boo::Primitive::TriStrips, boo::ZTest::LEqual, true,
                                                  true, false, boo::CullMode::None);
    m_noTexAdditiveNoZWrite = ctx.newShaderPipeline(VS, FS_NOTEX, m_vtxFormat,
                                                    boo::BlendFactor::SrcAlpha, boo::BlendFactor::One,
                                                    boo::Primitive::TriStrips, boo::ZTest::LEqual, false,
                                                    true, false, boo::CullMode::None);

    return new struct VulkanParticleSwooshDataBindingFactory;
}
#endif

}