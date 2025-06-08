executable('execution_service',
  [
    'src/main.cpp',
    'src/grpc_server.cpp',
    'src/executor_limit.cpp',
    'src/executor_market.cpp',
    'src/executor_sl.cpp',
    'src/logger.cpp',
    'src/proto/order.pb.cc',
    'src/proto/order.grpc.pb.cc',
    'src/execution_router.cpp',
    'src/redis_publisher.cpp',
    'src/pnl_client.cpp',
    'src/user_client.cpp',

  ],
  include_directories : [
    include_directories('include'),
    include_directories('include/proto')
  ],
  dependencies : [
    dependency('fmt'),
    dependency('spdlog'),
    dependency('boost'),
    dependency('protobuf'),
    dependency('grpc'),
    dependency('redis++')
  ]
)
