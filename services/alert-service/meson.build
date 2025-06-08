project('alert-service', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++20'])

boost_dep = dependency('boost', modules: ['asio'])
spdlog_dep = dependency('spdlog')
json_dep = dependency('nlohmann_json')

inc = include_directories('include')

srcs = [
  'src/main.cpp',
  'src/rule_engine.cpp',
  'src/anomaly_detector.cpp',
  'src/alert_dispatcher.cpp'
]

executable('alert-service',
  srcs,
  include_directories: inc,
  dependencies: [boost_dep, spdlog_dep, json_dep])
