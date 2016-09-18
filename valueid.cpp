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
#include <QDebug>

#include "Manager.h"

QtValueID::QtValueID(const OpenZWave::ValueID &vid) :
    m_vid(vid)
{
    qDebug() << "New ValueID " << vid.GetNodeId();
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
    case OpenZWave::ValueID::ValueType_Byte:
    case OpenZWave::ValueID::ValueType_Decimal:
    case OpenZWave::ValueID::ValueType_Int:
    case OpenZWave::ValueID::ValueType_List:
    case OpenZWave::ValueID::ValueType_Schedule:
    case OpenZWave::ValueID::ValueType_Short:
    case OpenZWave::ValueID::ValueType_String:
    case OpenZWave::ValueID::ValueType_Button:
    case OpenZWave::ValueID::ValueType_Raw:
    {
        return QVariant();
        break;
    }
    }
}
