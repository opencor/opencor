/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Solver interface
//==============================================================================

#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QList>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

enum Type {
    Dae,
    Nla,
    Ode
};

//==============================================================================

enum PropertyType {
    Integer,
    Double
};

//==============================================================================

class Property
{
public:
    explicit Property(const PropertyType &pType, const QString &pId,
                      const Descriptions &pDescriptions,
                      const QVariant &pDefaultValue,
                      const bool &pHasVoiUnit = false);

    PropertyType type() const;
    QString id() const;
    Descriptions descriptions() const;
    QVariant defaultValue() const;
    bool hasVoiUnit() const;

private:
    PropertyType mType;
    QString mId;
    Descriptions mDescriptions;
    QVariant mDefaultValue;
    bool mHasVoiUnit;
};

//==============================================================================

typedef QList<Property> Properties;

//==============================================================================

}   // namespace Solver

//==============================================================================

class SolverInterface
{
public:
#define INTERFACE_DEFINITION
    #include "solverinterface.inl"
#undef INTERFACE_DEFINITION

    QString typeAsString() const;
};

//==============================================================================

typedef QList<SolverInterface *> SolverInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR::SolverInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
