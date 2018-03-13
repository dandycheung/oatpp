/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
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
 ***************************************************************************/

#include "Library.hpp"

#include <memory>
#include <unistd.h>
#include <sys/socket.h>

namespace oatpp { namespace os { namespace io {
  
v_int32 Library::handle_close(v_handle handle){
  return close(handle);
}
  
Library::v_size Library::handle_read(v_handle handle, void *buf, v_size count){
  return read(handle, buf, count);
}

Library::v_size Library::handle_write(v_handle handle, const void *buf, v_size count){
  return write(handle, buf, count);
}

}}}
