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

#ifndef glanzzzo_web_client_Client_hpp
#define glanzzzo_web_client_Client_hpp

#include "./RequestExecutor.hpp"

#include "../protocol/http/incoming/Response.hpp"

#include "../protocol/http/outgoing/DtoBody.hpp"
#include "../protocol/http/outgoing/BufferBody.hpp"

#include "../../../../oatpp-lib/core/src/data/stream/ChunkedBuffer.hpp"

#include "../../../../oatpp-lib/core/src/data/mapping/type/Primitive.hpp"
#include "../../../../oatpp-lib/core/src/data/mapping/type/Type.hpp"
#include "../../../../oatpp-lib/core/src/data/mapping/ObjectMapper.hpp"

#include "../../../../oatpp-lib/core/src/collection/ListMap.hpp"

#include "../../../../oatpp-lib/core/src/utils/ConversionUtils.hpp"

#include "../../../../oatpp-lib/core/src/base/Controllable.hpp"
#include "../../../../oatpp-lib/core/src/base/SharedWrapper.hpp"

#include <string>
#include <list>
#include <unordered_map>

namespace oatpp { namespace web { namespace client {
  
class ApiClient : public oatpp::base::Controllable {
public:
  static constexpr const char* const TAG = "Client";
protected:
  typedef oatpp::collection::ListMap<
    oatpp::base::String::SharedWrapper,
    oatpp::data::mapping::type::AbstractSharedWrapper
  > StringToParamMap;
protected:
  typedef std::unordered_map<std::string, std::shared_ptr<oatpp::base::String>> PathVariablesMap;
private:
  typedef oatpp::collection::ListMap<
    oatpp::base::String::SharedWrapper,
    oatpp::base::String::SharedWrapper
  > StringToStringMap;
public:
  typedef oatpp::web::protocol::http::Status Status;
  typedef oatpp::web::protocol::http::Header Header;
public:
  typedef oatpp::data::mapping::type::StringSharedWrapper String;
  typedef oatpp::data::mapping::type::Int32::SharedWrapper Int32;
  typedef oatpp::data::mapping::type::Int64::SharedWrapper Int64;
  typedef oatpp::data::mapping::type::Float32::SharedWrapper Float32;
  typedef oatpp::data::mapping::type::Float64::SharedWrapper Float64;
  typedef oatpp::data::mapping::type::Boolean::SharedWrapper Boolean;
public:
  typedef oatpp::web::protocol::http::incoming::Response Response;
protected:
  
  class PathSegment {
  public:
    constexpr static const v_int32 SEG_PATH = 0;
    constexpr static const v_int32 SEG_VAR = 1;
  public:
    PathSegment(const std::string& pText, v_int32 pType)
      : text (pText)
      , type (pType)
    {}
    const std::string text;
    const v_int32 type;
  };
  
  typedef std::list<PathSegment> PathPattern;
  
private:
  
  void formatPath(oatpp::data::stream::OutputStream* stream,
                  const PathPattern& pathPattern,
                  const std::shared_ptr<StringToParamMap>& params);
  
  void addPathQueryParams(oatpp::data::stream::OutputStream* stream,
                          const std::shared_ptr<StringToParamMap>& params);
  
  std::shared_ptr<StringToStringMap> convertParamsMap(const std::shared_ptr<StringToParamMap>& params);
  
protected:
  
  static PathSegment parsePathSegment(p_char8 data, v_int32 size, v_int32& position);
  static PathSegment parseVarSegment(p_char8 data, v_int32 size, v_int32& position);
  static PathPattern parsePathPattern(p_char8 data, v_int32 size);
  
protected:
  std::shared_ptr<RequestExecutor> m_requestExecutor;
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;
  
public:
  ApiClient(const std::shared_ptr<RequestExecutor>& requestExecutor,
            const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper)
    : m_requestExecutor(requestExecutor)
    , m_objectMapper(objectMapper)
  {}
public:
  
  static std::shared_ptr<ApiClient> createShared(const std::shared_ptr<RequestExecutor>& requestExecutor,
                                           const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper) {
    return std::shared_ptr<ApiClient>(new ApiClient(requestExecutor, objectMapper));
  }
  
protected:
  
  virtual std::shared_ptr<Response> formatAndExecuteRequest(const oatpp::base::String::SharedWrapper& method,
                                                            const PathPattern& pathPattern,
                                                            const std::shared_ptr<StringToParamMap>& headers,
                                                            const std::shared_ptr<StringToParamMap>& pathParams,
                                                            const std::shared_ptr<StringToParamMap>& queryParams,
                                                            const std::shared_ptr<RequestExecutor::Body>& body) {
    
    auto stream = oatpp::data::stream::ChunkedBuffer::createShared();
    
    formatPath(stream.get(), pathPattern, pathParams);
    
    if(queryParams) {
      addPathQueryParams(stream.get(), queryParams);
    }
    
    return m_requestExecutor->execute(method,
                                      stream->toString(),
                                      convertParamsMap(headers),
                                      body);
    
  }
  
};
  
}}}

#endif /* glanzzzo_web_client_Client_hpp */
