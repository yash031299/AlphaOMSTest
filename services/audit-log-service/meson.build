project('audit-log-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

nlohmann_json_dep = dependency('nlohmann_json')
spdlog_dep = dependency('spdlog')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/log_writer.cpp',
]

executable('audit-log-service',
  srcs,
  include_directories: inc,
  dependencies: [nlohmann_json_dep, spdlog_dep])
