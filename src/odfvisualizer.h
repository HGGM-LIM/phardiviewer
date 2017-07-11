#ifndef ODFVISUALIZER_H
#define ODFVISUALIZER_H

#include <QMainWindow>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QConeMesh>

#include <QPropertyAnimation>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include "pervertexcolormaterialmodified.h"
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QCameraLens>

#include <QElapsedTimer>
#include <QTimer>
#include <QCheckBox>
#include <QCommandLinkButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QSpinBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QFont>

#include "femmesh.h"
#include "femlines.h"
#include "orbitcameracontrollermodified.h"

#include "odfgenerator.h"
#include "slicemesh.h"
#include "image.h"

class ODFvisualizer : public QMainWindow
{
    Q_OBJECT
public:
    explicit ODFvisualizer(QWidget *parent = 0);
    ~ODFvisualizer();

    void start();
    void getMeshPosition(QVector3D *center, float *maxDistX, float *maxDistY, float *maxDistZ);
    void setSceneLighting();

private:
    void cleanScene();

    Qt3DRender::QCamera *cameraEntity;

    Qt3DCore::QEntity *rootEntity;

    Qt3DCore::QEntity *m_maskODFSliceEntity;
    Qt3DCore::QEntity *m_xODFSliceEntity;
    Qt3DCore::QEntity *m_yODFSliceEntity;
    Qt3DCore::QEntity *m_zODFSliceEntity;
    Qt3DCore::QEntity *m_xScalarSliceEntity;
    Qt3DCore::QEntity *m_yScalarSliceEntity;
    Qt3DCore::QEntity *m_zScalarSliceEntity;

    ODFGenerator *generator;

    QVector3D meshCenter;
    float maxDistX;
    float maxDistY;
    float maxDistZ;

    QString lastDir;

    QString odfFileName;
    QString maskFileName;
    QString scalarFileName;
    QString reconstFacesFileName;
    QString reconstVerticesFileName;

    // buttons and widgets
    QPushButton *loadODFFileButton;
    QCheckBox *xSliceCB;
    QCheckBox *ySliceCB;
    QCheckBox *zSliceCB;
    QCheckBox *xSliceBgCB;
    QCheckBox *ySliceBgCB;
    QCheckBox *zSliceBgCB;
    QSpinBox *xSliceSpinBox;
    QSpinBox *ySliceSpinBox;
    QSpinBox *zSliceSpinBox;
    QPushButton *processButton;
    QPushButton *loadScalarFileButton;
    QPushButton *loadMaskFileButton;
    QPushButton *loadReconstFacesFileButton;
    QPushButton *loadReconstVerticesFileButton;
    QCheckBox *duplicateSchemeCB;

signals:

public slots:

private slots:
    void loadODFButtonClicked();
    void loadScalarButtonClicked();
    void loadMaskButtonClicked();
    void loadReconstFacesButtonClicked();
    void loadReconstVerticesButtonClicked();
    void generateODFView();
};

#endif // ODFVISUALIZER_H
