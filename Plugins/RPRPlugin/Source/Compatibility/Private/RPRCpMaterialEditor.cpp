/*************************************************************************
* Copyright 2020 Advanced Micro Devices
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/

#if WITH_EDITOR

#include "RPRCpMaterialEditor.h"

#if ENGINE_MINOR_VERSION == 18

FName FRPRCpMaterialEditor::GetMaterialExpressionInputName(UMaterialExpression* MaterialExpression, int32 InputIndex)
{
    return (*MaterialExpression->GetInputName(InputIndex));
}


#elif ENGINE_MINOR_VERSION >= 19


FName FRPRCpMaterialEditor::GetMaterialExpressionInputName(UMaterialExpression* MaterialExpression, int32 InputIndex)
{
    return (MaterialExpression->GetInputName(InputIndex));
}


#endif // ENGINE_MINOR_VERSIOn

#endif // WITH_EDITOR
