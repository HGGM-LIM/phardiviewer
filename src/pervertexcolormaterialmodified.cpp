#include "pervertexcolormaterialmodified.h"

PerVertexColorMaterialModified::PerVertexColorMaterialModified(Qt3DCore::QNode *parent)
    : Qt3DRender::QMaterial(parent)
    , m_vertexEffect(new Qt3DRender::QEffect())
    , m_ambientParameter(new Qt3DRender::QParameter(QStringLiteral("ka"), QColor::fromRgbF(0.05f, 0.05f, 0.05f, 1.0f)))
    , m_diffuseParameter(new Qt3DRender::QParameter(QStringLiteral("kd"), QColor::fromRgbF(0.7f, 0.7f, 0.7f, 1.0f)))
    , m_specularParameter(new Qt3DRender::QParameter(QStringLiteral("ks"), QColor::fromRgbF(0.01f, 0.01f, 0.01f, 1.0f)))
    , m_shininessParameter(new Qt3DRender::QParameter(QStringLiteral("shininess"), 150.0f))
    , m_alphaParameter(new Qt3DRender::QParameter(QStringLiteral("alpha"), 1.0f))
    , m_vertexGL3Technique(new Qt3DRender::QTechnique())
    , m_vertexGL2Technique(new Qt3DRender::QTechnique())
    , m_vertexES2Technique(new Qt3DRender::QTechnique())
    , m_vertexGL3RenderPass(new Qt3DRender::QRenderPass())
    , m_vertexGL2RenderPass(new Qt3DRender::QRenderPass())
    , m_vertexES2RenderPass(new Qt3DRender::QRenderPass())
    , m_vertexGL3Shader(new Qt3DRender::QShaderProgram())
    , m_vertexGL2ES2Shader(new Qt3DRender::QShaderProgram())
    , m_noDepthMask(new Qt3DRender::QNoDepthMask())
    , m_blendState(new Qt3DRender::QBlendEquationArguments())
    , m_blendEquation(new Qt3DRender::QBlendEquation())
    , m_filterKey(new Qt3DRender::QFilterKey)
{
    init();
}

void PerVertexColorMaterialModified::init()
{

    connect(m_ambientParameter, &Qt3DRender::QParameter::valueChanged,
            this, &PerVertexColorMaterialModified::handleAmbientChanged);
    connect(m_diffuseParameter, &Qt3DRender::QParameter::valueChanged,
            this, &PerVertexColorMaterialModified::handleDiffuseChanged);
    connect(m_specularParameter, &Qt3DRender::QParameter::valueChanged,
            this, &PerVertexColorMaterialModified::handleSpecularChanged);
    connect(m_shininessParameter, &Qt3DRender::QParameter::valueChanged,
            this, &PerVertexColorMaterialModified::handleShininessChanged);
    connect(m_alphaParameter, &Qt3DRender::QParameter::valueChanged,
            this, &PerVertexColorMaterialModified::handleAlphaChanged);
    connect(m_blendEquation, &Qt3DRender::QBlendEquation::blendFunctionChanged,
                     this, &PerVertexColorMaterialModified::blendFunctionArgChanged);
    connect(m_blendState, &Qt3DRender::QBlendEquationArguments::destinationAlphaChanged,
                     this, &PerVertexColorMaterialModified::destinationAlphaArgChanged);
    connect(m_blendState, &Qt3DRender::QBlendEquationArguments::destinationRgbChanged,
                     this, &PerVertexColorMaterialModified::destinationRgbArgChanged);
    connect(m_blendState, &Qt3DRender::QBlendEquationArguments::sourceAlphaChanged,
                     this, &PerVertexColorMaterialModified::sourceAlphaArgChanged);
    connect(m_blendState, &Qt3DRender::QBlendEquationArguments::sourceRgbChanged,
                     this, &PerVertexColorMaterialModified::sourceRgbArgChanged);

    m_vertexGL3Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/custom.vert")));
    m_vertexGL3Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/custom.frag")));
    m_vertexGL2ES2Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/custom.vert")));
    m_vertexGL2ES2Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/custom.frag")));

    m_vertexGL3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_vertexGL3Technique->graphicsApiFilter()->setMajorVersion(3);
    m_vertexGL3Technique->graphicsApiFilter()->setMinorVersion(1);
    m_vertexGL3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    m_vertexGL2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_vertexGL2Technique->graphicsApiFilter()->setMajorVersion(2);
    m_vertexGL2Technique->graphicsApiFilter()->setMinorVersion(0);
    m_vertexGL2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);

    m_vertexES2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
    m_vertexES2Technique->graphicsApiFilter()->setMajorVersion(2);
    m_vertexES2Technique->graphicsApiFilter()->setMinorVersion(0);
    m_vertexES2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);

    m_filterKey->setParent(this);
    m_filterKey->setName(QStringLiteral("renderingStyle"));
    m_filterKey->setValue(QStringLiteral("forward"));

    m_vertexGL3Technique->addFilterKey(m_filterKey);
    m_vertexGL2Technique->addFilterKey(m_filterKey);
    m_vertexES2Technique->addFilterKey(m_filterKey);

//    m_blendState->setSourceRgb(Qt3DRender::QBlendEquationArguments::SourceAlpha);
//    m_blendState->setDestinationRgb(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
//    m_blendEquation->setBlendFunction(Qt3DRender::QBlendEquation::Add);

    m_vertexGL3RenderPass->setShaderProgram(m_vertexGL3Shader);
    m_vertexGL2RenderPass->setShaderProgram(m_vertexGL2ES2Shader);
    m_vertexES2RenderPass->setShaderProgram(m_vertexGL2ES2Shader);

//    m_vertexGL3RenderPass->addRenderState(m_noDepthMask);
//    m_vertexGL3RenderPass->addRenderState(m_blendState);
//    m_vertexGL3RenderPass->addRenderState(m_blendEquation);

//    m_vertexGL2RenderPass->addRenderState(m_noDepthMask);
//    m_vertexGL2RenderPass->addRenderState(m_blendState);
//    m_vertexGL2RenderPass->addRenderState(m_blendEquation);

//    m_vertexES2RenderPass->addRenderState(m_noDepthMask);
//    m_vertexES2RenderPass->addRenderState(m_blendState);
//    m_vertexES2RenderPass->addRenderState(m_blendEquation);

    m_vertexGL3Technique->addRenderPass(m_vertexGL3RenderPass);
    m_vertexGL2Technique->addRenderPass(m_vertexGL2RenderPass);
    m_vertexES2Technique->addRenderPass(m_vertexES2RenderPass);

    m_vertexEffect->addTechnique(m_vertexGL3Technique);
    m_vertexEffect->addTechnique(m_vertexGL2Technique);
    m_vertexEffect->addTechnique(m_vertexES2Technique);

    m_vertexEffect->addParameter(m_ambientParameter);
    m_vertexEffect->addParameter(m_diffuseParameter);
    m_vertexEffect->addParameter(m_specularParameter);
    m_vertexEffect->addParameter(m_shininessParameter);
    m_vertexEffect->addParameter(m_alphaParameter);

    setEffect(m_vertexEffect);
}

void PerVertexColorMaterialModified::handleAmbientChanged(const QVariant &var)
{
    emit ambientChanged(var.value<QColor>());
}

void PerVertexColorMaterialModified::handleDiffuseChanged(const QVariant &var)
{
    emit diffuseChanged(var.value<QColor>());
}

void PerVertexColorMaterialModified::handleSpecularChanged(const QVariant &var)
{
    emit specularChanged(var.value<QColor>());
}

void PerVertexColorMaterialModified::handleShininessChanged(const QVariant &var)
{
    emit shininessChanged(var.toFloat());
}

void PerVertexColorMaterialModified::handleAlphaChanged(const QVariant &var)
{
    emit alphaChanged(var.toFloat());
}

/*!
    \property PerVertexColorMaterialModified::ambient
    Holds the ambient color.
*/
QColor PerVertexColorMaterialModified::ambient() const
{
    return m_ambientParameter->value().value<QColor>();
}

/*!
    \property PerVertexColorMaterialModified::diffuse
    Holds the diffuse color.
*/
QColor PerVertexColorMaterialModified::diffuse() const
{
    return m_diffuseParameter->value().value<QColor>();
}

/*!
    \property PerVertexColorMaterialModified::specular
    Holds the specular color.
*/
QColor PerVertexColorMaterialModified::specular() const
{
    return m_specularParameter->value().value<QColor>();
}

/*!
    \property PerVertexColorMaterialModified::shininess
    Holds the shininess exponent.
*/
float PerVertexColorMaterialModified::shininess() const
{
    return m_shininessParameter->value().toFloat();
}

/*!
    \property PerVertexColorMaterialModified::alpha
    Holds the alpha component of the object which varies between 0 and 1.
    The default value is 0.5f.
*/
float PerVertexColorMaterialModified::alpha() const
{
    return m_alphaParameter->value().toFloat();
}

/*!
    \property PerVertexColorMaterialModified::sourceRgbArg
    Holds the blend equation source RGB blending argument.
    \sa Qt3DRender::QBlendEquationArguments::Blending
*/
Qt3DRender::QBlendEquationArguments::Blending PerVertexColorMaterialModified::sourceRgbArg() const
{
    return m_blendState->sourceRgb();
}

/*!
    \property PerVertexColorMaterialModified::destinationRgbArg
    Holds the blend equation destination RGB blending argument.
    \sa Qt3DRender::QBlendEquationArguments::Blending
*/
Qt3DRender::QBlendEquationArguments::Blending PerVertexColorMaterialModified::destinationRgbArg() const
{
    return m_blendState->destinationRgb();
}

/*!
    \property PerVertexColorMaterialModified::sourceAlphaArg
    Holds the blend equation source alpha blending argument.
    \sa Qt3DRender::QBlendEquationArguments::Blending
*/
Qt3DRender::QBlendEquationArguments::Blending PerVertexColorMaterialModified::sourceAlphaArg() const
{
    return m_blendState->sourceAlpha();
}

/*!
    \property PerVertexColorMaterialModified::destinationAlphaArg
    Holds the blend equation destination alpha blending argument.
    \sa Qt3DRender::QBlendEquationArguments::Blending
*/
Qt3DRender::QBlendEquationArguments::Blending PerVertexColorMaterialModified::destinationAlphaArg() const
{
    return m_blendState->destinationAlpha();
}

/*!
    \property PerVertexColorMaterialModified::blendFunctionArg
    Holds the blend equation function argument.
    \sa Qt3DRender::QBlendEquation::BlendFunction
*/
Qt3DRender::QBlendEquation::BlendFunction PerVertexColorMaterialModified::blendFunctionArg() const
{
    return m_blendEquation->blendFunction();
}

void PerVertexColorMaterialModified::setAmbient(const QColor &ambient)
{
    m_ambientParameter->setValue(ambient);
}

void PerVertexColorMaterialModified::setDiffuse(const QColor &diffuse)
{
    m_diffuseParameter->setValue(diffuse);
}

void PerVertexColorMaterialModified::setSpecular(const QColor &specular)
{
    m_specularParameter->setValue(specular);
}

void PerVertexColorMaterialModified::setShininess(float shininess)
{
    m_shininessParameter->setValue(shininess);
}

void PerVertexColorMaterialModified::setAlpha(float alpha)
{
    m_alphaParameter->setValue(alpha);
}

void PerVertexColorMaterialModified::setSourceRgbArg(Qt3DRender::QBlendEquationArguments::Blending sourceRgbArg)
{
    m_blendState->setSourceRgb(sourceRgbArg);
}

void PerVertexColorMaterialModified::setDestinationRgbArg(Qt3DRender::QBlendEquationArguments::Blending destinationRgbArg)
{
    m_blendState->setDestinationRgb(destinationRgbArg);
}

void PerVertexColorMaterialModified::setSourceAlphaArg(Qt3DRender::QBlendEquationArguments::Blending sourceAlphaArg)
{
    m_blendState->setSourceAlpha(sourceAlphaArg);
}

void PerVertexColorMaterialModified::setDestinationAlphaArg(Qt3DRender::QBlendEquationArguments::Blending destinationAlphaArg)
{
    m_blendState->setDestinationAlpha(destinationAlphaArg);
}

void PerVertexColorMaterialModified::setBlendFunctionArg(Qt3DRender::QBlendEquation::BlendFunction blendFunctionArg)
{
    m_blendEquation->setBlendFunction(blendFunctionArg);
}
