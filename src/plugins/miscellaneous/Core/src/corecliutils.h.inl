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
// Core CLI utilities
//==============================================================================

QString CORE_EXPORT osName();

QString CORE_EXPORT copyright();

QString CORE_EXPORT formatErrorMessage(const QString &pErrorMessage,
                                       const bool &pLowerCase = true,
                                       const bool &pDotDotDot = false);

bool CORE_EXPORT readTextFromUrl(const QString &pUrl, QString &pText,
                                 QString *pErrorMessage = 0);

QString CORE_EXPORT nonDiacriticString(const QString &pString);

//==============================================================================
// End of file
//==============================================================================
