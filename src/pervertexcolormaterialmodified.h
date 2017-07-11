#ifndef PERVERTEXCOLORMATERIALMODIFIED_H
#define PERVERTEXCOLORMATERIALMODIFIED_H


#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QNoDepthMask>
#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QBlendEquation>

#include <QVector3D>
#include <QVector4D>

#include <QColor>
#include <QUrl>

class PerVertexColorMaterialModified : public Qt3DRender::QMaterial
{
    Q_OBJECT
    Q_PROPERTY(QColor ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(QColor specular READ specular WRITE setSpecular NOTIFY specularChanged)
    Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
    Q_PROPERTY(float alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(Qt3DRender::QBlendEquationArguments::Blending sourceRgbArg READ sourceRgbArg WRITE setSourceRgbArg NOTIFY sourceRgbArgChanged)
    Q_PROPERTY(Qt3DRender::QBlendEquationArguments::Blending destinationRgbArg READ destinationRgbArg WRITE setDestinationRgbArg NOTIFY destinationRgbArgChanged)
    Q_PROPERTY(Qt3DRender::QBlendEquationArguments::Blending sourceAlphaArg READ sourceAlphaArg WRITE setSourceAlphaArg NOTIFY sourceAlphaArgChanged)
    Q_PROPERTY(Qt3DRender::QBlendEquationArguments::Blending destinationAlphaArg READ destinationAlphaArg WRITE setDestinationAlphaArg NOTIFY destinationAlphaArgChanged)
    Q_PROPERTY(Qt3DRender::QBlendEquation::BlendFunction blendFunctionArg READ blendFunctionArg WRITE setBlendFunctionArg NOTIFY blendFunctionArgChanged)

public:
    PerVertexColorMaterialModified(Qt3DCore::QNode *parent = 0);

private:
    Qt3DRender::QEffect *m_vertexEffect;
    Qt3DRender::QParameter *m_ambientParameter;
    Qt3DRender::QParameter *m_diffuseParameter;
    Qt3DRender::QParameter *m_specularParameter;
    Qt3DRender::QParameter *m_shininessParameter;
    Qt3DRender::QParameter *m_alphaParameter;
    Qt3DRender::QTechnique *m_vertexGL3Technique;
    Qt3DRender::QTechnique *m_vertexGL2Technique;
    Qt3DRender::QTechnique *m_vertexES2Technique;
    Qt3DRender::QRenderPass *m_vertexGL3RenderPass;
    Qt3DRender::QRenderPass *m_vertexGL2RenderPass;
    Qt3DRender::QRenderPass *m_vertexES2RenderPass;
    Qt3DRender::QShaderProgram *m_vertexGL3Shader;
    Qt3DRender::QShaderProgram *m_vertexGL2ES2Shader;
    Qt3DRender::QNoDepthMask *m_noDepthMask;
    Qt3DRender::QBlendEquationArguments *m_blendState;
    Qt3DRender::QBlendEquation *m_blendEquation;
    Qt3DRender::QFilterKey *m_filterKey;

    Qt3DRender::QBlendEquationArguments::Blending sourceRgbArg() const;
    Qt3DRender::QBlendEquationArguments::Blending destinationRgbArg() const;
    Qt3DRender::QBlendEquationArguments::Blending sourceAlphaArg() const;
    Qt3DRender::QBlendEquationArguments::Blending destinationAlphaArg() const;
    Qt3DRender::QBlendEquation::BlendFunction blendFunctionArg() const;

    QColor ambient() const;
    QColor diffuse() const;
    QColor specular() const;
    float shininess() const;
    float alpha() const;

    void init();

private slots:
    void handleAmbientChanged(const QVariant &var);
    void handleDiffuseChanged(const QVariant &var);
    void handleSpecularChanged(const QVariant &var);
    void handleShininessChanged(const QVariant &var);
    void handleAlphaChanged(const QVariant &var);

    void setAmbient(const QColor &ambient);
    void setDiffuse(const QColor &diffuse);
    void setSpecular(const QColor &specular);
    void setShininess(float shininess);
    void setAlpha(float alpha);
    void setSourceRgbArg(Qt3DRender::QBlendEquationArguments::Blending sourceRgbArg);
    void setDestinationRgbArg(Qt3DRender::QBlendEquationArguments::Blending destinationRgbArg);
    void setSourceAlphaArg(Qt3DRender::QBlendEquationArguments::Blending sourceAlphaArg);
    void setDestinationAlphaArg(Qt3DRender::QBlendEquationArguments::Blending destinationAlphaArg);
    void setBlendFunctionArg(Qt3DRender::QBlendEquation::BlendFunction blendFunctionArg);

signals:
    void ambientChanged(const QColor &ambient);
    void diffuseChanged(const QColor &diffuse);
    void specularChanged(const QColor &specular);
    void shininessChanged(float shininess);
    void alphaChanged(float alpha);
    void sourceRgbArgChanged(Qt3DRender::QBlendEquationArguments::Blending sourceRgbArg);
    void destinationRgbArgChanged(Qt3DRender::QBlendEquationArguments::Blending destinationRgbArg);
    void sourceAlphaArgChanged(Qt3DRender::QBlendEquationArguments::Blending sourceAlphaArg);
    void destinationAlphaArgChanged(Qt3DRender::QBlendEquationArguments::Blending destinationAlphaArg);
    void blendFunctionArgChanged(Qt3DRender::QBlendEquation::BlendFunction blendFunctionArg);

};

#endif // PERVERTEXCOLORMATERIALMODIFIED_H
