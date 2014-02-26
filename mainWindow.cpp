#include "mainWindow.hpp"


mainWindow::mainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    sbar_ = statusBar();
    createActions();
    createToolBar();
    generateMenu();
    QString windowTitle = "Moth Hamster Wheel Data Parser ";
    windowTitle += HAMSTER_VERSION;
    setWindowTitle(windowTitle);
}


void mainWindow::createActions()
{
    openAct_ = new QAction(tr("Open File"), this);
    openAct_->setStatusTip(tr("Opens a .txt or .csv file to be parsed"));
    openAct_->setShortcut(QKeySequence("ctrl+o"));
    openAct_->setIcon(QIcon("icons/fileopen.png"));
    connect(openAct_, SIGNAL(triggered()), this, SLOT(openFile()));
    
    quitAct_ = new QAction(tr("Quit"), this);
    quitAct_->setStatusTip(tr("Quits the Application"));
    quitAct_->setShortcut(QKeySequence("Alt+f4"));
    quitAct_->setIcon(QIcon("icons/exit.png"));
    connect(quitAct_, SIGNAL(triggered()), this, SLOT(quit()));
    
    aboutAct_ = new QAction(tr("About"), this);
    aboutAct_->setStatusTip(tr("Displays the Credits page window"));
    aboutAct_->setIcon(QIcon("icons/messagebox_info.png"));
    connect(aboutAct_, SIGNAL(triggered()), this, SLOT(aboutWindow()));

    aboutQtAct_ = new QAction(tr("About Qt"), this);
    aboutQtAct_->setStatusTip(tr("This Program was made with Qt"));
    aboutQtAct_->setIcon(QIcon("icons/messagebox_info.png"));

    connect(aboutQtAct_, SIGNAL(triggered()), this, SLOT(aboutQtWindow()));
}

void mainWindow::createToolBar()
{
    QToolBar * toolbar = new QToolBar(this);
    openFileBtn_ = new QToolButton (this);
    openFileBtn_->setIcon(QIcon("icons/fileopen.png"));
    openFileBtn_->setStatusTip(tr("Open data file for parsing"));
    connect(openFileBtn_, SIGNAL(pressed()), this, SLOT(openFile()));
    toolbar->addWidget(openFileBtn_);
    
    openFolderWhenSaveCheckBox_ = new QCheckBox(this);
    openFolderWhenSaveCheckBox_->setText(tr("Open working folder when done?"));
    openFolderWhenSaveCheckBox_->setStatusTip(tr("Open a File Manager for easy access to new data"));
    toolbar->addWidget(openFolderWhenSaveCheckBox_);
    
    addToolBar(toolbar); // attach this to the main window toolbar position for a new toolbar
}


void mainWindow::generateMenu()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(openAct_);
    fileMenu_->addAction(quitAct_);
    
    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutAct_);
    helpMenu_->addAction(aboutQtAct_);
}

void mainWindow::aboutWindow()
{
    QString out;
    out.reserve(200);
    out = tr("The Moth Hamster Wheel is Brought to you by:\n");
    out += tr("Coding: Daniel Fialkovsky,\nResearch: Thomson Paris\n\n");
    out += tr("Icons by David Vignoni (david@icon-king.com)\n\n");
    out += tr("Special THANKS to:\n");
    out += tr("The Citrus Research and Development Foundation,\n");
    out += tr("USDA ARS CMAVE,\n");
    out += tr("University of Florida,\n");
    out += tr("For supporting this project");

    QMessageBox::about(nullptr, tr("Credits"), 
		       out);
}

void mainWindow::aboutQtWindow()
{
    QMessageBox::aboutQt(nullptr, "About Qt");
}


void mainWindow::quit()
{
     QApplication::quit();
}


void mainWindow::openFile()
{
 QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "",
                                                 tr("text, csv (*.txt *.csv)"));
 
    newParser P;
    std::pair<int, unsigned int> msg;
    msg = P.processFile(fileName.toStdString());
    
    if(ifErrorsEvaluate(msg)) // if an error exists, send a QErrorMessage and return. 
	return;
    
    P.DumpRaws();
    
    QString folder = fileName;
    int index = folder.lastIndexOf("/");
    folder = folder.left(index);
    sbar_->showMessage(tr("File(s) Saved in: %1").arg(folder));
    
    if(openFolderWhenSaveCheckBox_->isChecked())
    {

	QDir::toNativeSeparators(QApplication::applicationDirPath());
	QDesktopServices::openUrl(QUrl(folder));
    }
}

bool mainWindow::ifErrorsEvaluate(std::pair< int, unsigned int > msg)
{
    QString output;
    switch(msg.first)
    {
	case newParser::FILE_OK:
	    return false;
	case newParser::ERR_1ENTRY_PER_LINE:
	    output = tr("File is wrong!\nLine %1 has 1 entry when it needs two.").arg(msg.second);
	    break;
	case newParser::ERR_3PLUS_ENTRES_PER_LINE:
	    output = tr("File is wrong!\nLine %1 has 3+ entries when it needs two.").arg(msg.second);
	    break;
	case newParser::ERR_MILLIS_SHRUNK:
	    output.reserve(97);
	    output = tr("File is wrong!\nLine %1 shows that the data went back in time\n").arg(msg.second);
	    output += tr("When this never should have happened\n");
	    break;
	case newParser::ERR_TOXIC_CHARACTER:
	    output = tr("File data is wrong!\nLine %1 has a non-number character\n(Use 0-9 (no '-'))").arg(msg.second);
	    break;
	case newParser::FILE_EMPTY:
	    output = tr("The given file was empty");
	    break;
	default:
	    output = tr("Unspecified error in file");
	    break;
    }
    QErrorMessage* out = new QErrorMessage(this);
    out->showMessage(output);
    return true;

    
}



void mainWindow::newSession()
{

}


mainWindow::~mainWindow()
{

}
