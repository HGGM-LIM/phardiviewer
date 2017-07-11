#include "odfvisualizer.h"

ODFvisualizer::ODFvisualizer(QWidget *parent)
    : QMainWindow(parent)
{
    rootEntity = new Qt3DCore::QEntity;

    m_xODFSliceEntity = nullptr;
    m_yODFSliceEntity = nullptr;
    m_zODFSliceEntity = nullptr;

    m_xScalarSliceEntity = nullptr;
    m_yScalarSliceEntity = nullptr;
    m_zScalarSliceEntity = nullptr;

    lastDir = QDir::homePath();
}

ODFvisualizer::~ODFvisualizer()
{
    rootEntity->deleteLater();
}

void ODFvisualizer::getMeshPosition(QVector3D *center, float *maxDistX, float *maxDistY, float *maxDistZ)
{
    (*center) = meshCenter;
    (*maxDistX) = this->maxDistX;
    (*maxDistY) = this->maxDistY;
    (*maxDistZ) = this->maxDistZ;
}

void ODFvisualizer::setSceneLighting()
{
    QList<QVector3D> positionList;
    positionList.append(QVector3D(0, 0, 200));
    positionList.append(QVector3D(0, 0, -200));
    positionList.append(QVector3D(200, 0, 0));
    positionList.append(QVector3D(-200, 0, 0));
    positionList.append(QVector3D(0, 200, 0));
    positionList.append(QVector3D(0, -200, 0));

    float intensity = 0.4;

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor(Qt::white);
    light->setIntensity(intensity);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(positionList.at(0));
    lightEntity->addComponent(lightTransform);

    Qt3DCore::QEntity *lightEntity2 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light2 = new Qt3DRender::QPointLight(lightEntity2);
    light2->setColor(Qt::white);
    light2->setIntensity(intensity);
    lightEntity2->addComponent(light2);
    Qt3DCore::QTransform *lightTransform2 = new Qt3DCore::QTransform(lightEntity2);
    lightTransform2->setTranslation(positionList.at(1));
    lightEntity2->addComponent(lightTransform2);

    Qt3DCore::QEntity *lightEntity3 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light3 = new Qt3DRender::QPointLight(lightEntity3);
    light3->setColor(Qt::white);
    light3->setIntensity(intensity);
    lightEntity3->addComponent(light3);
    Qt3DCore::QTransform *lightTransform3 = new Qt3DCore::QTransform(lightEntity3);
    lightTransform3->setTranslation(positionList.at(2));
    lightEntity3->addComponent(lightTransform3);

    Qt3DCore::QEntity *lightEntity4 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light4 = new Qt3DRender::QPointLight(lightEntity4);
    light4->setColor(Qt::white);
    light4->setIntensity(intensity);
    lightEntity4->addComponent(light4);
    Qt3DCore::QTransform *lightTransform4 = new Qt3DCore::QTransform(lightEntity4);
    lightTransform4->setTranslation(positionList.at(3));
    lightEntity4->addComponent(lightTransform4);

    Qt3DCore::QEntity *lightEntity5 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light5 = new Qt3DRender::QPointLight(lightEntity5);
    light5->setColor(Qt::white);
    light5->setIntensity(intensity);
    lightEntity5->addComponent(light5);
    Qt3DCore::QTransform *lightTransform5 = new Qt3DCore::QTransform(lightEntity5);
    lightTransform5->setTranslation(positionList.at(4));
    lightEntity5->addComponent(lightTransform5);

    Qt3DCore::QEntity *lightEntity6 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light6 = new Qt3DRender::QPointLight(lightEntity6);
    light6->setColor(Qt::white);
    light6->setIntensity(intensity);
    lightEntity6->addComponent(light6);
    Qt3DCore::QTransform *lightTransform6 = new Qt3DCore::QTransform(lightEntity6);
    lightTransform6->setTranslation(positionList.at(5));
    lightEntity6->addComponent(lightTransform6);
}

void ODFvisualizer::start()
{

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(Qt::black);
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 10);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("pHARDI ODFs Viewer"));

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    // Camera
    cameraEntity = view->camera();

    // For camera controls
    Qt3DExtras::OrbitCameraControllerModified *camController = new Qt3DExtras::OrbitCameraControllerModified(rootEntity);
    camController->setLinearSpeed( 50.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(cameraEntity);


    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    //cameraEntity->setPosition(QVector3D(0, 0, meshCenter.z() + maxDistZ + 10));
    cameraEntity->setPosition(QVector3D(0, 0, 50));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0,0,0));
    //cameraEntity->setViewCenter(meshCenter);

    setSceneLighting();

    // Set root object of the scene
    view->setRootEntity(rootEntity);

    // Create control widgets

    // ODF section
    QGroupBox *odfGB = new QGroupBox(widget);
    odfGB->setTitle("ODF Options");
    QVBoxLayout *odfMainLayout = new QVBoxLayout(odfGB);
    loadODFFileButton = new QPushButton(odfGB);
    loadODFFileButton->setText("Select the ODF File");
    loadODFFileButton->setMaximumWidth(180);

    loadMaskFileButton = new QPushButton(odfGB);
    loadMaskFileButton->setText("Select Mask File");
    loadMaskFileButton->setMaximumWidth(180);
    loadMaskFileButton->setToolTip("Only the ODFs of the voxels belonging to the mask will be displayed.");

    xSliceCB = new QCheckBox(odfGB);
    xSliceCB->setChecked(false);
    xSliceCB->setText(QStringLiteral("Display X Slice (Left - Right)"));

    ySliceCB = new QCheckBox(odfGB);
    ySliceCB->setChecked(false);
    ySliceCB->setText(QStringLiteral("Display Y Slice (Anterior - Posterior)"));

    zSliceCB = new QCheckBox(odfGB);
    zSliceCB->setChecked(false);
    zSliceCB->setText(QStringLiteral("Display Z Slice (Inferior - Superior)"));

    xSliceSpinBox = new QSpinBox(odfGB);
    xSliceSpinBox->setRange(1,1000);
    QLabel *xSliceLabel = new QLabel("Slice X Number", odfGB);
    QHBoxLayout *xRowLayout = new QHBoxLayout;
    xRowLayout->addWidget(xSliceLabel);
    xRowLayout->addWidget(xSliceSpinBox);

    ySliceSpinBox = new QSpinBox(odfGB);
    ySliceSpinBox->setRange(1,1000);
    QLabel *ySliceLabel = new QLabel("Slice Y Number", odfGB);
    QHBoxLayout *yRowLayout = new QHBoxLayout;
    yRowLayout->addWidget(ySliceLabel);
    yRowLayout->addWidget(ySliceSpinBox);

    zSliceSpinBox = new QSpinBox(odfGB);
    zSliceSpinBox->setRange(1,1000);
    QLabel *zSliceLabel = new QLabel("Slice Z Number", odfGB);
    QHBoxLayout *zRowLayout = new QHBoxLayout;
    zRowLayout->addWidget(zSliceLabel);
    zRowLayout->addWidget(zSliceSpinBox);

    odfMainLayout->addWidget(loadODFFileButton);
    odfMainLayout->addWidget(loadMaskFileButton);
    odfMainLayout->addWidget(xSliceCB);
    odfMainLayout->addWidget(ySliceCB);
    odfMainLayout->addWidget(zSliceCB);
    odfMainLayout->addLayout(xRowLayout);
    odfMainLayout->addLayout(yRowLayout);
    odfMainLayout->addLayout(zRowLayout);


    QGroupBox *scalarGB = new QGroupBox(widget);
    scalarGB->setTitle("Background Image Options");
    QVBoxLayout *scalarMainLayout = new QVBoxLayout(scalarGB);
    loadScalarFileButton = new QPushButton(scalarGB);
    loadScalarFileButton->setText("Select Background Image");
    loadScalarFileButton->setMaximumWidth(180);
    loadScalarFileButton->setToolTip("The ODfs will be displayed over this background image (ie. FA or MD map)");


    xSliceBgCB = new QCheckBox(scalarGB);
    xSliceBgCB->setChecked(false);
    xSliceBgCB->setText(QStringLiteral("Show X Slice Background"));

    ySliceBgCB = new QCheckBox(scalarGB);
    ySliceBgCB->setChecked(false);
    ySliceBgCB->setText(QStringLiteral("Show Y Slice Background"));

    zSliceBgCB = new QCheckBox(scalarGB);
    zSliceBgCB->setChecked(false);
    zSliceBgCB->setText(QStringLiteral("Show Z Slice Background"));

    scalarMainLayout->addWidget(loadScalarFileButton);
    scalarMainLayout->addWidget(xSliceBgCB);
    scalarMainLayout->addWidget(ySliceBgCB);
    scalarMainLayout->addWidget(zSliceBgCB);


    QGroupBox *reconstGB = new QGroupBox(widget);
    reconstGB->setTitle("Reconstruction Scheme Options");
    QVBoxLayout *reconstMainLayout = new QVBoxLayout(reconstGB);

    QLabel *reconstText = new QLabel(reconstGB);
    reconstText->setWordWrap(true);
    reconstText->setMaximumWidth(240);
    reconstText->setAlignment(Qt::AlignJustify);
    reconstText->setText("Custom reconstruction scheme can be specified. <b>IMPORTANT</b>: The scheme must be the same than the one employed to compute the ODFs.<br>"
                         "<b>Vertex File</b> must contain a Nx3 matrix with vertex coordinates.<br>"
                         "<b>Faces File</b> must contain a Nx3 matrix with faces indices. The face file is mandatory to create the ODFs patches.<br>"
                         "If the ODFs were computed using a semisphere reconstruction scheme then the whole sphere ODF can be displayed by checking <b>Duplicate Reconstruction Scheme</b>. <br>"
                         "<b>NOTE:</b> The faces file must contain the indices for both sides of the sphere.<br>"
                         "<b>Default Scheme:</b> 362 directions over the semisphere.");
    loadReconstVerticesFileButton = new QPushButton(reconstGB);
    loadReconstVerticesFileButton->setText("Select Reconstruction Scheme File");
    loadReconstVerticesFileButton->setMaximumWidth(200);
    loadReconstFacesFileButton = new QPushButton(reconstGB);
    loadReconstFacesFileButton->setText("Select Reconstruction Faces File");
    loadReconstFacesFileButton->setMaximumWidth(200);

    duplicateSchemeCB = new QCheckBox(reconstGB);
    duplicateSchemeCB->setText(QStringLiteral("Duplicate Reconstruction Scheme"));

    reconstMainLayout->addWidget(reconstText);
    reconstMainLayout->addWidget(loadReconstVerticesFileButton);
    reconstMainLayout->addWidget(loadReconstFacesFileButton);
    reconstMainLayout->addWidget(duplicateSchemeCB);

    processButton = new QPushButton(widget);
    processButton->setText("DISPLAY ODFs");
    QFont font;
    font.setPointSize(12);
    font.setWeight(QFont::Bold);
    processButton->setFont(font);

    vLayout->addWidget(odfGB);
    vLayout->addWidget(scalarGB);
    vLayout->addWidget(reconstGB);

    vLayout->addWidget(processButton);

    connect(loadODFFileButton,SIGNAL(released()),this,SLOT(loadODFButtonClicked()));
    connect(loadMaskFileButton,SIGNAL(released()),this,SLOT(loadMaskButtonClicked()));
    connect(loadScalarFileButton,SIGNAL(released()),this,SLOT(loadScalarButtonClicked()));
    connect(loadReconstFacesFileButton,SIGNAL(released()),this,SLOT(loadReconstFacesButtonClicked()));
    connect(loadReconstVerticesFileButton,SIGNAL(released()),this,SLOT(loadReconstVerticesButtonClicked()));
    connect(processButton,SIGNAL(released()),this,SLOT(generateODFView()));

    // Show window
    widget->showMaximized();
}

void ODFvisualizer::loadODFButtonClicked()
{
    odfFileName = QFileDialog::getOpenFileName(this,tr(""), lastDir, tr("Image Files (*.nii *.nii.gz)"));
    if (!odfFileName.isEmpty())
    {
        loadODFFileButton->setText(odfFileName);
        lastDir = odfFileName;
    }
    else
        loadODFFileButton->setText("Choose ODF File");
}

void ODFvisualizer::loadScalarButtonClicked()
{
    scalarFileName = QFileDialog::getOpenFileName(this,tr(""), lastDir, tr("Image Files (*.nii *.nii.gz)"));
    if (!scalarFileName.isEmpty())
    {
        loadScalarFileButton->setText(scalarFileName);
        lastDir = scalarFileName;
    }
    else
        loadScalarFileButton->setText("Choose Scalar Image File");
}

void ODFvisualizer::loadMaskButtonClicked()
{
    maskFileName = QFileDialog::getOpenFileName(this,tr(""), lastDir, tr("Image Files (*.nii *.nii.gz)"));
    if (!maskFileName.isEmpty())
    {
        loadMaskFileButton->setText(maskFileName);
        lastDir = maskFileName;
    }
    else
        loadMaskFileButton->setText("Choose Mask Image File");
}

void ODFvisualizer::loadReconstFacesButtonClicked()
{
    reconstFacesFileName = QFileDialog::getOpenFileName(this,tr(""), lastDir, tr("Text Files (*.txt)"));
    if (!reconstFacesFileName.isEmpty())
    {
        loadReconstFacesFileButton->setText(reconstFacesFileName);
        lastDir = reconstFacesFileName;
    }
    else
        loadReconstFacesFileButton->setText("Choose Reconst Faces File");
}

void ODFvisualizer::loadReconstVerticesButtonClicked()
{
    reconstVerticesFileName = QFileDialog::getOpenFileName(this,tr(""), lastDir, tr("Text Files (*.txt)"));
    if (!reconstVerticesFileName.isEmpty())
    {
        loadReconstVerticesFileButton->setText(reconstVerticesFileName);
        lastDir = reconstVerticesFileName;
    }
    else
        loadReconstVerticesFileButton->setText("Choose Reconst Vertices File");
}

// TODO:
//      - recalculate only modified parameters
void ODFvisualizer::generateODFView()
{
    if (odfFileName.isEmpty() || !QFile::exists(odfFileName))
    {
        qDebug() << "ODF File is empty or does not exist.";
        return;
    }

    if (!xSliceCB->isChecked() && !ySliceCB->isChecked() && !zSliceCB->isChecked() &&
            maskFileName.isEmpty())
    {
        qDebug() << "Mask or slice must be selected.";
        return;
    }

    meshCenter = QVector3D(0,0,0);
    int meshCenterCounter = 0;

    cleanScene();

    static QString oldODFFileName = "";
    static QString oldMaskFileName = "";
    static QString oldVerticesFileName = "";
    static QString oldFacesFileName = "";
    if (oldODFFileName.compare(odfFileName) != 0 || oldMaskFileName.compare(maskFileName) != 0 ||
            oldVerticesFileName.compare(reconstVerticesFileName) != 0 || oldFacesFileName.compare(reconstFacesFileName) != 0)
    {
        generator = new ODFGenerator(odfFileName.toUtf8().constData());

        if (!reconstFacesFileName.isEmpty() && !reconstVerticesFileName.isEmpty())
            generator->setVertAndFacetsScheme(reconstVerticesFileName.toUtf8().constData(), reconstFacesFileName.toUtf8().constData(), duplicateSchemeCB->isChecked());

        if (!maskFileName.isEmpty())
            generator->setMask(maskFileName.toUtf8().constData());
    }
    oldODFFileName = odfFileName;

//    FemLines *lines = new FemLines;
//    lines->loadGeometryFile("/media/Neuro/ibas/TestSurfOBJ/TestCoordinateSystem.off");

//    Qt3DCore::QEntity *linesEntity = new Qt3DCore::QEntity(rootEntity);
//    Qt3DExtras::QPerVertexColorMaterial *mat = new Qt3DExtras::QPerVertexColorMaterial;
//    linesEntity->addComponent(lines);
//    linesEntity->addComponent(mat);

    int x, y, z;
    generator->getImageSize(&x, &y, &z);
    xSliceSpinBox->setMaximum(x);
    ySliceSpinBox->setMaximum(y);
    zSliceSpinBox->setMaximum(z);

    if (!xSliceCB->isChecked() && !ySliceCB->isChecked() && !zSliceCB->isChecked() && !maskFileName.isEmpty())
    {
        surface odf = generator->generateODFSurfaces();

        if (!odf.faces.is_empty() && !odf.vertices.is_empty())
        {
            FemMesh *mesh = new FemMesh;
            mesh->setCustomGeometry(odf.vertices,odf.faces,odf.colors,odf.normals);

            QVector3D newCenter;
            mesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

            meshCenter += newCenter;
            ++meshCenterCounter;

            if (m_maskODFSliceEntity != nullptr)
            {
                bool enabled = false;
                m_maskODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
                delete m_maskODFSliceEntity;
                m_maskODFSliceEntity = nullptr;
            }

            m_maskODFSliceEntity = new Qt3DCore::QEntity(rootEntity);
            PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

            m_maskODFSliceEntity->addComponent(mesh);
            m_maskODFSliceEntity->addComponent(materialTest);
        }
    }

    if (xSliceCB->isChecked())
    {
        generator->setXSlice(xSliceSpinBox->value() - 1);
        surface odf = generator->generateODFSurfaces(Axis::X);

        if (!odf.faces.is_empty() && !odf.vertices.is_empty())
        {
            FemMesh *mesh = new FemMesh;
            mesh->setCustomGeometry(odf.vertices,odf.faces,odf.colors,odf.normals);

            QVector3D newCenter;
            mesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

            meshCenter += newCenter;
            ++meshCenterCounter;

            if (m_xODFSliceEntity != nullptr)
            {
                bool enabled = false;
                m_xODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
                delete m_xODFSliceEntity;
                m_xODFSliceEntity = nullptr;
            }

            m_xODFSliceEntity = new Qt3DCore::QEntity(rootEntity);
            PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

            m_xODFSliceEntity->addComponent(mesh);
            m_xODFSliceEntity->addComponent(materialTest);
        }
    }

    if (ySliceCB->isChecked())
    {
        generator->setYSlice(ySliceSpinBox->value() - 1);
        surface odf = generator->generateODFSurfaces(Axis::Y);

        if (!odf.faces.is_empty() && !odf.vertices.is_empty())
        {
            FemMesh *mesh = new FemMesh;
            mesh->setCustomGeometry(odf.vertices,odf.faces,odf.colors,odf.normals);

            QVector3D newCenter;
            mesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

            meshCenter += newCenter;
            ++meshCenterCounter;
            if (m_yODFSliceEntity != nullptr)
            {
                bool enabled = false;
                m_yODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
                delete m_yODFSliceEntity;
                m_yODFSliceEntity = nullptr;
            }

            m_yODFSliceEntity = new Qt3DCore::QEntity(rootEntity);
            PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

            m_yODFSliceEntity->addComponent(mesh);
            m_yODFSliceEntity->addComponent(materialTest);
        }
    }

    if (zSliceCB->isChecked())
    {
        generator->setZSlice(zSliceSpinBox->value() - 1);
        surface odf = generator->generateODFSurfaces(Axis::Z);

        if (!odf.faces.is_empty() && !odf.vertices.is_empty())
        {
            FemMesh *mesh = new FemMesh;
            mesh->setCustomGeometry(odf.vertices,odf.faces,odf.colors,odf.normals);

            QVector3D newCenter;
            mesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

            meshCenter += newCenter;
            ++meshCenterCounter;

            if (m_zODFSliceEntity != nullptr)
            {
                bool enabled = false;
                m_zODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
                delete m_zODFSliceEntity;
                m_zODFSliceEntity = nullptr;
            }

            m_zODFSliceEntity = new Qt3DCore::QEntity(rootEntity);
            PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

            m_zODFSliceEntity->addComponent(mesh);
            m_zODFSliceEntity->addComponent(materialTest);
        }
    }

    if (xSliceBgCB->isChecked() && !scalarFileName.isEmpty())
    {
        surface xslice = SliceMesh::generateSliceMesh(scalarFileName.toUtf8().constData(),xSliceSpinBox->value()-1,Axis::X);

        FemMesh *xsliceMesh = new FemMesh;
        xsliceMesh->setCustomGeometry(xslice.vertices,xslice.faces,xslice.colors);

        QVector3D newCenter;
        xsliceMesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

        meshCenter += newCenter;
        ++meshCenterCounter;

        if (m_xScalarSliceEntity != nullptr)
        {
            bool enabled = false;
            m_xScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
            delete m_xScalarSliceEntity;
        }

        m_xScalarSliceEntity = new Qt3DCore::QEntity(rootEntity);
        PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

        m_xScalarSliceEntity->addComponent(xsliceMesh);
        m_xScalarSliceEntity->addComponent(materialTest);
    }

    if (ySliceBgCB->isChecked() && !scalarFileName.isEmpty())
    {
        surface yslice = SliceMesh::generateSliceMesh(scalarFileName.toUtf8().constData(),ySliceSpinBox->value()-1,Axis::Y);

        FemMesh *ysliceMesh = new FemMesh;
        ysliceMesh->setCustomGeometry(yslice.vertices,yslice.faces,yslice.colors);

        QVector3D newCenter;
        ysliceMesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

        meshCenter += newCenter;
        ++meshCenterCounter;

        if (m_yScalarSliceEntity != nullptr)
        {
            bool enabled = false;
            m_yScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
            delete m_yScalarSliceEntity;
        }

        m_yScalarSliceEntity = new Qt3DCore::QEntity(rootEntity);
        PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

        m_yScalarSliceEntity->addComponent(ysliceMesh);
        m_yScalarSliceEntity->addComponent(materialTest);
    }

    if (zSliceBgCB->isChecked() && !scalarFileName.isEmpty())
    {
        surface zslice = SliceMesh::generateSliceMesh(scalarFileName.toUtf8().constData(),zSliceSpinBox->value()-1,Axis::Z);

        FemMesh *zsliceMesh = new FemMesh;
        zsliceMesh->setCustomGeometry(zslice.vertices,zslice.faces,zslice.colors);

        QVector3D newCenter;
        zsliceMesh->getMeshCoordinateSystem(&newCenter, &maxDistX, &maxDistY, &maxDistZ);

        meshCenter += newCenter;
        ++meshCenterCounter;

        if (m_zScalarSliceEntity != nullptr)
        {
            bool enabled = false;
            m_zScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
            delete m_zScalarSliceEntity;
        }

        m_zScalarSliceEntity = new Qt3DCore::QEntity(rootEntity);
        PerVertexColorMaterialModified *materialTest = new PerVertexColorMaterialModified;

        m_zScalarSliceEntity->addComponent(zsliceMesh);
        m_zScalarSliceEntity->addComponent(materialTest);
    }

    meshCenter /= meshCenterCounter;
    cameraEntity->setViewCenter(meshCenter);
//    QVector3D upVector(xSliceCB->isChecked() ? 1 : 0,
//                       ySliceCB->isChecked() ? 1 : 0,
//                       zSliceCB->isChecked() ? 1 : 0);
//    upVector.normalize();
//    cameraEntity->setUpVector(upVector);
//    qDebug() << cameraEntity->upVector();
//    cameraEntity->setPosition(QVector3D(xSliceCB->isChecked() ? meshCenter.x() + 5 : 0,
//                                        ySliceCB->isChecked() ? meshCenter.y() + 5 : 0,
//                                        zSliceCB->isChecked() ? meshCenter.z() + 5 : 0));
}

void ODFvisualizer::cleanScene()
{
    if (!xSliceCB->isChecked() && m_xODFSliceEntity != nullptr)
    {
        bool enabled = false;
        m_xODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_xODFSliceEntity->deleteLater();
        m_xODFSliceEntity = nullptr;
    }

    if (!ySliceCB->isChecked() && m_yODFSliceEntity != nullptr)
    {
        bool enabled = false;
        m_yODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_yODFSliceEntity->deleteLater();
        m_yODFSliceEntity = nullptr;
    }

    if (!zSliceCB->isChecked() && m_zODFSliceEntity != nullptr)
    {
        bool enabled = false;
        m_zODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_zODFSliceEntity->deleteLater();
        m_zODFSliceEntity = nullptr;
    }

    if (!xSliceBgCB->isChecked() && m_xScalarSliceEntity != nullptr)
    {
        bool enabled = false;
        m_xScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_xScalarSliceEntity->deleteLater();
        m_xScalarSliceEntity = nullptr;
    }

    if (!ySliceBgCB->isChecked() && m_yScalarSliceEntity != nullptr)
    {
        bool enabled = false;
        m_yScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_yScalarSliceEntity->deleteLater();
        m_yScalarSliceEntity = nullptr;
    }

    if (!zSliceBgCB->isChecked() && m_zScalarSliceEntity != nullptr)
    {
        bool enabled = false;
        m_zScalarSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_zScalarSliceEntity->deleteLater();
        m_zScalarSliceEntity = nullptr;
    }

    if ((xSliceCB->isChecked() || ySliceCB->isChecked() || zSliceCB->isChecked()) && m_maskODFSliceEntity != nullptr)
    {
        bool enabled = false;
        m_maskODFSliceEntity->setParent(enabled ? rootEntity : nullptr);
        m_maskODFSliceEntity->deleteLater();
        m_maskODFSliceEntity = nullptr;
    }
}

