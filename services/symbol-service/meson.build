project('symbol-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

spdlog_dep = dependency('spdlog')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/orderbook_manager.cpp',
  'src/snapshot_generator.cpp',
  'src/health_check.cpp',
]

executable('symbol-service',
  srcs,
  include_directories: inc,
  dependencies: [spdlog_dep])
