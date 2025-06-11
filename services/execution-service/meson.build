project('execution-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

add_project_arguments('-w', language: 'cpp')  # disable warnings

cc = meson.get_compiler('cpp')

# ───── Dependencies ─────
boost_inc = include_directories('/opt/homebrew/opt/boost/include')
boost_dep = declare_dependency(
  include_directories: boost_inc,
  link_args: ['-L/opt/homebrew/opt/boost/lib', '-lboost_system', '-lboost_thread']
)

redis_inc = include_directories('/opt/homebrew/include', '/opt/homebrew/include/sw')
redis_dep = declare_dependency(
  include_directories: redis_inc,
  link_args: ['-L/opt/homebrew/lib', '-lredis++', '-lhiredis']
)


spdlog_dep   = dependency('spdlog')
protobuf_dep = dependency('protobuf')
grpc_dep     = dependency('grpc')
grpcpp_dep   = dependency('grpc++')
json_dep     = dependency('nlohmann_json', required: false)  # optional

grpc_reflection_dep = declare_dependency(
  link_args: ['-L/opt/homebrew/Cellar/grpc/1.73.0/lib', '-lgrpc++_reflection']
)

# ───── Source Files ─────
srcs = files(
  'src/main.cpp',
  'src/grpc_server.cpp',
  'src/execution_router.cpp',
  'src/executor_limit.cpp',
  'src/executor_market.cpp',
  'src/executor_sl.cpp',
  'src/logger.cpp',
  'src/redis_publisher.cpp',
  'src/pnl_client.cpp',
  'src/user_client.cpp',
  'src/order.pb.cc',
  'src/order.grpc.pb.cc',
  'src/pnl.pb.cc',
  'src/pnl.grpc.pb.cc',
  'src/user.pb.cc',
  'src/user.grpc.pb.cc'
)

# ───── Include Paths ─────
inc = include_directories('include', '.')

# ───── Executable ─────
executable('execution-service',
  sources: srcs,
  include_directories: inc,
  dependencies: [
    boost_dep,
    spdlog_dep,
    grpc_dep,
    grpcpp_dep,
    grpc_reflection_dep,
    protobuf_dep,
    redis_dep,
    json_dep
  ]
)
