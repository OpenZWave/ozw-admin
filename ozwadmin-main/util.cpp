/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <QObject>
#include "util.h"

const char *nodeBasicStr (qint8 basic)
{
  switch (basic) {
  case 1:
    return "Controller";
  case 2:
    return "Static Controller";
  case 3:
    return "Slave";
  case 4:
    return "Routing Slave";
  }
  return "unknown";
}

QString SockErrorToString(QAbstractSocket::SocketError error) 
{
  switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
      return QObject::tr("Connection Refused");
    case QAbstractSocket::RemoteHostClosedError:
      return QObject::tr("Remote Host Closed");
    case QAbstractSocket::HostNotFoundError:
      return QObject::tr("Host Not Found");
    case QAbstractSocket::SocketAccessError:
      return QObject::tr("Socket Access Error");
    case QAbstractSocket::SocketResourceError:
      return QObject::tr("Socket Resource Error");
    case QAbstractSocket::SocketTimeoutError:
      return QObject::tr("Socket Timeout");
    case QAbstractSocket::DatagramTooLargeError:
      return QObject::tr("Datagram Too Large");
    case QAbstractSocket::NetworkError:
      return QObject::tr("Network Error");
    case QAbstractSocket::AddressInUseError:
      return QObject::tr("Address In Use");
    case QAbstractSocket::SocketAddressNotAvailableError:
      return QObject::tr("Socket Address Not Available");
    case QAbstractSocket::UnsupportedSocketOperationError:
      return QObject::tr("Unsupported Socket Operation");
    case QAbstractSocket::ProxyAuthenticationRequiredError:
      return QObject::tr("Proxy Authentication Required");
    case QAbstractSocket::SslHandshakeFailedError:
      return QObject::tr("SSL Handshake Failed");
    case QAbstractSocket::UnfinishedSocketOperationError:
      return QObject::tr("Unfinished Socket Operation");
    case QAbstractSocket::ProxyConnectionRefusedError:
      return QObject::tr("Proxy Connection Refused");
    case QAbstractSocket::ProxyConnectionClosedError:
      return QObject::tr("Proxy Connection Closed");
    case QAbstractSocket::ProxyConnectionTimeoutError:
      return QObject::tr("Proxy Connection Timeout");
    case QAbstractSocket::ProxyNotFoundError:
      return QObject::tr("Proxy Not Found");
    case QAbstractSocket::ProxyProtocolError:
      return QObject::tr("Proxy Protocol Error");
    case QAbstractSocket::OperationError:
      return QObject::tr("Operation Error");
    case QAbstractSocket::SslInternalError:
      return QObject::tr("SSL Internal Error");
    case QAbstractSocket::SslInvalidUserDataError:
      return QObject::tr("SSL Invalid User/Certificate");
    case QAbstractSocket::TemporaryError:
      return QObject::tr("Temporary Error");
    case QAbstractSocket::UnknownSocketError:
      return QObject::tr("Unknown Socket Error");
  }
}