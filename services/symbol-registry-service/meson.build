project('symbol-registry-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

protobuf_dep = dependency('protobuf')
grpc_dep = dependency('grpc')
spdlog_dep = dependency('spdlog')
hiredis_dep = dependency('', required: false)  # optional if needed later
nlohmann_json_dep = dependency('nlohmann_json')

proto_srcs = [
  'src/symbol.pb.cc',
  'src/symbol.grpc.pb.cc',
]

srcs = [
  'src/main.cpp',
  'src/grpc_server.cpp',
  'src/symbol_service.cpp',
] + proto_srcs

inc = include_directories('include')

executable('symbol-registry-service',
  srcs,
  include_directories: inc,
  dependencies: [protobuf_dep, grpc_dep, spdlog_dep, nlohmann_json_dep])
