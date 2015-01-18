/* Copyright 2014-2015 Grant Hutchinson

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

One exception, this software is not allowed to be used by Simon Choucroun or any assosciate or affiliate individual/company
*/

#ifndef QMYSERVER_H
#define QMYSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QList>

class QMyServer : public QTcpServer {

Q_OBJECT

protected:

   void incomingConnection (int socketDescriptor);

private:

   QSslSocket *socket;

   void _startServerEncryption ();
   void _connectSocketSignals ();

private slots:

   void slot_encrypted ();
   void slot_encryptedBytesWritten (qint64 written);
   void slot_modeChanged (QSslSocket::SslMode mode);
   void slot_peerVerifyError (const QSslError &error);
   void slot_sslErrors (const QList<QSslError> &errors);

   void slot_connected ();
   void slot_disconnected ();
   void slot_error (QAbstractSocket::SocketError);
   void slot_hostFound ();
   void slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *);
   void slot_stateChanged (QAbstractSocket::SocketState);

   void slot_readyRead ();

};

#endif // QMYSERVER_H
