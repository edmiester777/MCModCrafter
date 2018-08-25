/*************************************************************************
* MCModCrafter Program to help introduce people to minecraft modding and
* to speed development time via logic visualization.
* 
* Copyright (C) 2017 Edward Callahan
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#ifndef __MASTER_NODE_MODEL_H__
#define __MASTER_NODE_MODEL_H__

#include <stdafx.h>
#include <nodes/NodeDataModel>

using namespace QtNodes;

/**
 * This class defines the model for the master node used for most funcitonality
 * in our custom node editor.
 */
class MasterNodeModel : public NodeDataModel
{
    Q_OBJECT
public:
    /**
     * Construct a new master node.
     */
    MasterNodeModel(QString title);
    virtual ~MasterNodeModel();
    
    QString caption() const override;
    QString name() const override;
    
private:
    const QString m_title;
};

#endif //!__MASTER_NODE_MODEL_H__
