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
#include "qmyserver.h"
#include <QByteArray>
#include <QSslKey>
#include <QFile>

 #define CACERTIFICATES_FILE "cacert.pem"
#define LOCALCERTIFICATE_FILE "/home/skynet/.potcoin/server.cert"
#define PRIVATEKEY_FILE "/home/skynet/.potcoin/server.pem"


void QMyServer::incomingConnection (int socketDescriptor)
{
   qDebug("QMyServer::incomingConnection(%d)", socketDescriptor);

   socket = new QSslSocket;

   if (!socket) {
      qWarning("not enough memory to create new QSslSocket");
      return;
   }
   socket->setProtocol(QSsl::AnyProtocol);

   _connectSocketSignals();

   if (!socket->setSocketDescriptor(socketDescriptor)) {
      qWarning("couldn't set socket descriptor");
      delete socket;
      return;
   }
   _startServerEncryption();
}

void QMyServer::_startServerEncryption ()
{
   bool b;
   QFile file(PRIVATEKEY_FILE);

   file.open(QIODevice::ReadOnly);
   if (!file.isOpen()) {
      qWarning("could'n open %s", PRIVATEKEY_FILE);
      socket->disconnectFromHost();
      return;
   }

   QSslKey key(&file, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");

   if (key.isNull()) {
      qWarning("key is null");
      socket->disconnectFromHost();
      return;
   }

   b = socket->addCaCertificates(CACERTIFICATES_FILE);
   if (!b) {
      qWarning() << "Couldn't add CA certificates" << endl;
   }
   else {
      socket->setLocalCertificate(LOCALCERTIFICATE_FILE);
      socket->setPrivateKey(key);
      socket->startServerEncryption();
   }
}

void QMyServer::_connectSocketSignals ()
{
   connect(socket, SIGNAL(encrypted()), this, SLOT(slot_encrypted()));
   connect(socket, SIGNAL(encryptedBytesWritten(qint64)),
      this, SLOT(slot_encryptedBytesWritten(qint64)));
   connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)),
      this, SLOT(slot_modeChanged(QSslSocket::SslMode)));
   connect(socket, SIGNAL(peerVerifyError(const QSslError &)),
      this, SLOT(slot_peerVerifyError (const QSslError &)));
   connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),
      this, SLOT(slot_sslErrors(const QList<QSslError> &)));
   connect(socket, SIGNAL(readyRead()),
      this, SLOT(slot_readyRead()));
   connect(socket, SIGNAL(connected()),
      this, SLOT(slot_connected()));
   connect(socket, SIGNAL(disconnected()),
      this, SLOT(slot_disconnected()));
   connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(slot_error(QAbstractSocket::SocketError)));
   connect(socket, SIGNAL(hostFound()),
      this, SLOT(slot_hostFound()));
   connect(socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
      this, SLOT(slot_proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
   connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
      this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
}

void QMyServer::slot_encrypted ()
{
   qDebug("QMyServer::slot_encrypted");
}

void QMyServer::slot_encryptedBytesWritten (qint64 written)
{
   qDebug("QMyServer::slot_encryptedBytesWritten(%ld)", (long) written);
}

void QMyServer::slot_modeChanged (QSslSocket::SslMode mode)
{
   qDebug("QMyServer::slot_modeChanged(%d)", mode);
}

void QMyServer::slot_peerVerifyError (const QSslError &)
{
   qDebug("QMyServer::slot_peerVerifyError");
}

void QMyServer::slot_sslErrors (const QList<QSslError> &)
{
   qDebug("QMyServer::slot_sslErrors");
}

void QMyServer::slot_readyRead ()
{
   qDebug("QMyServer::slot_readyRead");
}

void QMyServer::slot_connected ()
{
   qDebug("QMyServer::slot_connected");
}

void QMyServer::slot_disconnected ()
{
   qDebug("QMyServer::slot_disconnected");
}

void QMyServer::slot_error (QAbstractSocket::SocketError err)
{
   qDebug() << "QMyServer::slot_error(" << err << ")";
   qDebug() << socket->errorString();
}

void QMyServer::slot_hostFound ()
{
   qDebug("QMyServer::slot_hostFound");
}

void QMyServer::slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *)
{
   qDebug("QMyServer::slot_proxyAuthenticationRequired");
}

void QMyServer::slot_stateChanged (QAbstractSocket::SocketState state)
{
   qDebug() << "QMyServer::slot_stateChanged(" << state << ")";
}
