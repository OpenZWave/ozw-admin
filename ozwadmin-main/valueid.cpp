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
#include "valueid.h"
#include "util.h"
#include <QDebug>

#include "Manager.h"

QtValueID::QtValueID(const OpenZWave::ValueID &vid) :
    m_vid(vid)
{
    qDebug() << "New ValueID " << this->GetLabel();
}
QtValueID::QtValueID() :
    m_vid((uint32)0,(uint64)0)
{
    qDebug() << "invalid constructor";
}

QtValueID::QtValueID(const QtValueID &other) :
    m_vid(other.m_vid)
{
}

QtValueID::~QtValueID() {

}





uint32 QtValueID::GetHomeId() const {
    return this->m_vid.GetHomeId();
}
uint32 QtValueID::GetNodeId() const {
    return this->m_vid.GetNodeId();
}
OpenZWave::ValueID::ValueGenre QtValueID::GetGenre() const {
    return this->m_vid.GetGenre();
}
uint8 QtValueID::GetCommmandClassId() const {
    return this->m_vid.GetCommandClassId();
}
uint8 QtValueID::GetInstance() const {
    return this->m_vid.GetInstance();
}
uint8 QtValueID::GetIndex() const {
    return this->m_vid.GetIndex();
}
uint8 QtValueID::GetType() const {
    return this->m_vid.GetType();
}

QString QtValueID::GetLabel() const {
    return OpenZWave::Manager::Get()->GetValueLabel(this->m_vid).c_str();
}

OpenZWave::ValueID const &QtValueID::getValueID() const {
    return this->m_vid;
}

QVariant QtValueID::getValue() {
    qDebug() << "HomeID: " << this->GetHomeId();
    switch (this->GetType()) {
    case OpenZWave::ValueID::ValueType_Bool: {
        bool ret;
        if (OpenZWave::Manager::Get()->GetValueAsBool(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Byte: {
        uint8 ret;
        if (OpenZWave::Manager::Get()->GetValueAsByte(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Decimal:  {
        float ret;
        if (OpenZWave::Manager::Get()->GetValueAsFloat(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Int:  {
        int32 ret;
        if (OpenZWave::Manager::Get()->GetValueAsInt(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_List:  {
        int32 ret;
        if (OpenZWave::Manager::Get()->GetValueListSelection(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Schedule: {
        qDebug() << "QtValueID getValue() Schedule todo";
        break;
    }
    case OpenZWave::ValueID::ValueType_Short: {
        int16 ret;
        if (OpenZWave::Manager::Get()->GetValueAsShort(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_String: {
        std::string ret;
        if (OpenZWave::Manager::Get()->GetValueAsString(this->m_vid, &ret))
                return QString::fromStdString(ret);
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Button: {
        bool ret;
        if (OpenZWave::Manager::Get()->GetValueAsBool(this->m_vid, &ret))
                return ret;
        else
                return QVariant();
        break;
    }
    case OpenZWave::ValueID::ValueType_Raw: {
        qDebug() << "TODO QtValueID Raw";
        return QVariant();
        break;
    }
    }
}

QStringList QtValueID::GetValueListItems() const {
    std::vector<std::string> Items;
    if (OpenZWave::Manager::Get()->GetValueListItems(this->m_vid, &Items)) {
        QStringList QItems = VectorString_to_QStringList(Items);
        return QItems;
    } else {
        return QStringList();
    }

}
