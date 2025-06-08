project('order-service', 'cpp', default_options : ['cpp_std=c++20'])

grpc_dep = dependency('gRPC')
protobuf_dep = dependency('protobuf')
spdlog_dep = dependency('spdlog', required : true)

inc = include_directories('include')

sources = files(
  'src/controller.cpp',
  'src/router.cpp',
  'src/validator.cpp',
  'src/grpc_client.cpp',
  'src/main.cpp',
  'src/router.pb.cc',
  'src/router.grpc.pb.cc'
  'src/symbol_client.cpp',
  'src/user_client.cpp'
)

executable('order_service',
  sources,
  include_directories : inc,
  dependencies : [grpc_dep, protobuf_dep, spdlog_dep]
)
