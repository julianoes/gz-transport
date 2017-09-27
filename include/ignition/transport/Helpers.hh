/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGN_TRANSPORT_HELPERS_HH_
#define IGN_TRANSPORT_HELPERS_HH_

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>

#include "ignition/transport/Export.hh"

namespace ignition
{
  namespace transport
  {
    /// \brief Constant used when not interested in throttling.
    static const uint64_t kUnthrottled = std::numeric_limits<uint64_t>::max();

    /// \brief Find the environment variable '_name' and return its value.
    /// \param[in] _name Name of the environment variable.
    /// \param[out] _value Value if the variable was found.
    /// \return True if the variable was found or false otherwise.
    IGNITION_TRANSPORT_VISIBLE
    bool env(const std::string &_name,
             std::string &_value);
  }
}

// Use safer functions on Windows
#ifdef _MSC_VER
  #define ign_strcat strcat_s
  #define ign_strcpy strcpy_s
  #define ign_sprintf sprintf_s
  #define ign_strdup _strdup
#else
  #define ign_strcat std::strcat
  #define ign_strcpy std::strcpy
  #define ign_sprintf std::sprintf
  #define ign_strdup strdup
#endif

// __IGN_TRANSPORT_HELPERS_HH_INCLUDED__
#endif
