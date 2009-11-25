#ifndef NETWORKWINDOW_H
#define NETWORKWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"

class NetworkWindow : public QMainWindow
{
   Q_OBJECT

   public:
   NetworkWindow(QWidget* parent = 0);
   ~NetworkWindow();

   bool observe(NetworkService* net);

   protected:
   void saveSettings();
   void loadSettings();
   void closeEvent(QCloseEvent* event);

   private slots:
   void showServerState(NetworkServer::state state, const QString& msg = QString());
   void promptClientConnection();

   private:
   struct Private;
   Private* d;
};

#endif // NETWORKWINDOW_H