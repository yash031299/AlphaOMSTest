project('audit-log-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

# -------------------
# External Dependencies
# -------------------

boost_inc = include_directories('/opt/homebrew/opt/boost/include')
boost_lib = declare_dependency(
  include_directories: boost_inc,
  link_args: ['-L/opt/homebrew/opt/boost/lib', '-lboost_system', '-lboost_thread']
)

spdlog_dep = dependency('spdlog', required: true)
grpc_dep = dependency('grpc', required: true)
grpcpp_dep = dependency('grpc++', required: true)
protobuf_dep = dependency('protobuf', required: true)
nlohmann_json_dep = dependency('nlohmann_json', required: true)

# gRPC reflection library (linked manually)
grpc_reflection_dep = declare_dependency(
  link_args: ['-L/opt/homebrew/lib', '-lgrpc++_reflection']
)

hiredis_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include'),
  link_args: ['-L/opt/homebrew/lib', '-lhiredis']
)

rapidjson_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include')
)

# -------------------
# Source Structure
# -------------------

inc = include_directories('include')

srcs = files(
  'src/main.cpp',
  'src/grpc_server.cpp',
  'src/redis_consumer.cpp',
  'src/log_writer.cpp',
  'src/log_writer_async.cpp',
  'src/health_check.cpp',
  'src/api_server.cpp',
  'src/log_event.pb.cc',
  'src/log_event.grpc.pb.cc'
)

add_project_arguments('-w', language: 'cpp')  # Suppress all warnings

# -------------------
# Target
# -------------------

executable('audit-log-service',
  srcs,
  include_directories: inc,
  dependencies: [
    boost_lib,
    spdlog_dep,
    grpc_dep,
    grpcpp_dep,
    protobuf_dep,
    hiredis_dep,
    rapidjson_dep,
    nlohmann_json_dep,
    grpc_reflection_dep
  ])

# -------------------
# Test Target
# -------------------
test_srcs = files('tests/test_log_writer.cpp')

executable('test-log-writer',
  test_srcs + files(
    'src/log_writer.cpp',
    'src/log_event.pb.cc'
  ),
  include_directories: inc,
  dependencies: [
    protobuf_dep,
    nlohmann_json_dep,
    spdlog_dep
  ]
)

