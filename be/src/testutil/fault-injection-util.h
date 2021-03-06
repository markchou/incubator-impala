// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef IMPALA_TESTUTIL_FAULT_INJECTION_UTIL_H
#define IMPALA_TESTUTIL_FAULT_INJECTION_UTIL_H

#include "util/time.h"

namespace impala {

#ifndef NDEBUG

class FaultInjectionUtil {
 public:
  enum RpcCallType {
    RPC_NULL = 0,
    RPC_EXECQUERYFINSTANCES,
    RPC_CANCELQUERYFINSTANCES,
    RPC_PUBLISHFILTER,
    RPC_UPDATEFILTER,
    RPC_TRANSMITDATA,
    RPC_REPORTEXECSTATUS,
    RPC_RANDOM    // This must be last.
  };

  enum RpcExceptionType {
    RPC_EXCEPTION_NONE = 0,
    RPC_EXCEPTION_LOST_CONNECTION_SEND,
    RPC_EXCEPTION_LOST_CONNECTION_RECV,
    RPC_EXCEPTION_SSL_ERROR_SEND,
    RPC_EXCEPTION_SSL_ERROR_RECV
  };

  /// Test util function that injects delays to specified RPC server handling function
  /// so that RPC caller could hit the RPC recv timeout condition.
  /// 'my_type' specifies which RPC type of the current function.
  /// FLAGS_fault_injection_rpc_type specifies which RPC function the delay should
  /// be enabled. FLAGS_fault_injection_rpc_delay_ms specifies the delay in ms.
  static void InjectRpcDelay(RpcCallType my_type);

  /// Test util function that injects exceptions to RPC client functions.
  /// 'my_type' specifies which RPC type of the current function. Currently, only
  /// TransmitData() is supported.
  /// 'is_send' indicates whether injected fault is at the send RPC call or recv RPC.
  ///  It's true if for send RPC call and false for recv RPC call.
  /// FLAGS_fault_injection_rpc_exception_type specifies the exception to be injected.
  static void InjectRpcException(RpcCallType my_type, bool is_send);

 private:
  static int32_t GetTargetRPCType();

};

#define FAULT_INJECTION_RPC_DELAY(type)                         \
    FaultInjectionUtil::InjectRpcDelay(FaultInjectionUtil::type)
#define FAULT_INJECTION_RPC_EXCEPTION(type, is_send)            \
    FaultInjectionUtil::InjectRpcException(FaultInjectionUtil::type, is_send)

#else // NDEBUG

#define FAULT_INJECTION_RPC_DELAY(type)
#define FAULT_INJECTION_RPC_EXCEPTION(type, is_send)

#endif

}
#endif // IMPALA_TESTUTIL_FAULT_INJECTION_UTIL_H
