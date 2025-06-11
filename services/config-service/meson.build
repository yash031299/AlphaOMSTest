project('config-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

add_project_arguments('-w', language: 'cpp')  # disable warnings

cc = meson.get_compiler('cpp')

# Dependencies
boost_inc = include_directories('/opt/homebrew/opt/boost/include')
boost_dep = declare_dependency(
  include_directories: boost_inc,
  link_args: ['-L/opt/homebrew/opt/boost/lib', '-lboost_system', '-lboost_thread']
)

spdlog_dep   = dependency('spdlog')
protobuf_dep = dependency('protobuf')
grpc_dep     = dependency('grpc')
grpcpp_dep   = dependency('grpc++')
json_dep     = dependency('nlohmann_json')

# Add manually located gRPC reflection dylib
grpc_reflection_dep = declare_dependency(
  link_args: ['-L/opt/homebrew/Cellar/grpc/1.73.0/lib', '-lgrpc++_reflection']
)


# Source files
srcs = files(
  'src/main.cpp',
  'src/config_loader.cpp',
  'src/grpc_server.cpp',
  'src/config.pb.cc',
  'src/config.grpc.pb.cc'
)

inc = include_directories('include', '.')

executable('config-service',
  sources : srcs,
  include_directories : inc,
  dependencies : [
    boost_dep,
    spdlog_dep,
    grpc_dep,
    grpcpp_dep,
    grpc_reflection_dep,
    protobuf_dep,
    json_dep
  ]
)