project('user-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

redis_dep  = dependency('hiredis')
spdlog_dep = dependency('spdlog')

boost_dep = dependency('boost', modules: ['system'], required: false)  # optional for now

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/redis_safe_client.cpp',
  'src/user_wallet_store.cpp',
  'src/user_metadata_store.cpp',
  'src/margin_listener.cpp',
  'src/scheduler.cpp',
  'src/health_checker.cpp',
]

executable('user-service',
  srcs,
  include_directories: inc,
  dependencies: [redis_dep, spdlog_dep])
