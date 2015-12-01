/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2013-2015 Matt Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/
#include "preferencesdialog.h"


PreferencesDialog::PreferencesDialog( QWidget* parent ) : QDialog(parent)
{
    setWindowTitle(tr("Preferences"));
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::init( PreferenceManager* m )
{
    Q_ASSERT( m != nullptr );
    mPrefManager = m;

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode( QListView::IconMode );
    contentsWidget->setIconSize( QSize( 96, 84 ) );
    contentsWidget->setMovement( QListView::Static );
    contentsWidget->setMaximumWidth( 128 );
    contentsWidget->setSpacing( 12 );

    GeneralPage* general = new GeneralPage( this );
    general->setManager( mPrefManager );
    general->updateValues();
    //connect(mPrefManager, &PreferenceManager::effectChanged, general, &GeneralPage::updateValues);
    
    FilesPage* file = new FilesPage( this );
    file->setManager( mPrefManager );
    file->updateValues();
    
    TimelinePage* timeline = new TimelinePage( this );
    timeline->setManager( mPrefManager );

    ToolsPage* tools = new ToolsPage( this );
    tools->setManager( mPrefManager );
    
    ShortcutsPage* shortcuts = new ShortcutsPage( this );
    shortcuts->setManager( mPrefManager );

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget( general );
    pagesWidget->addWidget( file );
    pagesWidget->addWidget( timeline );
    pagesWidget->addWidget( tools );
    pagesWidget->addWidget( shortcuts );

    QPushButton* closeButton = new QPushButton( tr( "Close" ) );
    connect( closeButton, &QPushButton::clicked, this, &PreferencesDialog::close );

    createIcons();
    contentsWidget->setCurrentRow( 0 );

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget( contentsWidget );
    horizontalLayout->addWidget( pagesWidget, 1 );

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch( 1 );
    buttonsLayout->addWidget( closeButton );

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout( horizontalLayout );
    mainLayout->addStretch( 1 );
    mainLayout->addSpacing( 12 );
    mainLayout->addLayout( buttonsLayout );
    setLayout( mainLayout );

    makeConnections();
}

void PreferencesDialog::makeConnections()
{

}

void PreferencesDialog::createIcons()
{
    QListWidgetItem* generalButton = new QListWidgetItem(contentsWidget);
    generalButton->setIcon(QIcon(":icons/prefspencil.png"));
    generalButton->setText(tr("General"));
    generalButton->setTextAlignment(Qt::AlignHCenter);
    generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem* filesButton = new QListWidgetItem(contentsWidget);
    filesButton->setIcon(QIcon(":icons/prefs-files.png"));
    filesButton->setText(tr("Files"));
    filesButton->setTextAlignment(Qt::AlignHCenter);
    filesButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem* timelineButton = new QListWidgetItem(contentsWidget);
    timelineButton->setIcon(QIcon(":icons/prefstimeline.png"));
    timelineButton->setText(tr("Timeline"));
    timelineButton->setTextAlignment(Qt::AlignHCenter);
    timelineButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem* toolsButton = new QListWidgetItem(contentsWidget);
    toolsButton->setIcon(QIcon(":/icons/prefs-files.png"));
    toolsButton->setText(tr("Tools"));
    toolsButton->setTextAlignment(Qt::AlignHCenter);
    toolsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem* shortcutsButton = new QListWidgetItem(contentsWidget);
    shortcutsButton->setIcon(QIcon(":/icons/prefs-shortcuts.png"));
    shortcutsButton->setText(tr("Shortcuts"));
    shortcutsButton->setTextAlignment(Qt::AlignHCenter);
    shortcutsButton->setFlags((Qt::ItemIsSelectable | Qt::ItemIsEnabled));

    auto onCurrentItemChanged = static_cast< void ( QListWidget::* )( QListWidgetItem*, QListWidgetItem* ) >( &QListWidget::currentItemChanged );
    connect(contentsWidget,
            onCurrentItemChanged,
            this,
            &PreferencesDialog::changePage);
}

void PreferencesDialog::closeEvent(QCloseEvent *)
{

    this->deleteLater();
}

void PreferencesDialog::changePage(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

GeneralPage::GeneralPage(QWidget* parent) : QWidget(parent)
{
    QSettings settings( PENCIL2D, PENCIL2D );
    QVBoxLayout* lay = new QVBoxLayout();

    QGroupBox* windowOpacityBox = new QGroupBox(tr("Window opacity"));
    QGroupBox* backgroundBox = new QGroupBox(tr("Background"));
    QGroupBox* appearanceBox = new QGroupBox(tr("Appearance"));
    QGroupBox* displayBox = new QGroupBox(tr("Rendering"));
    QGroupBox* editingBox = new QGroupBox(tr("Editing"));

    QLabel* windowOpacityLabel = new QLabel(tr("Opacity"));
    mWindowOpacityLevel = new QSlider(Qt::Horizontal);
    mWindowOpacityLevel->setMinimum(30);
    mWindowOpacityLevel->setMaximum(100);
    int value = settings.value("windowOpacity").toInt();
    mWindowOpacityLevel->setValue( 100 - value );

    mBackgroundButtons = new QButtonGroup();
    QRadioButton* checkerBackgroundButton = new QRadioButton();
    QRadioButton* whiteBackgroundButton = new QRadioButton();
    QRadioButton* greyBackgroundButton = new QRadioButton();
    QRadioButton* dotsBackgroundButton = new QRadioButton();
    QRadioButton* weaveBackgroundButton = new QRadioButton();

    QPixmap previewCheckerboard( ":background/checkerboard.png" );
    QPixmap previewWhite(32,32);
    QPixmap previewGrey(32,32);
    QPixmap previewDots( ":background/dots.png" );
    QPixmap previewWeave( ":background/weave.jpg" );

    previewWhite.fill( Qt::white );

    previewGrey.fill( Qt:: lightGray );

    checkerBackgroundButton->setIcon( previewCheckerboard.scaled(32, 32) );
    whiteBackgroundButton->setIcon( previewWhite );
    greyBackgroundButton->setIcon( previewGrey );
    dotsBackgroundButton->setIcon( previewDots.scaled(32, 32) );
    weaveBackgroundButton->setIcon( previewWeave.scaled(32, 32) );
    mBackgroundButtons->addButton(checkerBackgroundButton);
    mBackgroundButtons->addButton(whiteBackgroundButton);
    mBackgroundButtons->addButton(greyBackgroundButton);
    mBackgroundButtons->addButton(dotsBackgroundButton);
    mBackgroundButtons->addButton(weaveBackgroundButton);
    mBackgroundButtons->setId(checkerBackgroundButton, 1);
    mBackgroundButtons->setId(whiteBackgroundButton, 2);
    mBackgroundButtons->setId(greyBackgroundButton, 3);
    mBackgroundButtons->setId(dotsBackgroundButton, 4);
    mBackgroundButtons->setId(weaveBackgroundButton, 5);

    QHBoxLayout* backgroundLayout = new QHBoxLayout();
    backgroundBox->setLayout(backgroundLayout);
    backgroundLayout->addWidget(checkerBackgroundButton);
    backgroundLayout->addWidget(whiteBackgroundButton);
    backgroundLayout->addWidget(greyBackgroundButton);
    backgroundLayout->addWidget(dotsBackgroundButton);
    backgroundLayout->addWidget(weaveBackgroundButton);

    mShadowsBox = new QCheckBox(tr("Shadows"));
    mToolCursorsBox = new QCheckBox(tr("Tool Cursors"));
    mAntialiasingBox = new QCheckBox(tr("Antialiasing"));
    mBlurryZoomBox = new QCheckBox(tr("Blurry Zoom"));


    QGridLayout* windowOpacityLayout = new QGridLayout();
    windowOpacityBox->setLayout(windowOpacityLayout);
    windowOpacityLayout->addWidget(windowOpacityLabel, 0, 0);
    windowOpacityLayout->addWidget(mWindowOpacityLevel, 0, 1);

    QVBoxLayout* appearanceLayout = new QVBoxLayout();
    appearanceBox->setLayout(appearanceLayout);
    appearanceLayout->addWidget(mShadowsBox);
    appearanceLayout->addWidget(mToolCursorsBox);

    QGridLayout* displayLayout = new QGridLayout();
    displayBox->setLayout(displayLayout);
    displayLayout->addWidget(mAntialiasingBox, 0, 0);
    displayLayout->addWidget(mBlurryZoomBox, 1, 0);

    QLabel* curveSmoothingLabel = new QLabel(tr("Vector curve smoothing"));
    mCurveSmoothingLevel = new QSlider(Qt::Horizontal);
    mCurveSmoothingLevel->setMinimum(1);
    mCurveSmoothingLevel->setMaximum(100);
    value = settings.value("curveSmoothing").toInt();
    mCurveSmoothingLevel->setValue( value );

    mHighResBox = new QCheckBox(tr("Tablet high-resolution position"));


    QGridLayout* editingLayout = new QGridLayout();
    editingBox->setLayout(editingLayout);
    editingLayout->addWidget(curveSmoothingLabel, 0, 0);
    editingLayout->addWidget(mCurveSmoothingLevel, 1, 0);
    editingLayout->addWidget(mHighResBox, 2, 0);

    lay->addWidget(windowOpacityBox);
    lay->addWidget(appearanceBox);
    lay->addWidget(backgroundBox);
    lay->addWidget(displayBox);
    lay->addWidget(editingBox);

    PreferencesDialog* preference = qobject_cast< PreferencesDialog* >( parent );

    auto kButtonClicked = static_cast< void (QButtonGroup::* )( int ) >( &QButtonGroup::buttonClicked );
    connect( mWindowOpacityLevel, &QSlider::valueChanged, preference, &PreferencesDialog::windowOpacityChange );
    connect( mBackgroundButtons,  kButtonClicked,         this, &GeneralPage::backgroundChange );
    connect( mShadowsBox,         &QCheckBox::stateChanged, this, &GeneralPage::shadowsCheckboxStateChanged );
    connect( mToolCursorsBox,     &QCheckBox::stateChanged, this, &GeneralPage::toolCursorsCheckboxStateChanged );
    connect( mAntialiasingBox,    &QCheckBox::stateChanged, this, &GeneralPage::antiAliasCheckboxStateChanged );
    connect( mBlurryZoomBox,    &QCheckBox::stateChanged, this, &GeneralPage::blurryZoomCheckboxStateChanged );
    connect( mCurveSmoothingLevel, &QSlider::valueChanged, this, &GeneralPage::curveSmoothingChange );
    connect( mHighResBox,         &QCheckBox::stateChanged, this, &GeneralPage::highResCheckboxStateChanged );

    setLayout(lay);
}



void GeneralPage::updateValues()
{
    mCurveSmoothingLevel->setValue(mManager->getInt(SETTING::CURVE_SMOOTHING));
    mWindowOpacityLevel->setValue(100 - mManager->getInt(SETTING::WINDOW_OPACITY));
    mShadowsBox->setChecked(mManager->isOn(SETTING::SHADOW));
    mToolCursorsBox->setChecked(mManager->isOn(SETTING::TOOL_CURSOR));
    mAntialiasingBox->setChecked(mManager->isOn(SETTING::ANTIALIAS));
    mBlurryZoomBox->setChecked(mManager->isOn(SETTING::BLURRYZOOM));
    mHighResBox->setChecked(mManager->isOn(SETTING::HIGH_RESOLUTION));

    QString bgName = mManager->getString(SETTING::BACKGROUND_STYLE);
    if (bgName == "checkerboard") {
        mBackgroundButtons->button(1)->setChecked(true);
    }
    if (bgName == "white") {
        mBackgroundButtons->button(2)->setChecked(true);
    }
    if (bgName == "grey") {
        mBackgroundButtons->button(3)->setChecked(true);
    }
    if (bgName == "dots") {
        mBackgroundButtons->button(4)->setChecked(true);
    }
    if (bgName == "weave") {
        mBackgroundButtons->button(5)->setChecked(true);
    }
}

void GeneralPage::backgroundChange(int value)
{
    QString brushName = "white";
    switch (value) {
    case 1:
        brushName = "checkerboard";
        break;
    case 2:
        brushName = "white";
        break;
    case 3:
        brushName = "grey";
        break;
    case 4:
        brushName = "dots";
        break;
    case 5:
        brushName = "weave";
        break;
    default:
        break;
    }
    mManager->set(SETTING::BACKGROUND_STYLE, brushName);
}

void GeneralPage::curveSmoothingChange(int value)
{
    mManager->set(SETTING::CURVE_SMOOTHING, value);
}

void GeneralPage::highResCheckboxStateChanged( bool b )
{
    mManager->set( SETTING::HIGH_RESOLUTION, b );
}

void GeneralPage::shadowsCheckboxStateChanged( bool b )
{
    mManager->set( SETTING::SHADOW, b );
}

void GeneralPage::antiAliasCheckboxStateChanged( bool b )
{
    mManager->set( SETTING::ANTIALIAS, b );
}

void GeneralPage::blurryZoomCheckboxStateChanged( bool b )
{
    mManager->set( SETTING::BLURRYZOOM, b );
}

void GeneralPage::toolCursorsCheckboxStateChanged(bool b)
{
    mManager->set( SETTING::TOOL_CURSOR, b );
}


TimelinePage::TimelinePage(QWidget* parent) : QWidget(parent)
{
    QSettings settings("Pencil","Pencil");

    QVBoxLayout* lay = new QVBoxLayout();

    QGroupBox* timeLineBox = new QGroupBox(tr("Timeline"));
    QCheckBox* drawLabel = new QCheckBox(tr("Draw timeline labels"));
    QSpinBox* fontSize = new QSpinBox();
    QLabel* frameSizeLabel = new QLabel(tr("Frame size in Pixels"));
    QSpinBox* frameSize = new QSpinBox(this);
    QLabel* lengthSizeLabel = new QLabel(tr("Timeline size in Frames"));
    QLineEdit* lengthSize = new QLineEdit(this);
    lengthSize->setInputMask("0009");

    QCheckBox* scrubBox = new QCheckBox(tr("Short scrub"));
    scrubBox->setChecked(false); // default
    if (settings.value("shortScrub").toBool()) scrubBox->setChecked(true);

    fontSize->setMinimum(4);
    fontSize->setMaximum(20);
    frameSize->setMinimum(4);
    frameSize->setMaximum(20);

    fontSize->setFixedWidth(50);
    frameSize->setFixedWidth(50);
    lengthSize->setFixedWidth(50);

    if (settings.value("drawLabel")=="false") drawLabel->setChecked(false);
    else drawLabel->setChecked(true);
    fontSize->setValue(settings.value("labelFontSize").toInt());
    frameSize->setValue(settings.value("frameSize").toInt());
    if (settings.value("labelFontSize").toInt()==0) fontSize->setValue(12);
    if (settings.value("frameSize").toInt()==0) frameSize->setValue(6);
    lengthSize->setText(settings.value("length").toString());
    if (settings.value("length").toInt()==0) lengthSize->setText("240");

    connect(fontSize, SIGNAL(valueChanged(int)), parent, SIGNAL(fontSizeChange(int)));
    connect(frameSize, SIGNAL(valueChanged(int)), parent, SIGNAL(frameSizeChange(int)));
    connect(lengthSize, SIGNAL(textChanged(QString)), parent, SIGNAL(lengthSizeChange(QString)));
    connect(drawLabel, SIGNAL(stateChanged(int)), parent, SIGNAL(labelChange(int)));
    connect(scrubBox, SIGNAL(stateChanged(int)), parent, SIGNAL(scrubChange(int)));

    lay->addWidget(frameSizeLabel);
    lay->addWidget(frameSize);
    lay->addWidget(lengthSizeLabel);
    lay->addWidget(lengthSize);
    lay->addWidget(scrubBox);
    timeLineBox->setLayout(lay);

    QVBoxLayout* lay2 = new QVBoxLayout();
    lay2->addWidget(timeLineBox);
    lay2->addStretch(1);
    setLayout(lay2);
}

FilesPage::FilesPage(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout();

    QGroupBox* autosaveBox = new QGroupBox(tr("Autosave documents"));
    mAutosaveCheckBox = new QCheckBox(tr("Enable autosave"));
    QLabel* autosaveNumberLabel = new QLabel(tr("Number of modifications before autosaving:"));
    mAutosaveNumberBox = new QSpinBox();

    mAutosaveNumberBox->setMinimum(5);
    mAutosaveNumberBox->setMaximum(200);
    mAutosaveNumberBox->setFixedWidth(50);

    connect( mAutosaveCheckBox, &QCheckBox::stateChanged, this, &FilesPage::autosaveChange );
    connect(mAutosaveNumberBox, SIGNAL(valueChanged(int)), this, SLOT(autosaveNumberChange(int)));

    lay->addWidget(mAutosaveCheckBox);
    lay->addWidget(autosaveNumberLabel);
    lay->addWidget(mAutosaveNumberBox);
    autosaveBox->setLayout(lay);

    QVBoxLayout* lay2 = new QVBoxLayout();
    lay2->addWidget(autosaveBox);
    lay2->addStretch(1);
    setLayout(lay2);
}

void FilesPage::updateValues()
{
    mAutosaveCheckBox->setChecked(mManager->isOn(SETTING::AUTO_SAVE));
    mAutosaveNumberBox->setValue(mManager->getInt(SETTING::AUTO_SAVE_NUMBER));
}

void FilesPage::autosaveChange(bool b)
{
    mManager->set(SETTING::AUTO_SAVE, b);
}

void FilesPage::autosaveNumberChange(int number)
{
    mManager->set(SETTING::AUTO_SAVE_NUMBER, number);
}

ToolsPage::ToolsPage(QWidget* parent) : QWidget(parent)
{
    QSettings settings("Pencil","Pencil");

    QVBoxLayout* lay = new QVBoxLayout();

    QGroupBox* onionSkinBox = new QGroupBox(tr("Onion skin"));
    
    QLabel* onionMaxOpacityLabel = new QLabel(tr("Maximum onion opacity %"));
    QSpinBox* onionMaxOpacityBox = new QSpinBox();
    QLabel* onionMinOpacityLabel = new QLabel(tr("Minimum onion opacity %"));
    QSpinBox* onionMinOpacityBox = new QSpinBox();
    QLabel* onionPrevFramesNumLabel = new QLabel(tr("Number of previous onion frames shown"));
    QSpinBox* onionPrevFramesNumBox = new QSpinBox();
    QLabel* onionNextFramesNumLabel = new QLabel(tr("Number of next onion frames shown"));
    QSpinBox* onionNextFramesNumBox = new QSpinBox();

    onionMaxOpacityBox->setMinimum(0);
    onionMaxOpacityBox->setMaximum(100);
    onionMaxOpacityBox->setFixedWidth(50);
    onionMinOpacityBox->setMinimum(0);
    onionMinOpacityBox->setMaximum(100);
    onionMinOpacityBox->setFixedWidth(50);
    onionPrevFramesNumBox->setMinimum(1);
    onionPrevFramesNumBox->setMaximum(60);
    onionPrevFramesNumBox->setFixedWidth(50);
    onionNextFramesNumBox->setMinimum(1);
    onionNextFramesNumBox->setMaximum(60);
    onionNextFramesNumBox->setFixedWidth(50);

    onionMaxOpacityBox->setValue(settings.value( SETTING_ONION_MAX_OPACITY ).toInt());
    onionMinOpacityBox->setValue(settings.value( SETTING_ONION_MIN_OPACITY ).toInt());
    onionPrevFramesNumBox->setValue(settings.value( SETTING_ONION_PREV_FRAMES_NUM).toInt());
    onionNextFramesNumBox->setValue(settings.value( SETTING_ONION_NEXT_FRAMES_NUM ).toInt());

    connect(onionMaxOpacityBox, SIGNAL(valueChanged(int)), parent, SIGNAL(onionMaxOpacityChange(int)));
    connect(onionMinOpacityBox, SIGNAL(valueChanged(int)), parent, SIGNAL(onionMinOpacityChange(int)));
    connect(onionPrevFramesNumBox, SIGNAL(valueChanged(int)), parent, SIGNAL(onionPrevFramesNumChange(int)));
    connect(onionNextFramesNumBox, SIGNAL(valueChanged(int)), parent, SIGNAL(onionNextFramesNumChange(int)));

    lay->addWidget(onionMaxOpacityLabel);
    lay->addWidget(onionMaxOpacityBox);
    lay->addWidget(onionMinOpacityLabel);
    lay->addWidget(onionMinOpacityBox);
    lay->addWidget(onionPrevFramesNumLabel);
    lay->addWidget(onionPrevFramesNumBox);
    lay->addWidget(onionNextFramesNumLabel);
    lay->addWidget(onionNextFramesNumBox);
    onionSkinBox->setLayout(lay);

    QVBoxLayout* lay2 = new QVBoxLayout();
    lay2->addWidget(onionSkinBox);
    lay2->addStretch(1);
    setLayout(lay2);
}
