#include <QGraphicsView>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include <QtNetwork>
#include "networkwindow.h"
#include "networkservice.h"
#include "networkserver.h"

// Const
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 300

struct NetworkWindow::Private
{
public:
   Private()
   {}

   NetworkNewClientConnection* newClientConnection;
};

NetworkWindow::NetworkWindow(QWidget* parent)
      : QMainWindow(parent), d(new Private)
{
   // Load window defaults
   setWindowTitle(tr("Shared paint - network"));
   setWindowIcon(QIcon(":/icons/spaint.png"));
   resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

   d->newClientConnection = new NetworkNewClientConnection(this);
   setCentralWidget(d->newClientConnection);

   // Load settings
   loadSettings();
}

NetworkWindow::~NetworkWindow()
{
   delete d;
}

bool NetworkWindow::observe(NetworkService* net)
{
   connect(net, SIGNAL(serverState(NetworkServer::state, QString)),
           this, SLOT(showServerState(NetworkServer::state, QString)));

   connect(d->newClientConnection, SIGNAL(submitForm(QString&, QString&)),
           net, SLOT(startClient(QString&, QString&)));
   connect(d->newClientConnection, SIGNAL(cancelForm()),
           this, SLOT(close()));

   return true;
}

void NetworkWindow::showServerState(NetworkServer::state state, const QString &msg)
{
   switch (state) {
      case NetworkServer::errStart:
         QMessageBox::critical(this, tr("Shared Paint Server"),
                               tr("Unable to start the server: %1.")
                               .arg(msg));
         break;

      case NetworkServer::start:
         // starting
         break;

      case NetworkServer::run:
         // running
         break;

      case NetworkServer::stop:
         // stopped
         break;

      default:
         QMessageBox::information(this, tr("Shared Paint Server"),
                                  tr("NOT IMPLEMENTED: Unknown state"));
   }
}

void NetworkWindow::closeEvent(QCloseEvent *event)
{
   // Save settings
   saveSettings();

   // TODO
   event->accept();
}

void NetworkWindow::saveSettings()
{
   QSettings set;

   // Window settings
   set.setValue("NetworkWindow/State",     saveState());
   set.setValue("NetworkWindow/Size",      size());
   set.setValue("NetworkWindow/Position",  pos());
   set.setValue("NetworkWindow/Maximized", windowState().testFlag(Qt::WindowMaximized));
}

void NetworkWindow::loadSettings()
{
   QSettings set;

   // Window settings
   if(set.contains("NetworkWindow/State"))
   {
      resize(set.value("NetworkWindow/Size").toSize());
      move(set.value("NetworkWindow/Position").toPoint());

      if(set.value("NetworkWindow/Maximized").toBool())
         setWindowState(windowState() | Qt::WindowMaximized);

      restoreState(set.value("NetworkWindow/State").toByteArray());
   }
   else
   {
      // Defaults
      resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   }
}

struct NetworkNewClientConnection::Private : public QWidget
{
public:
   Private()
   {
   }

   QString host;
   QLabel* hostLabel;
   QLineEdit* hostEdit;

   QString port;
   QLabel* portLabel;
   QLineEdit* portEdit;

   QPushButton* ok;
   QPushButton* cancel;

   QGridLayout* layout;
};

NetworkNewClientConnection::NetworkNewClientConnection(QWidget* parent)
      : QWidget(parent), d(new Private)
{
   d->hostLabel = new QLabel(QString(tr("IP address:")));
   d->hostEdit = new QLineEdit("");
   d->hostLabel->setBuddy(d->hostEdit);
   d->hostLabel->setFocus();

   d->portLabel = new QLabel(QString(tr("Port:")));
   d->portEdit = new QLineEdit("");
   d->portLabel->setBuddy(d->portEdit);

   d->ok = new QPushButton(tr("Ok"));
   d->cancel = new QPushButton(tr("Cancel"));

   d->layout = new QGridLayout;
   d->layout->addWidget(d->hostLabel, 0, 0);
   d->layout->addWidget(d->hostEdit, 0, 1);
   d->layout->addWidget(d->portLabel, 1, 0);
   d->layout->addWidget(d->portEdit, 1, 1);
   d->layout->addWidget(d->ok, 2, 0);
   d->layout->addWidget(d->cancel, 2, 1);

   connect(d->ok, SIGNAL(clicked()), this, SLOT(okClicked()));
   connect(d->cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

   this->setLayout(d->layout);
}

NetworkNewClientConnection::~NetworkNewClientConnection()
{
   delete d;
}

void NetworkNewClientConnection::okClicked()
{
   qDebug() << "okClicked()";
   d->host = d->hostEdit->text();
   d->port = d->portEdit->text();
   emit submitForm(d->host, d->port);
}

void NetworkNewClientConnection::cancelClicked()
{
   qDebug() << "cancelClicked()";
   emit cancelForm();
}

#include "networkwindow.moc"
