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

#ifndef __LANGUAGE_NODE_TYPE_H__
#define __LANGUAGE_NODE_TYPE_H__

#include <nodes/NodeData>

using namespace QtNodes;

namespace pyplugin
{
    class SyntaxNodeType;
    typedef std::shared_ptr<SyntaxNodeType> NodeTypeRef;
    
    /**
     * This links python plugins with the nodal editor. A token is a representation of an
     * input/output property on a master node.
     */
    class SyntaxNodeType : NodeData
    {
    public:
        /**
         * Construct a dynamic language node property. This is used for inputs/outputs on
         * nodes.
         *
         * @param type Type name for this type (ex: "int"/"String")
         * @param package Package that this type is included in (if any)
         */
        SyntaxNodeType(std::string type, std::string package = "");
        
        virtual NodeDataType type() const override;
        
    private:
        const QString m_type;
        const QString m_package;
    };
}

#endif // !__LANGUAGE_NODE_TYPE_H__
