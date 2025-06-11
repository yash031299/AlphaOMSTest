project('accounting-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

# Dependency: Boost manually declared (asio, system, thread)
boost_inc = include_directories('/opt/homebrew/opt/boost/include')
boost_lib = declare_dependency(
  include_directories: boost_inc,
  link_args: ['-L/opt/homebrew/opt/boost/lib', '-lboost_system', '-lboost_thread']
)

# Dependency: spdlog (from brew)
spdlog_dep = dependency('spdlog', required: true)

# Dependency: libpqxx (PostgreSQL)
libpqxx_dep = dependency('libpqxx', required: true)

# Dependency: hiredis
hiredis_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include'),
  link_args: ['-L/opt/homebrew/lib', '-lhiredis']
)

# Dependency: cpp-httplib (header-only)
httplib_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include')
)

# Dependency: Protobuf and gRPC
protobuf_dep = dependency('protobuf', required: true)
grpc_dep = dependency('grpc', required: true)

# Dependency: rapidjson
rapidjson_dep = declare_dependency(
  include_directories: include_directories('/opt/homebrew/include')
)

grpcpp_dep = dependency('grpc++', required: true)



# Include headers
inc = include_directories('include')

# Source files
srcs = [
  'src/main.cpp',
  'src/ledger.cpp',
  'src/funding_handler.cpp',
  'src/pnl_tracker.cpp',
  'src/settlement_engine.cpp',
  'src/redis_listener.cpp',
  'src/metrics_server.cpp',
  'src/grpc_interface.cpp',
  'src/postgres_writer.cpp',
  'src/accounting.grpc.pb.cc',
  'src/accounting.pb.cc',
  'src/postgres_writer.cpp'
]

add_project_arguments('-w', language: 'cpp')  # disables all warnings

# Final build target
executable('accounting-service',
  srcs,
  include_directories: inc,
  dependencies: [
    boost_lib,
    spdlog_dep,
    libpqxx_dep,
    hiredis_dep,
    httplib_dep,
    protobuf_dep,
    rapidjson_dep,
    grpcpp_dep,
    grpc_dep
  ])
